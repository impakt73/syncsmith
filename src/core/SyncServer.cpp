#include <core/SyncServer.h>
#include <core/Track.h>
#include <core/FloatTrack.h>

bool SyncServer::Start(int inPort)
{
    if(mServer.listen(QHostAddress::Any, inPort))
    {
        mPort = inPort;

        connect(&mServer, &QTcpServer::newConnection, this, &SyncServer::OnNewConnection);

        qDebug() << "Server Started on Port" << mPort;

        return true;
    }
    else
    {
        return false;
    }
}

void SyncServer::Stop()
{
    if(mServer.isListening())
    {
        mServer.close();
        mClients.clear();
    }

    qDebug() << "Server Stopped";
}

/*
void SyncServer::BroadcastMessage(const QString& inMessage)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);

    out << (quint16)0;
    out << inMessage;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    for(auto& client : mClients)
    {
        client.second->write(block);
    }
}
*/

void SyncServer::OnNewConnection()
{
    QTcpSocket* newClient = mServer.nextPendingConnection();
    QString peerName = newClient->peerAddress().toString() + ":" + QString::number(newClient->peerPort());
    Q_ASSERT(mClients.find(peerName) == mClients.end()); // Make sure this user doesn't already exist
    SyncClient* syncClient = new SyncClient(newClient);
    mClients[peerName] = syncClient;

    connect(syncClient, &SyncClient::Disconnected, this, &SyncServer::OnClientDisconnected);
    connect(syncClient, &SyncClient::PacketReceived, this, &SyncServer::OnPacketReceived);

    qDebug() << "Client '" << peerName << "' Connected.";
}

void SyncServer::OnClientDisconnected()
{
    SyncClient* syncClient = static_cast<SyncClient*>(sender());
    const QTcpSocket* clientSocket = syncClient->GetSocket();
    QString peerName = clientSocket->peerAddress().toString() + ":" + QString::number(clientSocket->peerPort());
    std::map<QString, SyncClient*>::iterator socketIterator = mClients.find(peerName);
    Q_ASSERT(socketIterator != mClients.end()); // Make sure the user that is disconnecting already exists

    qDebug() << "Client '" << socketIterator->first << "' Disconnected.";

    delete socketIterator->second;
    mClients.erase(socketIterator);
}

void SyncServer::OnPacketReceived(SyncPacket *inPacket)
{
    qDebug() << "Received Packet!";

    switch(inPacket->Type)
    {
    case kSyncPacketType_Undefined:
        break;
    case kSyncPacketType_Handshake:
        break;
    case kSyncPacketType_AddTrack:
    {
        SyncPacket_AddTrack* addTrackPacket = static_cast<SyncPacket_AddTrack*>(inPacket);
        mSyncContext.AddTrack(addTrackPacket->TrackName.toStdString(), addTrackPacket->TrackType);

        qDebug() << "Added New Track With Name " << addTrackPacket->TrackName << " And Type " << addTrackPacket->TrackType;
        break;
    }
    case kSyncPacketType_RenameTrack:
        break;
    case kSyncPacketType_RemoveTrack:
    {
        SyncPacket_RemoveTrack* removeTrackPacket = static_cast<SyncPacket_RemoveTrack*>(inPacket);
        mSyncContext.RemoveTrack(removeTrackPacket->TrackName.toStdString());

        qDebug() << "Removed Track With Name " << removeTrackPacket->TrackName;
        break;
    }
    case kSyncPacketType_AddKey:
    {
        SyncPacket_AddKey* addKeyPacket = static_cast<SyncPacket_AddKey*>(inPacket);
        FloatTrack* floatTrack = static_cast<FloatTrack*>(mSyncContext.GetTrack(addKeyPacket->TrackName.toStdString()));
        floatTrack->AddKey(addKeyPacket->Position, addKeyPacket->Data);

        qDebug() << "Added Key to " << addKeyPacket->TrackName << " at " << addKeyPacket->Position << " with value " << addKeyPacket->Data;
        break;
    }
    case kSyncPacketType_ModifyKey:
    {
        SyncPacket_ModifyKey* modifyKeyPacket = static_cast<SyncPacket_ModifyKey*>(inPacket);
        FloatTrack* floatTrack = static_cast<FloatTrack*>(mSyncContext.GetTrack(modifyKeyPacket->TrackName.toStdString()));
        unsigned int keyIndex = 0;
        if(floatTrack->GetKeyIndex(modifyKeyPacket->Position, &keyIndex))
        {
            floatTrack->GetKey(keyIndex).SetData(modifyKeyPacket->Data);
        }
        qDebug() << "Modified Key on " << modifyKeyPacket->TrackName << " at " << modifyKeyPacket->Position << " to value " << modifyKeyPacket->Data;
        break;
    }
    case kSyncPacketType_RemoveKey:
    {
        SyncPacket_RemoveKey* removeKeyPacket = static_cast<SyncPacket_RemoveKey*>(inPacket);
        FloatTrack* floatTrack = static_cast<FloatTrack*>(mSyncContext.GetTrack(removeKeyPacket->TrackName.toStdString()));
        unsigned int keyIndex = 0;
        if(floatTrack->GetKeyIndex(removeKeyPacket->Position, &keyIndex))
        {
            floatTrack->RemoveKey(keyIndex);
        }
        qDebug() << "Removed Key from " << removeKeyPacket->TrackName << " at " << removeKeyPacket->Position;
        break;
    }
    case kSyncPacketType_Seek:
    {
        SyncPacket_Seek* seekPacket = static_cast<SyncPacket_Seek*>(inPacket);
        if(mSeekCallback != nullptr)
            mSeekCallback(seekPacket->Position);
        break;
    }
    case kSyncPacketType_Play:
    {
        if(mPlayCallback != nullptr)
            mPlayCallback();
        break;
    }
    case kSyncPacketType_Pause:
        break;
    case kSyncPacketType_Response:
        break;

    }

    delete inPacket;
}

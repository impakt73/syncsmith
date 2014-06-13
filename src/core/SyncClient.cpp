#include <core/SyncClient.h>

void SyncClient::Connect(const QString &inAddress, qint16 inPort)
{
    // Reset any previous connections
    mSocket.abort();

    mSocket.connectToHost(inAddress, inPort);
    connect(&mSocket, &QTcpSocket::connected, this, &SyncClient::on_connected);
    connect(&mSocket, &QTcpSocket::disconnected, this, &SyncClient::on_connected);
    connect(&mSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_error(QAbstractSocket::SocketError)));
    connect(&mSocket, &QTcpSocket::stateChanged, this, &SyncClient::on_state_changed);
    connect(&mSocket, &QTcpSocket::readyRead, this, &SyncClient::on_data_received);
}

void SyncClient::Disconnect()
{
    if(mSocket.state() == QAbstractSocket::ConnectedState)
    {
        mSocket.disconnectFromHost();
    }
}

void SyncClient::SendPacket(SyncPacket *inPacket)
{
    QByteArray block;
    QDataStream output(&block, QIODevice::WriteOnly);
    output.setVersion(QDataStream::Qt_5_3);

    // Size
    output << (quint16)0;

    output << (quint32)inPacket->Type;

    switch(inPacket->Type)
    {
    case kSyncPacketType_Undefined:
    {
        break;
    }
    case kSyncPacketType_Handshake:
    {
        SyncPacket_Handshake* handshakePacket = static_cast<SyncPacket_Handshake*>(inPacket);
        output << handshakePacket->Message;
        break;
    }
    case kSyncPacketType_AddTrack:
    {
        SyncPacket_AddTrack* addTrackPacket = static_cast<SyncPacket_AddTrack*>(inPacket);
        output << addTrackPacket->TrackName;
        output << (quint32)addTrackPacket->TrackType;
        break;
    }
    case kSyncPacketType_RenameTrack:
    {
        SyncPacket_RenameTrack* renameTrackPacket = static_cast<SyncPacket_RenameTrack*>(inPacket);
        output << renameTrackPacket->TrackName;
        output << renameTrackPacket->NewTrackName;
        break;
    }
    case kSyncPacketType_RemoveTrack:
    {
        SyncPacket_RemoveTrack* removeTrackPacket = static_cast<SyncPacket_RemoveTrack*>(inPacket);
        output << removeTrackPacket->TrackName;
        break;
    }
    case kSyncPacketType_AddKey:
        break;
    case kSyncPacketType_ModifyKey:
        break;
    case kSyncPacketType_RemoveKey:
        break;
    case kSyncPacketType_Seek:
    {
        SyncPacket_Seek* seekPacket = static_cast<SyncPacket_Seek*>(inPacket);
        output << seekPacket->Position;
        break;
    }
    }

    output.device()->seek(0);
    output << (quint16)(block.size() - sizeof(quint16));
    mSocket.write(block);

    delete inPacket;
}

void SyncClient::on_connected()
{
    emit Connected();
}

void SyncClient::on_disconnected()
{
    emit Disconnected();
}

void SyncClient::on_error(QAbstractSocket::SocketError inError)
{
    emit Error(inError, mSocket.errorString());
}

void SyncClient::on_state_changed(QAbstractSocket::SocketState inNewState)
{
    switch(inNewState)
    {
    case QAbstractSocket::UnconnectedState:
    {
        emit StateChanged(inNewState, "Not Connected");
        break;
    }
    case QAbstractSocket::HostLookupState:
    {
        emit StateChanged(inNewState, "Looking Up Host");
        break;
    }
    case QAbstractSocket::ConnectingState:
    {
        emit StateChanged(inNewState, "Connecting");
        break;
    }
    case QAbstractSocket::ConnectedState:
    {
        emit StateChanged(inNewState, "Connected");
        break;
    }
    case QAbstractSocket::BoundState:
    {
        emit StateChanged(inNewState, "Bound");
        break;
    }
    case QAbstractSocket::ListeningState:
    {
        emit StateChanged(inNewState, "Listening");
        break;
    }
    case QAbstractSocket::ClosingState:
    {
        emit StateChanged(inNewState, "Closing");
        break;
    }
    }
}

void SyncClient::on_data_received()
{
    QDataStream dataStream(&mSocket);
    dataStream.setVersion(QDataStream::Qt_5_3);

    if(mNextPacketSize == 0)
    {
        if(mSocket.bytesAvailable() < static_cast<int>(sizeof(quint16)))
        {
            // We still don't know how much to read
            return;
        }

        // Read the size of the next packet
        dataStream >> mNextPacketSize;
    }

    if(mSocket.bytesAvailable() < mNextPacketSize)
    {
        // Wait until we're able to read a full packet
        return;
    }

    // Reset the size
    mNextPacketSize = 0;

    eSyncPacketType packetType;
    dataStream >> (quint32&)packetType;

    switch(packetType)
    {
    case kSyncPacketType_Undefined:
    {
        break;
    }
    case kSyncPacketType_Handshake:
    {
        SyncPacket_Handshake* packet = new SyncPacket_Handshake();
        packet->Type = packetType;
        dataStream >> packet->Message;

        emit PacketReceived(packet);
        break;
    }
    case kSyncPacketType_AddTrack:
    {
        SyncPacket_AddTrack* packet = new SyncPacket_AddTrack();
        packet->Type = packetType;
        dataStream >> packet->TrackName;
        dataStream >> (quint32&)packet->TrackType;

        emit PacketReceived(packet);
        break;
    }
    case kSyncPacketType_RenameTrack:
    {
        SyncPacket_RenameTrack* packet = new SyncPacket_RenameTrack();
        packet->Type = packetType;
        dataStream >> packet->TrackName;
        dataStream >> packet->NewTrackName;

        emit PacketReceived(packet);
        break;
    }
    case kSyncPacketType_RemoveTrack:
    {
        SyncPacket_RemoveTrack* packet = new SyncPacket_RemoveTrack();
        packet->Type = packetType;
        dataStream >> packet->TrackName;

        emit PacketReceived(packet);
        break;
    }
    case kSyncPacketType_AddKey:
        break;
    case kSyncPacketType_ModifyKey:
        break;
    case kSyncPacketType_RemoveKey:
        break;
    case kSyncPacketType_Seek:
    {
        SyncPacket_Seek* packet = new SyncPacket_Seek();
        packet->Type = packetType;
        dataStream >> packet->Position;

        emit PacketReceived(packet);
        break;
    }
    case kSyncPacketType_Play:
    {
        SyncPacket_Play* packet = new SyncPacket_Play();
        packet->Type = packetType;

        emit PacketReceived(packet);
        break;
    }
    case kSyncPacketType_Pause:
    {
        SyncPacket_Pause* packet = new SyncPacket_Pause();
        packet->Type = packetType;

        emit PacketReceived(packet);
        break;
    }
    }
}

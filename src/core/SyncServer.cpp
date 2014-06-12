#include <core/SyncServer.h>

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
    if(mTimer.isActive())
    {
        mTimer.stop();
    }

    if(mServer.isListening())
    {
        mServer.close();
        mClients.clear();
    }

    qDebug() << "Server Stopped";
}

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

void SyncServer::OnToggleAction(double inStartPos)
{
    if(mTimer.isActive())
    {
        mTimer.stop();
    }
    else
    {
        mStartTimeOffset = inStartPos;
        mStartTime = QDateTime::currentMSecsSinceEpoch();
        mContext.SetPosition(mStartTimeOffset);
        emit PositionChanged(mStartTimeOffset);

        mTimer.start();
    }
}

void SyncServer::OnNewConnection()
{
    QTcpSocket* newClient = mServer.nextPendingConnection();
    QString peerName = newClient->peerAddress().toString() + ":" + QString::number(newClient->peerPort());
    Q_ASSERT(mClients.find(peerName) == mClients.end()); // Make sure this user doesn't already exist
    mClients[peerName] = newClient;

    connect(newClient, &QTcpSocket::disconnected, this, &SyncServer::OnClientDisconnected);

    qDebug() << "Client '" << peerName << "' Connected.";
}

void SyncServer::OnClientDisconnected()
{
    QTcpSocket* clientSocket = static_cast<QTcpSocket*>(sender());
    QString peerName = clientSocket->peerAddress().toString() + ":" + QString::number(clientSocket->peerPort());
    std::map<QString, QTcpSocket*>::iterator socketIterator = mClients.find(peerName);
    Q_ASSERT(socketIterator != mClients.end()); // Make sure the user that is disconnecting already exists

    qDebug() << "Client '" << socketIterator->first << "' Disconnected.";

    delete socketIterator->second;
    mClients.erase(socketIterator);
}

void SyncServer::OnTimerTicked()
{
    double currentTimeInSeconds = (static_cast<double>(QDateTime::currentMSecsSinceEpoch() - mStartTime) / 1000.0) + mStartTimeOffset;
    mContext.SetPosition(currentTimeInSeconds);

    emit PositionChanged(currentTimeInSeconds);
}

#pragma once

#include <QObject>
#include <QTcpSocket>
#include <core/SyncPacket.h>

class SyncClient : public QObject
{
    Q_OBJECT

public:
    SyncClient(void) : mSocket(nullptr), mNextPacketSize(0) {}
    SyncClient(QTcpSocket* inSocket) : mSocket(inSocket), mNextPacketSize(0)
    {
        connect(mSocket, &QTcpSocket::connected, this, &SyncClient::on_connected);
        connect(mSocket, &QTcpSocket::disconnected, this, &SyncClient::on_disconnected);
        connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_error(QAbstractSocket::SocketError)));
        connect(mSocket, &QTcpSocket::stateChanged, this, &SyncClient::on_state_changed);
        connect(mSocket, &QTcpSocket::readyRead, this, &SyncClient::on_data_received);
    }

    ~SyncClient(void)
    {
        if(mSocket != nullptr)
        {
            delete mSocket;
            mSocket = nullptr;
        }
    }

    void Connect(const QString& inAddress, qint16 inPort);
    void Disconnect(void);
    void SendPacket(SyncPacket* inPacket); // This function deletes your packet!

    inline const QTcpSocket* GetSocket(void) const { return mSocket; }

signals:
    void Connected();
    void Disconnected();
    void Error(QAbstractSocket::SocketError inError, const QString& inErrorString);
    void StateChanged(QAbstractSocket::SocketState inState, const QString& inStateString);
    void PacketReceived(SyncPacket* inPacket); // Packet must be deleted!

private slots:
    void on_connected(void);
    void on_disconnected(void);
    void on_error(QAbstractSocket::SocketError inError);
    void on_state_changed(QAbstractSocket::SocketState inNewState);
    void on_data_received(void);

private:
    QTcpSocket* mSocket;
    quint16 mNextPacketSize;
};

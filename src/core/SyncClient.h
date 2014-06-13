#pragma once

#include <QObject>
#include <QTcpSocket>
#include <core/SyncPacket.h>

class SyncClient : public QObject
{
    Q_OBJECT

public:
    SyncClient(void) : mNextPacketSize(0) {}
    ~SyncClient(void) {}

    void Connect(const QString& inAddress, qint16 inPort);
    void Disconnect(void);
    void SendPacket(SyncPacket* inPacket); // This function deletes your packet!

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
    QTcpSocket mSocket;
    quint16 mNextPacketSize;
};

#pragma once

#include <QtNetwork>
#include <SyncContext.h>

class SyncServer : public QObject
{
    Q_OBJECT

public:
    explicit SyncServer(QObject *parent = 0)
        : QObject(parent)
        , mServer(this) {}

    ~SyncServer(void) {}

    bool Start(int inPort = 8000);
    void Stop(void);

    void BroadcastMessage(const QString& inMessage);

    inline SyncContext& GetSyncContext(void) { return mContext; }

private slots:
    void OnNewConnection(void);
    void OnClientDisconnected(void);

private:
    SyncContext mContext;
    QTcpServer mServer;
    int mPort;
    std::map<QString, QTcpSocket*> mClients;
};

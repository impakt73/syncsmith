#pragma once

#include <QtNetwork>
#include <core/SyncClient.h>
#include <core/SyncContext.h>

typedef void (*PlayCallbackFunc)(void);
typedef void (*SeekCallbackFunc)(double);

class SyncServer : public QObject
{
    Q_OBJECT

public:
    explicit SyncServer(QObject *parent = 0)
        : QObject(parent)
        , mServer(this)
        , mPlayCallback(nullptr)
        , mSeekCallback(nullptr)
    {
    }

    ~SyncServer(void) { Stop(); }

    bool Start(int inPort = 8000);
    void Stop(void);

    void SetPlayCallback(PlayCallbackFunc inFunction) { mPlayCallback = inFunction; }
    void SetSeekCallback(SeekCallbackFunc inFunction) { mSeekCallback = inFunction; }

    const SyncContext& GetSyncContext(void) const { return mSyncContext; }

private slots:
    void OnNewConnection(void);
    void OnClientDisconnected(void);

    void OnPacketReceived(SyncPacket* inPacket);

private:
    SyncContext mSyncContext;
    QTcpServer mServer;
    int mPort;
    std::map<QString, SyncClient*> mClients;

    PlayCallbackFunc mPlayCallback;
    SeekCallbackFunc mSeekCallback;
};

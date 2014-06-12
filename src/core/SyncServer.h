#pragma once

#include <QtNetwork>
#include <SyncContext.h>
#include <ISyncSmithServer.h>
#include <QTimer>

class SyncServer : public QObject, public ISyncSmithServer
{
    Q_OBJECT

public:
    explicit SyncServer(QObject *parent = 0)
        : QObject(parent)
        , mServer(this)
        , mTimer(this)
        , mStartTime(0)
        , mStartTimeOffset(0.0)
    {
        mTimer.setInterval(16);
        mTimer.setTimerType(Qt::PreciseTimer);
        connect(&mTimer, &QTimer::timeout, this, &SyncServer::OnTimerTicked);

        mStartTime = QDateTime::currentMSecsSinceEpoch();
    }

    ~SyncServer(void) { Stop(); }

    bool Start(int inPort = 8000);
    void Stop(void);

    void BroadcastMessage(const QString& inMessage);

    inline SyncContext* GetSyncContext(void) { return &mContext; }

    inline void Release(void) { delete this; }

public slots:
    void OnToggleAction(double inStartPos);

private slots:
    void OnNewConnection(void);
    void OnClientDisconnected(void);
    void OnTimerTicked(void);

signals:
    void PositionChanged(double inNewPosition);

private:
    SyncContext mContext;
    QTcpServer mServer;
    int mPort;
    std::map<QString, QTcpSocket*> mClients;
    QTimer mTimer;
    qint64 mStartTime;
    double mStartTimeOffset;
};

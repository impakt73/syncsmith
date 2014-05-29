#pragma once

#include <QtNetwork>
#include <core/SyncContext.h>
#include <QTimer>

class SyncServer : public QObject
{
    Q_OBJECT

public:
    explicit SyncServer(QObject *parent = 0)
        : QObject(parent)
        , mServer(this)
        , mTimer(this)
        , mStartTime(0)
    {
        mTimer.setInterval(16);
        mTimer.setTimerType(Qt::PreciseTimer);
        connect(&mTimer, &QTimer::timeout, this, &SyncServer::OnTimerTicked);

        mStartTime = QDateTime::currentMSecsSinceEpoch();
    }

    ~SyncServer(void) {}

    bool Start(int inPort = 8000);
    void Stop(void);

    void BroadcastMessage(const QString& inMessage);

    inline SyncContext& GetSyncContext(void) { return mContext; }

public slots:
    void OnToggleAction(void);

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
};

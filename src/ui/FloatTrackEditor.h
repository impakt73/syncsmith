#pragma once

#include <QWidget>
#include <core/SyncClient.h>
#include <core/FloatTrack.h>

class Track;

class FloatTrackEditor : public QWidget
{
    Q_OBJECT

public:
    explicit FloatTrackEditor(Track* inTrack, SyncClient* inSyncClient, QWidget *parent = 0);
    ~FloatTrackEditor() {}

    Track* track(void) const { return mTrack; }
    void setTrack(Track* inTrack) { mTrack = inTrack; }

private:
    void paintEvent(QPaintEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    Track* mTrack;
    SyncClient* mSyncClient;
    QPoint mLastMousePosition;

    bool mIsEditingKey;

signals:
    void editingFinished();
    void KeyAdded(FloatTrack* inFloatTrack, double inPosition, float inData);
    void KeyModified(FloatTrack* inFloatTrack, double inPosition, float inData);
    void KeyRemoved(FloatTrack* inFloatTrack, double inPosition);

};

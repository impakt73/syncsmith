#pragma once

#include <QWidget>

class TimelineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimelineWidget(QWidget *parent = 0);
    ~TimelineWidget();

    inline double scrollOffset() const { return mScrollOffset; }
    inline double length() const { return mLengthInSeconds; }
    inline double userPlaybackPosition() const { return mUserPlaybackPositionOffset; }

public slots:
    void updateScrollOffset(double inScrollOffset);
    void setPlaybackPosition(double inSeconds);
    void setLength(double inSeconds);

private:
    void paintEvent(QPaintEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    double mScrollOffset; // In Seconds
    double mPlaybackPositionInSeconds;
    double mUserPlaybackPositionOffset;
    double mLengthInSeconds;
};

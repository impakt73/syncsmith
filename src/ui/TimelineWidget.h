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

public slots:
    void updateScrollOffset(double inScrollOffset);
    void setPlaybackPosition(double inSeconds);
    void setLength(double inSeconds);

private:
    void paintEvent(QPaintEvent * event);
    double mScrollOffset; // In Seconds
    double mPlaybackPositionInSeconds;
    double mPlaybackPositionOffset;
    double mLengthInSeconds;
};

#pragma once

#include <QListView>
#include <TrackItemDelegate.h>
#include <TimelineWidget.h>

class TrackListView : public QListView
{
    Q_OBJECT

public:
    explicit TrackListView(QWidget *parent = 0);
    ~TrackListView();

private:
    TrackItemDelegate mItemDelegate;
    TimelineWidget mTimelineWidget;
    QListView mTrackHeadersListView;

    void scrollContentsBy(int dx, int dy);
    void resizeEvent(QResizeEvent *e);
    void setModel(QAbstractItemModel *model);
};

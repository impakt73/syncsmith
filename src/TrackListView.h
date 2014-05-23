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
};

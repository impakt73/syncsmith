#pragma once

#include <QListView>
#include <ui/TrackItemDelegate.h>

class TrackListView : public QListView
{
    Q_OBJECT

public:
    explicit TrackListView(QWidget *parent = 0);
    ~TrackListView();

private:
    TrackItemDelegate mItemDelegate;
};

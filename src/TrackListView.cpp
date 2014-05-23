#include <TrackListView.h>
#include <QtGui>
#include <QScrollBar>

TrackListView::TrackListView(QWidget *parent)
    : QListView(parent)
    , mTimelineWidget(this)
{
    setItemDelegate(&mItemDelegate);
    setViewportMargins(100, 30, 0, 0);

    mTimelineWidget.move(100, 0);
    mTimelineWidget.resize(width(), 30);
}

TrackListView::~TrackListView()
{

}

void TrackListView::scrollContentsBy(int dx, int dy)
{
    mTimelineWidget.updateOffset(horizontalScrollBar()->value());
    QListView::scrollContentsBy(dx, dy);
}

void TrackListView::resizeEvent(QResizeEvent *e)
{
    mTimelineWidget.resize(width(), 30);
}

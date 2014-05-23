#include <TrackListView.h>
#include <QtGui>
#include <QScrollBar>


TrackListView::TrackListView(QWidget *parent)
    : QListView(parent)
    , mTimelineWidget(this)
    , mTrackHeadersListView(this)
{
    setItemDelegate(&mItemDelegate);
    setViewportMargins(100, 30, 0, 0);

    mTimelineWidget.move(100, 0);
    mTimelineWidget.resize(width(), 30);

    mTrackHeadersListView.move(0, 30);
    mTrackHeadersListView.resize(100, height());
    mTrackHeadersListView.setItemDelegate(&mItemDelegate);
    mTrackHeadersListView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mTrackHeadersListView.setHorizontalScrollMode(QListView::ScrollPerPixel);
    mTrackHeadersListView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mTrackHeadersListView.setVerticalScrollMode(QListView::ScrollPerPixel);
    mTrackHeadersListView.setBatchSize(10);
    mTrackHeadersListView.setUniformItemSizes(true);
}

TrackListView::~TrackListView()
{

}

void TrackListView::scrollContentsBy(int dx, int dy)
{
    QListView::scrollContentsBy(dx, dy);
    float scrollVal = (float)( verticalScrollBar()->minimum() - verticalScrollBar()->value() ) / (float)( verticalScrollBar()->maximum() - verticalScrollBar()->minimum() );

    mTrackHeadersListView.verticalScrollBar()->setValue(mTrackHeadersListView.verticalScrollBar()->minimum() + (-scrollVal * (mTrackHeadersListView.verticalScrollBar()->maximum() - mTrackHeadersListView.verticalScrollBar()->minimum())));
    mTimelineWidget.updateOffset(horizontalScrollBar()->value());

    qDebug() << "Headers: " << mTrackHeadersListView.height();
    qDebug() << "Tracks: " << height();
}

void TrackListView::resizeEvent(QResizeEvent *e)
{
    mTimelineWidget.resize(width(), 30);
    mTrackHeadersListView.resize(100, height());
}

void TrackListView::setModel(QAbstractItemModel *model)
{
    QListView::setModel(model);
    mTrackHeadersListView.setModel(model);
}

#include <TrackListView.h>
#include <QtGui>


TrackListView::TrackListView(QWidget *parent)
    : QListView(parent)
{
    setItemDelegate(&mItemDelegate);
}

TrackListView::~TrackListView()
{

}

#include <TrackListView.h>


TrackListView::TrackListView(QWidget *parent)
    : QListView(parent)
{
    setItemDelegate(&mItemDelegate);
}

TrackListView::~TrackListView()
{

}

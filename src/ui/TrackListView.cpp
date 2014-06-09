#include <ui/TrackListView.h>


TrackListView::TrackListView(QWidget *parent)
    : QListView(parent)
{
    setItemDelegate(&mItemDelegate);
    setEditTriggers(QListView::CurrentChanged);
}

TrackListView::~TrackListView()
{

}

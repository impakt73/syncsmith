#include <ui/TrackListView.h>


TrackListView::TrackListView(QWidget *parent)
    : QListView(parent)
{
    setItemDelegate(&mItemDelegate);
    setEditTriggers(QListView::SelectedClicked);
}

TrackListView::~TrackListView()
{

}

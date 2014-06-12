#include <ui/TrackListView.h>
#include <QtGui>


TrackListView::TrackListView(QWidget *parent)
    : QListView(parent)
{
    setItemDelegate(&mItemDelegate);
    setEditTriggers(QListView::SelectedClicked);
}

TrackListView::~TrackListView()
{

}

void TrackListView::keyPressEvent(QKeyEvent *event)
{
    event->ignore();
}

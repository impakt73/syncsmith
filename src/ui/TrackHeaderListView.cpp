#include <ui/TrackHeaderListView.h>
#include <QScrollBar>


TrackHeaderListView::TrackHeaderListView(QWidget *parent)
    : QListView(parent)
{
    setItemDelegate(&mItemDelegate);
}

TrackHeaderListView::~TrackHeaderListView()
{

}

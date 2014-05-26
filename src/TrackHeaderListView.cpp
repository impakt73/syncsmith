#include <TrackHeaderListView.h>
#include <QScrollBar>


TrackHeaderListView::TrackHeaderListView(QWidget *parent)
    : QListView(parent)
{
    setViewportMargins(0, 0, 0, 13);
    setItemDelegate(&mItemDelegate);
}

TrackHeaderListView::~TrackHeaderListView()
{

}

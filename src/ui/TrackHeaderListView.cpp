#include <ui/TrackHeaderListView.h>
#include <QScrollBar>
#include <QtGui>
#include <ui/TrackListModel.h>


TrackHeaderListView::TrackHeaderListView(QWidget *parent)
    : QListView(parent)
{
    setItemDelegate(&mItemDelegate);
    //setEditTriggers(QListView::SelectedClicked);
}

TrackHeaderListView::~TrackHeaderListView()
{

}

void TrackHeaderListView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        unsigned int trackIndex = selectionModel()->currentIndex().row();
        TrackListModel* modelHandle = static_cast<TrackListModel*>(model());

        modelHandle->removeTrack(trackIndex);

        event->accept();
    }
    else
    {
        event->ignore();
    }
}

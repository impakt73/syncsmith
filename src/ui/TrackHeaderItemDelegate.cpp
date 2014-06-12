#include <ui/TrackHeaderItemDelegate.h>
#include <ui/TrackListModel.h>
#include <ui/TrackUtils.h>
#include <QtGui>

TrackHeaderItemDelegate::TrackHeaderItemDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{
}

TrackHeaderItemDelegate::~TrackHeaderItemDelegate()
{

}

void TrackHeaderItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const TrackListModel* model = static_cast<const TrackListModel*>(index.model());
    const Track* track = model->GetSyncContext()->GetTrack(index.row());

    TrackUtils::RenderTrackBackground(painter, option.rect, track);

    if(track->GetType() == kTrackType_Float)
    {
        TrackUtils::RenderFloatTrackHeader(painter, option.rect, track);
    }
}

QSize TrackHeaderItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(100, 100);
}

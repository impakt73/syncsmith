#include <ui/TrackItemDelegate.h>
#include <QtGui>
#include <ui/TrackListModel.h>
#include <ui/UIConstants.h>
#include <core/FloatTrack.h>
#include <ui/FloatTrackEditor.h>
#include <ui/TrackHandle.h>
#include <ui/TrackUtils.h>

TrackItemDelegate::TrackItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

TrackItemDelegate::~TrackItemDelegate()
{

}

void TrackItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const TrackListModel* model = static_cast<const TrackListModel*>(index.model());
    const Track* track = model->GetSyncContext()->GetTrack(index.row());

    TrackUtils::RenderTrackBackground(painter, option.rect, option.state & QStyle::State_Selected);

    if(track->GetType() == kTrackType_Float)
    {
        TrackUtils::RenderFloatTrack(painter, option.rect, track, false, QPoint());
    }
}

QSize TrackItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const TrackListModel* model = static_cast<const TrackListModel*>(index.model());
    return QSize(model->getAudioLength() * UIConstants::SecondSizeInPixels, 100);
}

QWidget *TrackItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.data(TrackListModel::TrackHandleRole).canConvert<TrackHandle>())
    {
        const TrackListModel* model = static_cast<const TrackListModel*>(index.model());
        TrackHandle trackHandle = index.data(TrackListModel::TrackHandleRole).value<TrackHandle>();
        if(trackHandle.track()->GetType() == kTrackType_Float)
        {
            // LOLOLOLOLOLOLOL Sorry.
            FloatTrackEditor* editor = new FloatTrackEditor(trackHandle.track(), const_cast<SyncClient*>(model->GetSyncClient()), parent);
            connect(editor, &FloatTrackEditor::editingFinished, this, &TrackItemDelegate::commitAndCloseEditor);
            return editor;
        }
    }
}

void TrackItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.data(TrackListModel::TrackHandleRole).canConvert<TrackHandle>())
    {
        TrackHandle trackHandle = index.data(TrackListModel::TrackHandleRole).value<TrackHandle>();
        if(trackHandle.track()->GetType() == kTrackType_Float)
        {
            FloatTrackEditor* editor = static_cast<FloatTrackEditor*>(editor);
            editor->setTrack(trackHandle.track());
        }
    }
}

void TrackItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}

void TrackItemDelegate::commitAndCloseEditor()
{

}

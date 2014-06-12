#include <ui/TrackListModel.h>
#include <SyncContext.h>
#include <core/FloatTrack.h>
#include <QtCore>
#include <ui/TrackHandle.h>

QVariant TrackListModel::data(const QModelIndex &index, int role) const
{
    unsigned int trackIndex = static_cast<unsigned int>(index.row());

    Track* track = mSyncContext->GetTrack(trackIndex);
    Q_ASSERT(track != nullptr);

    switch(role)
    {
        case Qt::DisplayRole:
        {
            return QVariant(QString(track->GetName().c_str()));
        }
        case TrackListModel::TrackHandleRole:
        {
            QVariant variant;
            variant.setValue(TrackHandle(track));
            return variant;
        }
    }

    return QVariant();
}

QFlags<Qt::ItemFlag> TrackListModel::flags(const QModelIndex &index) const
{
    return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
}

void TrackListModel::addFloatTrack()
{
    beginInsertRows(QModelIndex(), 0, 0);
    mSyncContext->AddTrack("Intro.CubeRoll", kTrackType_Float);
    endInsertRows();
}

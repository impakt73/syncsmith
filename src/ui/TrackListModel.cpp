#include <ui/TrackListModel.h>
#include <SyncContext.h>
#include <core/FloatTrack.h>
#include <QtCore>

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
        case TrackListModel::TypeRole:
        {
            return QVariant(track->GetType());
        }
    }

    return QVariant();
}

void TrackListModel::addFloatTrack()
{
    beginInsertRows(QModelIndex(), 0, 0);
    mSyncContext->AddTrack("Test Float Track", kTrackType_Float);
    endInsertRows();
}

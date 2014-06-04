#include <ui/TrackListModel.h>
#include <SyncContext.h>
#include <core/FloatTrack.h>

QVariant TrackListModel::data(const QModelIndex &index, int role) const
{
    unsigned int trackIndex = static_cast<unsigned int>(index.row());
    Q_ASSERT(trackIndex < mTracks.size());

    Track* track = mTracks[trackIndex];
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
        case TrackListModel::DataRole:
        {
            switch(track->GetType())
            {
                case kTrackType_Float:
                {
                    FloatTrack* floatTrack = static_cast<FloatTrack*>(track);
                    return QVariant(floatTrack->GetValue(mPosition));
                }
            }
        }
    }

    return QVariant();
}

void TrackListModel::addFloatTrack()
{
    beginInsertRows(QModelIndex(), 0, 0);
    endInsertRows();
}

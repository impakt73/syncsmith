#pragma once

#include <QAbstractListModel>
#include <core/Track.h>
#include <core/SyncContext.h>

class TrackListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TrackListModel(QObject *parent = 0)
        : QAbstractListModel(parent)
        , mAudioSamples(nullptr) {}
    ~TrackListModel() {}

    int rowCount(const QModelIndex &parent) const { return static_cast<int>(mSyncContext->GetTrackCount()); }
    QVariant data(const QModelIndex &index, int role) const;

    QFlags<Qt::ItemFlag> flags(const QModelIndex &index) const;

    void addTrack(const std::string& inTrackName, eTrackType inTrackType);
    void removeTrack(unsigned int inTrackIndex);
    void SetSyncContext(SyncContext* inSyncContext) { mSyncContext = inSyncContext; }
    SyncContext* GetSyncContext(void) { return mSyncContext; }
    const SyncContext* GetSyncContext(void) const { return mSyncContext; }

    void setAudioSamples(const std::vector<unsigned short>* inAudioSamples) { mAudioSamples = inAudioSamples; }
    void setMinSample(unsigned short inSample) { mMinSample = inSample; }
    void setMaxSample(unsigned short inSample) { mMaxSample = inSample; }
    void setAudioLength(double inSeconds) { beginResetModel(); mAudioLengthInSeconds = inSeconds; endResetModel(); }
    unsigned short getMinSample(void) const { return mMinSample; }
    unsigned short getMaxSample(void) const { return mMaxSample; }
    const std::vector<unsigned short>* getAudioSamples(void) const { return mAudioSamples; }
    double getAudioLength(void) const { return mAudioLengthInSeconds; }

    enum eTrackDataRole
    {
        TrackHandleRole = Qt::UserRole
    };

private:
    SyncContext* mSyncContext;
    const std::vector<unsigned short>* mAudioSamples;
    unsigned short mMinSample;
    unsigned short mMaxSample;
    double mAudioLengthInSeconds;
};

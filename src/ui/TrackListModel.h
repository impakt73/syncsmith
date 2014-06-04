#pragma once

#include <QAbstractListModel>
#include <core/Track.h>

class TrackListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TrackListModel(QObject *parent = 0)
        : QAbstractListModel(parent)
        , mPosition(0.0)
        , mAudioSamples(nullptr) {}
    ~TrackListModel() {}

    int rowCount(const QModelIndex &parent) const { return static_cast<int>(mTracks.size()); }
    QVariant data(const QModelIndex &index, int role) const;

    void addFloatTrack(void);

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
        TypeRole = Qt::UserRole,
        DataRole
    };

private:
    std::vector<Track*> mTracks;
    double mPosition;
    const std::vector<unsigned short>* mAudioSamples;
    unsigned short mMinSample;
    unsigned short mMaxSample;
    double mAudioLengthInSeconds;
};

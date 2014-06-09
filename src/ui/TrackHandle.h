#pragma once

#include <QtCore>

class TrackHandle
{
public:
    TrackHandle(void)
        : mTrack(nullptr)
    {
    }

    TrackHandle(const TrackHandle& inOther)
        : mTrack(inOther.mTrack)
    {
    }

    TrackHandle(Track* inTrack)
        : mTrack(inTrack)
    {
    }

    ~TrackHandle(void)
    {
    }

    inline Track* track(void) { return mTrack; }
    inline const Track* track(void) const { return mTrack; }

private:
    Track* mTrack;
};

Q_DECLARE_METATYPE(TrackHandle)

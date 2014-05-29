#pragma once

#include <map>
#include <core/Track.h>
#include <core/FloatTrack.h>

class SyncContext
{
public:
    SyncContext(void) : mPosition(0.0) {}
    ~SyncContext(void) {}

    template <typename T>
    T GetValue(const std::string& inTrackName); // Will cause compilation failure if the user asks for a non-specialized type

    inline void SetPosition(double inPosition) { mPosition = inPosition; }
    inline double GetPosition(void) const { return mPosition; }

private:
    Track* GetTrack(const std::string& inTrackName);

    std::map<std::string, Track*> mTracks;
    double mPosition;
};

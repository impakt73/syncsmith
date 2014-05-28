#pragma once

#include <map>
#include <Track.h>
#include <FloatTrack.h>

class SyncContext
{
public:
    SyncContext(void) {}
    ~SyncContext(void) {}

    const FloatTrack* GetFloatTrack(const std::string& inTrackName);

private:
    std::map<std::string, Track*> mTracks;
};

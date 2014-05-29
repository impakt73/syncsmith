#pragma once

#include <core/Track.h>
#include <vector>
#include <core/TrackKey.h>

class FloatTrack : public Track
{
public:
    FloatTrack(const std::string& inName)
        : Track(inName, kTrackType_Float) {}
    ~FloatTrack(void) {}

    float GetValue(double inPosition) const;

private:
    std::vector<TrackKey<float>> mKeys;
};

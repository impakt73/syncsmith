#pragma once

#include <core/Track.h>
#include <vector>
#include <core/TrackKey.h>

class FloatTrack : public Track
{
public:
    FloatTrack(const std::string& inName)
        : Track(inName, kTrackType_Float)
        , mMinValue(0.0f)
        , mMaxValue(100.0f) {}

    ~FloatTrack(void) {}

    float GetValue(double inPosition) const;

    inline TrackKey<float>& GetKey(unsigned int inIndex) { return mKeys[inIndex]; }
    inline const TrackKey<float>& GetKey(unsigned int inIndex) const { return mKeys[inIndex]; }

    inline unsigned int GetKeyCount(void) const { return mKeys.size(); }

    bool GetKeyIndex(double inPosition, unsigned int* outKeyIndex);

    void AddKey(double inPosition, float inValue);
    void RemoveKey(unsigned int inIndex);

    inline float GetMinValue(void) const { return mMinValue; }
    inline float GetMaxValue(void) const { return mMaxValue; }

private:
    std::vector<TrackKey<float>> mKeys;
    float mMinValue;
    float mMaxValue;
};

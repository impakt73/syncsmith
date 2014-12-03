#pragma once

#include <core/TrackInterpolationType.h>

template <class T>
class TrackKey
{
public:
    TrackKey(void) : mPosition(0.0), mData(0), mInterpolationType(kTrackInterpolationType_None) {}
    TrackKey(double inPosition, const T& inData)
        : mPosition(inPosition)
        , mData(inData)
        , mInterpolationType(kTrackInterpolationType_None) {}

    TrackKey(double inPosition, const T& inData, eTrackInterpolationType inInterpolationType)
        : mPosition(inPosition)
        , mData(inData)
        , mInterpolationType(inInterpolationType) {}

    inline double GetPosition(void) const { return mPosition; }
    inline T GetData(void) const { return mData; }
    inline void SetData(const T& inData) { mData = inData; }

    inline eTrackInterpolationType GetInterpolationType(void) const { return mInterpolationType; }
    inline void SetInterpolationType(eTrackInterpolationType inInterpolationType) { mInterpolationType = inInterpolationType; }

private:
    double mPosition;
    T mData;
    eTrackInterpolationType mInterpolationType;

};

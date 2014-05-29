#pragma once

template <class T>
class TrackKey
{
public:
    TrackKey(void) : mPosition(0.0) {}
    TrackKey(double inPosition, T inData)
        : mPosition(inPosition)
        , mData(inData) {}

    inline double GetPosition(void) const { return mPosition; }
    inline T GetData(void) const { return mData; }

private:
    double mPosition;
    T mData;
};

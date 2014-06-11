#pragma once

template <class T>
class TrackKey
{
public:
    TrackKey(void) : mPosition(0.0) {}
    TrackKey(double inPosition, const T& inData)
        : mPosition(inPosition)
        , mData(inData) {}

    inline double GetPosition(void) const { return mPosition; }
    inline T GetData(void) const { return mData; }
    inline void SetData(const T& inData) { mData = inData; }

private:
    double mPosition;
    T mData;
};

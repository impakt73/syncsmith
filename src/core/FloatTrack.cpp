#include <core/FloatTrack.h>

float FloatTrack::GetValue(double inPosition) const
{
    return 0.0f;
}

void FloatTrack::AddKey(double inPosition, float inValue)
{
    mKeys.push_back(TrackKey<float>(inPosition, inValue));
}

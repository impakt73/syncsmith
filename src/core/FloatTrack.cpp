#include <core/FloatTrack.h>

float FloatTrack::GetValue(double inPosition) const
{
    return 0.0f;
}

bool FloatTrack::GetKeyIndex(double inPosition, unsigned int* outKeyIndex)
{
    for(unsigned int keyIndex = 0; keyIndex < mKeys.size(); ++keyIndex)
    {
        if(mKeys[keyIndex].GetPosition() == inPosition)
        {
            *outKeyIndex = keyIndex;
            return true;
        }
    }

    return false;
}

void FloatTrack::AddKey(double inPosition, float inValue)
{
    mKeys.push_back(TrackKey<float>(inPosition, inValue));
}

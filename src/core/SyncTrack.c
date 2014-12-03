#include <core/SyncTrack.h>
#include <assert.h>
#include <math.h>

const float kDefaultFloatValue = 0.0f;
const int kDefaultIntegerValue = 0;
const bool kDefaultBooleanValue = false;
const char* kDefaultStringValue = "Default";

float InterpolateFloatKeys(const struct SyncTrackKey* inKeyA, const struct SyncTrackKey* inKeyB, unsigned int inPosition)
{
    switch(inKeyA->InterpolationType)
    {
        case kTrackInterpolationType_None:
        {
            return inKeyA->FloatData;
        }
        case kTrackInterpolationType_Linear:
        {
            unsigned int positionDifference = inKeyB->Position - inKeyA->Position;
            float interpolationValue = (float)(inPosition - inKeyA->Position) / (float)positionDifference;
            return (1.0f - interpolationValue) * inKeyA->FloatData + interpolationValue * inKeyB->FloatData;
        }
        case kTrackInterpolationType_Smoothstep:
        {
            unsigned int positionDifference = inKeyB->Position - inKeyA->Position;
            float interpolationValue = (float)(inPosition - inKeyA->Position) / (float)positionDifference;
            interpolationValue = interpolationValue * interpolationValue * (3.0 - 2.0 * interpolationValue);
            return (1.0f - interpolationValue) * inKeyA->FloatData + interpolationValue * inKeyB->FloatData;
        }
        case kTrackInterpolationType_Ramp:
        {
            unsigned int positionDifference = inKeyB->Position - inKeyA->Position;
            float interpolationValue = (float)(inPosition - inKeyA->Position) / (float)positionDifference;
            interpolationValue = interpolationValue * interpolationValue;
            return (1.0f - interpolationValue) * inKeyA->FloatData + interpolationValue * inKeyB->FloatData;
        }
        case kTrackInterpolationType_Cosine:
        {
            unsigned int positionDifference = inKeyB->Position - inKeyA->Position;
            float interpolationValue = (float)(inPosition - inKeyA->Position) / (float)positionDifference;
            interpolationValue = (1.0f - (float)cos((double)(interpolationValue * 3.14159265f))) * 0.5f;
            return (1.0f - interpolationValue) * inKeyA->FloatData + interpolationValue * inKeyB->FloatData;
        }
        default:
        {
            // Return the default value if we have an invalid interpolation mode
            return kDefaultFloatValue;
        }
    }
}

float SyncTrackGetFloat(const struct SyncTrack* inSyncTrack, unsigned int inPosition)
{
    assert(inSyncTrack->Type == kTrackType_Float);

    if(inSyncTrack->NumOfKeys == 0)
    {
        // Return the default value since there's no keys
        return kDefaultFloatValue;
    }
    else if(inSyncTrack->NumOfKeys == 1)
    {
        // Don't even bother checking position here, just return the only value we have
        struct SyncTrackKey* trackKey = &inSyncTrack->Keys[0];
        assert(trackKey->ParentType == kTrackType_Float);

        return trackKey->FloatData;
    }
    else
    {
        // Binary search for two closest keys here since we're sure we have atleast 2 keys
        // @TODO

        struct SyncTrackKey* keyA = 0;
        struct SyncTrackKey* keyB = 0;
        // Return the interpolated value of the two keys based on the position
        return InterpolateFloatKeys(keyA, keyB, inPosition);
    }
}

int SyncTrackGetInteger(const struct SyncTrack* inSyncTrack, unsigned int inPosition)
{
    assert(inSyncTrack->Type == kTrackType_Integer);

    return kDefaultIntegerValue;
}

bool SyncTrackGetBoolean(const struct SyncTrack* inSyncTrack, unsigned int inPosition)
{
    assert(inSyncTrack->Type == kTrackType_Boolean);

    return kDefaultBooleanValue;
}

const char* SyncTrackGetString(const struct SyncTrack* inSyncTrack, unsigned int inPosition)
{
    assert(inSyncTrack->Type == kTrackType_String);

    return kDefaultStringValue;
}

#include <core/SyncTrack.h>
#include <assert.h>
#include <math.h>

const float kDefaultFloatValue = 0.0f;
const int kDefaultIntegerValue = 0;
const bool kDefaultBooleanValue = false;
const char* kDefaultStringValue = "Default";

unsigned int SyncTrackFindKeys(const struct SyncTrack* inSyncTrack, unsigned int inPosition, struct SyncTrackKey outTrackKeys[2]);

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

    struct SyncTrackKey trackKeys[2];
    unsigned int keyCount = SyncTrackFindKeys(inSyncTrack, inPosition, trackKeys);
    switch(keyCount)
    {
        case 0:
        {
            // No keys in track
            return kDefaultFloatValue;
        }
        case 1:
        {
            // Single key value
            return trackKeys[0].FloatData;
        }
        case 2:
        {
            // Interpolated key value
            return InterpolateFloatKeys(&trackKeys[0], &trackKeys[1], inPosition);
        }
    }

    // This should never happen...
    assert(false);
    return kDefaultFloatValue;
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

unsigned int SyncTrackFindKeys(const struct SyncTrack* inSyncTrack, unsigned int inPosition, struct SyncTrackKey outTrackKeys[2])
{
    if(inSyncTrack->NumOfKeys == 0)
    {
        return 0;
    }
    else if(inSyncTrack->NumOfKeys == 1)
    {
        outTrackKeys[0] = inSyncTrack->Keys[0];
        return 1;
    }
    else
    {
        // Binary search for two closest keys here since we're sure we have at least 2 keys

        unsigned int minIndex = 0;
        unsigned int maxIndex = inSyncTrack->NumOfKeys - 1;

        while(minIndex < maxIndex)
        {
            unsigned int midIndex = (minIndex + maxIndex) / 2;
            assert(midIndex < maxIndex);

            if(inSyncTrack->Keys[midIndex].Position < inPosition)
            {
                minIndex = midIndex + 1;
            }
            else
            {
                maxIndex = midIndex;
            }
        }

        // Assume we must have finished iteration if we've exited the loop
        assert(minIndex == maxIndex);

        unsigned int keyIndex = minIndex;
        const struct SyncTrackKey* key = &inSyncTrack->Keys[keyIndex];

        if(key->Position == inPosition)
        {
            // Found a direct match, simply return the key
            outTrackKeys[0] = inSyncTrack->Keys[keyIndex];
            return 1;
        }
        else
        {
            // Direct match not found, figure out what to do based on the surrounding keys
            int direction = (key->Position - inPosition) > 0 ? 1 : -1; // We know this will never be zero since the case above wasn't taken
            int secondKeyIndex = keyIndex + direction;
            if(secondKeyIndex >= 0 && secondKeyIndex < inSyncTrack->NumOfKeys)
            {
                // We have a valid secondary key, return both

                unsigned int minKeyIndex = keyIndex > secondKeyIndex ? secondKeyIndex : keyIndex;
                unsigned int maxKeyIndex = keyIndex > secondKeyIndex ? keyIndex : secondKeyIndex;

                outTrackKeys[0] = inSyncTrack->Keys[minKeyIndex];
                outTrackKeys[1] = inSyncTrack->Keys[maxKeyIndex];
                return 2;
            }
            else
            {
                // Our secondary key falls outside of the array, return the first key
                outTrackKeys[0] = inSyncTrack->Keys[keyIndex];
                return 1;
            }
        }
    }
}

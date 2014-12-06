#include <core/SyncTrack.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

const float kDefaultFloatValue = 0.0f;
const int kDefaultIntegerValue = 0;
const bool kDefaultBooleanValue = false;


////////////////////////////////////////////////////////////////////////////////////////
// Private Key Functions
////////////////////////////////////////////////////////////////////////////////////////

unsigned int SyncTrackFindKeys(const struct SyncTrack* inSyncTrack, unsigned int inPosition, struct SyncTrackKey outTrackKeys[2], unsigned int outTrackKeyIndices[2]);
void SyncTrackResizeKeys(struct SyncTrack* inSyncTrack, unsigned int inNewNumOfKeys);
void SyncTrackAppendKey(struct SyncTrack* inSyncTrack, const struct SyncTrackKey* inKey);
void SyncTrackInsertKey(struct SyncTrack* inSyncTrack, const struct SyncTrackKey* inKey, unsigned int inIndex);

unsigned int SyncTrackFindKeys(const struct SyncTrack* inSyncTrack, unsigned int inPosition, struct SyncTrackKey outTrackKeys[2], unsigned int outTrackKeyIndices[2])
{
    if(inSyncTrack->NumOfKeys == 0)
    {
        return 0;
    }
    else if(inSyncTrack->NumOfKeys == 1)
    {
        outTrackKeys[0] = inSyncTrack->Keys[0];
        outTrackKeyIndices[0] = 0;
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
            outTrackKeyIndices[0] = keyIndex;
            return 1;
        }
        else
        {
            // Direct match not found, figure out what to do based on the surrounding keys
            int direction = (key->Position < inPosition) ? 1 : -1; // We know these will never be equal since the case above wasn't taken
            int secondKeyIndex = keyIndex + direction;
            if(secondKeyIndex >= 0 && secondKeyIndex < inSyncTrack->NumOfKeys)
            {
                // We have a valid secondary key, return both

                unsigned int minKeyIndex = keyIndex > secondKeyIndex ? secondKeyIndex : keyIndex;
                unsigned int maxKeyIndex = keyIndex > secondKeyIndex ? keyIndex : secondKeyIndex;

                outTrackKeys[0] = inSyncTrack->Keys[minKeyIndex];
                outTrackKeys[1] = inSyncTrack->Keys[maxKeyIndex];
                outTrackKeyIndices[0] = minKeyIndex;
                outTrackKeyIndices[1] = maxKeyIndex;
                return 2;
            }
            else
            {
                // Our secondary key falls outside of the array, return the first key
                outTrackKeys[0] = inSyncTrack->Keys[keyIndex];
                outTrackKeyIndices[0] = keyIndex;
                return 1;
            }
        }
    }
}

void SyncTrackResizeKeys(struct SyncTrack* inSyncTrack, unsigned int inNewNumOfKeys)
{
    inSyncTrack->Keys = realloc(inSyncTrack->Keys, sizeof(struct SyncTrackKey) * inNewNumOfKeys);
    inSyncTrack->NumOfKeys = inNewNumOfKeys;

    /*
    if(inNewNumOfKeys == 0)
    {
        // We're removing all keys so destroy the array
        inSyncTrack->NumOfKeys = 0;
        free(inSyncTrack->Keys);
    }
    else
    {
        inSyncTrack->NumOfKeys = inNewNumOfKeys;

        // We're resizing to a non-zero amount
        if(inSyncTrack->NumOfKeys > 0)
        {
            // We had keys and an array previously
            inSyncTrack->Keys = realloc(inSyncTrack->Keys, sizeof(struct SyncTrackKey) * inNewNumOfKeys);
        }
        else
        {
            // We have no keys so create a new array
            inSyncTrack->Keys = malloc(sizeof(struct SyncTrackKey) * inNewNumOfKeys);
        }
    }
    */
}

void SyncTrackAppendKey(struct SyncTrack* inSyncTrack, const struct SyncTrackKey* inKey)
{
    // Make space for the new key and copy it in
    SyncTrackResizeKeys(inSyncTrack, inSyncTrack->NumOfKeys + 1);
    inSyncTrack->Keys[inSyncTrack->NumOfKeys - 1] = *inKey;
}

void SyncTrackInsertKey(struct SyncTrack* inSyncTrack, const struct SyncTrackKey* inKey, unsigned int inIndex)
{
    assert(inIndex < inSyncTrack->NumOfKeys);

    if(inIndex == 0)
    {
        // Add space for another key to the end of the array
        SyncTrackResizeKeys(inSyncTrack, inSyncTrack->NumOfKeys + 1);

        // Move all keys after the index over one space
        for(int keyIndex = inSyncTrack->NumOfKeys - 1; keyIndex > 0; --keyIndex)
        {
            inSyncTrack->Keys[keyIndex] = inSyncTrack->Keys[keyIndex - 1];
        }

        // Copy the new key in
        inSyncTrack->Keys[0] = *inKey;
    }
    else
    {
        // 0 1 2 (3)

        // Add space for another key to the end of the array
        SyncTrackResizeKeys(inSyncTrack, inSyncTrack->NumOfKeys + 1);

        // Calculate the key location
        void* keyLocation = inSyncTrack->Keys + (inIndex * sizeof(struct SyncTrackKey));

        // Move all keys after the index over one space
        memmove(keyLocation + sizeof(struct SyncTrackKey), keyLocation, sizeof(struct SyncTrackKey) * ((inSyncTrack->NumOfKeys - 1) - inIndex));

        // Copy the new key in
        inSyncTrack->Keys[inIndex] = *inKey;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
// Public Track Functions
////////////////////////////////////////////////////////////////////////////////////////

struct SyncTrack* SyncTrackCreateTrack(const char* inName, enum eTrackType inType)
{
    struct SyncTrack* syncTrack = (struct SyncTrack*)malloc(sizeof(struct SyncTrack));
    memset(syncTrack, 0, sizeof(struct SyncTrack));
    char* nameBuffer = (char*)malloc(strlen(inName) + 1);
    strcpy(nameBuffer, inName);
    syncTrack->Name = nameBuffer;
    syncTrack->Type = inType;

    return syncTrack;
}

void SyncTrackDestroyTrack(struct SyncTrack** inSyncTrack)
{
    struct SyncTrack* syncTrack = *inSyncTrack;
    free((void*)syncTrack->Name);
    free(syncTrack->Keys);
    free(syncTrack);
    *inSyncTrack = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////
// Public Key Functions
////////////////////////////////////////////////////////////////////////////////////////

void SyncTrackUpdateKey(struct SyncTrack* inSyncTrack, unsigned int inPosition, enum eTrackInterpolationType inInterpolationType, const struct SyncTrackKeyValue* inValue)
{
    // Create a new key with the updated information
    struct SyncTrackKey newKey;
    newKey.Position = inPosition;
    newKey.InterpolationType = inInterpolationType;
    newKey.Value = *inValue;

    struct SyncTrackKey trackKeys[2];
    unsigned int trackKeyIndices[2];
    unsigned int keyCount = SyncTrackFindKeys(inSyncTrack, inPosition, trackKeys, trackKeyIndices);

    switch(keyCount)
    {
        case 0:
        {
            // No keys in track so append the new key to the end
            SyncTrackAppendKey(inSyncTrack, &newKey);
            break;
        }
        case 1:
        {
            // We're either on an edge of the array or we've found an existing key
            unsigned int keyIndex = trackKeyIndices[0];
            if(inSyncTrack->Keys[keyIndex].Position == inPosition)
            {
                // We've found an exact match
                // Copy the new key values over the old ones
                inSyncTrack->Keys[keyIndex] = newKey;
            }
            else
            {
                if(inSyncTrack->NumOfKeys > 1)
                {
                    // We're on an edge of the array since we only got one key back and we have multiple keys
                    if(keyIndex == 0)
                    {
                        // We're at the beginning so insert
                        SyncTrackInsertKey(inSyncTrack, &newKey, keyIndex);
                    }
                    else
                    {
                        // We're at the end so append
                        SyncTrackAppendKey(inSyncTrack, &newKey);
                    }
                }
                else
                {
                    // We only have one key in the array so figure out how to add the new one
                    if(inSyncTrack->Keys[keyIndex].Position > inPosition)
                    {
                        // Our key is smaller so insert
                        SyncTrackInsertKey(inSyncTrack, &newKey, keyIndex);
                    }
                    else
                    {
                        // Our key is larger so append
                        SyncTrackAppendKey(inSyncTrack, &newKey);
                    }
                }
            }

            break;
        }
        case 2:
        {
            // Key doesn't exist yet but it needs to be inserted between existing keys
            SyncTrackInsertKey(inSyncTrack, &newKey, trackKeyIndices[0] + 1);
            break;
        }
    }
}

void SyncTrackRemoveKey(struct SyncTrack* inSyncTrack, unsigned int inPosition)
{
    assert(inSyncTrack->NumOfKeys > 0);

    struct SyncTrackKey trackKeys[2];
    unsigned int trackKeyIndices[2];
    unsigned int keyCount = SyncTrackFindKeys(inSyncTrack, inPosition, trackKeys, trackKeyIndices);
    assert(keyCount == 1);

    // If we have more than one key currently, we need to move the array down before resizing
    if(inSyncTrack->NumOfKeys > 1)
    {
        if(trackKeyIndices[0] == 0)
        {
            // This is the first key in the array
            // Just move the remaining part of the array ontop of this one
            for(int keyIndex = 0; keyIndex < inSyncTrack->NumOfKeys - 1; ++keyIndex)
            {
                inSyncTrack->Keys[keyIndex] = inSyncTrack->Keys[keyIndex + 1];
            }
        }
        else if(trackKeyIndices[0] != inSyncTrack->NumOfKeys - 1)
        {
            // This key is between two other keys
            // Move all keys after the index over one space to the left
            size_t keyOffset = sizeof(struct SyncTrackKey) * trackKeyIndices[0];
            size_t arraySize = sizeof(struct SyncTrackKey) * (inSyncTrack->NumOfKeys - (trackKeyIndices[0] + 1));
            memmove(inSyncTrack->Keys + keyOffset, inSyncTrack->Keys + keyOffset + sizeof(struct SyncTrackKey), arraySize);
        }

        // If it's the last key in the array, we really don't have to do anything special
    }

    SyncTrackResizeKeys(inSyncTrack, inSyncTrack->NumOfKeys - 1);
}

////////////////////////////////////////////////////////////////////////////////////////
// Float Key Functions
////////////////////////////////////////////////////////////////////////////////////////

float InterpolateFloatKeys(const struct SyncTrackKey* inKeyA, const struct SyncTrackKey* inKeyB, unsigned int inPosition)
{
    switch(inKeyA->InterpolationType)
    {
        case kTrackInterpolationType_None:
        {
            return inKeyA->Value.FloatData;
        }
        case kTrackInterpolationType_Linear:
        {
            unsigned int positionDifference = inKeyB->Position - inKeyA->Position;
            float interpolationValue = (float)(inPosition - inKeyA->Position) / (float)positionDifference;
            return (1.0f - interpolationValue) * inKeyA->Value.FloatData + interpolationValue * inKeyB->Value.FloatData;
        }
        case kTrackInterpolationType_Smoothstep:
        {
            unsigned int positionDifference = inKeyB->Position - inKeyA->Position;
            float interpolationValue = (float)(inPosition - inKeyA->Position) / (float)positionDifference;
            interpolationValue = interpolationValue * interpolationValue * (3.0 - 2.0 * interpolationValue);
            return (1.0f - interpolationValue) * inKeyA->Value.FloatData + interpolationValue * inKeyB->Value.FloatData;
        }
        case kTrackInterpolationType_Ramp:
        {
            unsigned int positionDifference = inKeyB->Position - inKeyA->Position;
            float interpolationValue = (float)(inPosition - inKeyA->Position) / (float)positionDifference;
            interpolationValue = interpolationValue * interpolationValue;
            return (1.0f - interpolationValue) * inKeyA->Value.FloatData + interpolationValue * inKeyB->Value.FloatData;
        }
        case kTrackInterpolationType_Cosine:
        {
            unsigned int positionDifference = inKeyB->Position - inKeyA->Position;
            float interpolationValue = (float)(inPosition - inKeyA->Position) / (float)positionDifference;
            interpolationValue = (1.0f - (float)cos((double)(interpolationValue * 3.14159265f))) * 0.5f;
            return (1.0f - interpolationValue) * inKeyA->Value.FloatData + interpolationValue * inKeyB->Value.FloatData;
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
    unsigned int trackKeyIndices[2];
    unsigned int keyCount = SyncTrackFindKeys(inSyncTrack, inPosition, trackKeys, trackKeyIndices);
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
            return trackKeys[0].Value.FloatData;
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

////////////////////////////////////////////////////////////////////////////////////////
// Integer Key Functions
////////////////////////////////////////////////////////////////////////////////////////

int SyncTrackGetInteger(const struct SyncTrack* inSyncTrack, unsigned int inPosition)
{
    assert(inSyncTrack->Type == kTrackType_Integer);

    return kDefaultIntegerValue;
}

////////////////////////////////////////////////////////////////////////////////////////
// Boolean Key Functions
////////////////////////////////////////////////////////////////////////////////////////

bool SyncTrackGetBoolean(const struct SyncTrack* inSyncTrack, unsigned int inPosition)
{
    assert(inSyncTrack->Type == kTrackType_Boolean);

    return kDefaultBooleanValue;
}

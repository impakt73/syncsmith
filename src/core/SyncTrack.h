#pragma once

#include <stdbool.h>
#include <core/TrackType.h>
#include <core/TrackInterpolationType.h>

struct SyncTrackKey
{
    unsigned int Position;
    enum eTrackType ParentType;
    enum eTrackInterpolationType InterpolationType;
    union
    {
        float FloatData;
        int IntegerData;
        bool BooleanData;
        const char* StringData;
    };
};

struct SyncTrack
{
    enum eTrackType Type;
    unsigned int NumOfKeys;
    struct SyncTrackKey* Keys;
};

float SyncTrackGetFloat(const struct SyncTrack* inSyncTrack, unsigned int inPosition);
int SyncTrackGetInteger(const struct SyncTrack* inSyncTrack, unsigned int inPosition);
bool SyncTrackGetBoolean(const struct SyncTrack* inSyncTrack, unsigned int inPosition);
const char* SyncTrackGetString(const struct SyncTrack* inSyncTrack, unsigned int inPosition);

#pragma once

#include <stdbool.h>
#include <core/TrackInterpolationType.h>

struct SyncTrackKeyValue
{
    union
    {
        float FloatData;
        int IntegerData;
        bool BooleanData;
    };
};

struct SyncTrackKey
{
    unsigned int Position;
    enum eTrackInterpolationType InterpolationType;
    struct SyncTrackKeyValue Value;
};

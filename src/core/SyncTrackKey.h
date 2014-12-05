#pragma once

#include <stdbool.h>
#include <core/TrackInterpolationType.h>

struct SyncTrackKey
{
    unsigned int Position;
    enum eTrackInterpolationType InterpolationType;
    union
    {
        float FloatData;
        int IntegerData;
        bool BooleanData;
        const char* StringData;
    };
};

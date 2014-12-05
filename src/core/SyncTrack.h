#pragma once

#include <core/TrackType.h>
#include <core/SyncTrackKey.h>

struct SyncTrack
{
    enum eTrackType Type;
    unsigned int NumOfKeys;
    struct SyncTrackKey* Keys;
};

////////////////////////////////////////////////////////////////////////////////////////
// General Track Functions
////////////////////////////////////////////////////////////////////////////////////////

// Update an existing key or create a new one if it doesn't exist
void SyncTrackUpdateKey(struct SyncTrack* inSyncTrack, const struct SyncTrackKey* inKey);

// Remove an existing key
void SyncTrackRemoveKey(struct SyncTrack* inSyncTrack, unsigned int inPosition);

////////////////////////////////////////////////////////////////////////////////////////
// Typed Track Functions
////////////////////////////////////////////////////////////////////////////////////////

float SyncTrackGetFloat(const struct SyncTrack* inSyncTrack, unsigned int inPosition);
int SyncTrackGetInteger(const struct SyncTrack* inSyncTrack, unsigned int inPosition);
bool SyncTrackGetBoolean(const struct SyncTrack* inSyncTrack, unsigned int inPosition);

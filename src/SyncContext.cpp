#include <SyncContext.h>

const FloatTrack* SyncContext::GetFloatTrack(const std::string& inTrackName)
{
    std::map<std::string, Track*>::const_iterator trackIterator = mTracks.find(inTrackName);
    if(trackIterator != mTracks.end())
    {
        if(trackIterator->second->GetType() == kTrackType_Float)
        {
            return static_cast<FloatTrack*>(trackIterator->second);
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        FloatTrack* floatTrack = new FloatTrack(inTrackName);
        mTracks[inTrackName] = floatTrack;

        // Make sure the server reflects this change.

        return floatTrack;
    }
}

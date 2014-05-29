#include <core/SyncContext.h>

template <>
float SyncContext::GetValue<float>(const std::string& inTrackName)
{
    Track* track = GetTrack(inTrackName);
    if(track != nullptr && track->GetType() == kTrackType_Float)
    {
        return static_cast<FloatTrack*>(track)->GetValue(mPosition);
    }
    else
    {
        return 0.0f;
    }
}

Track* SyncContext::GetTrack(const std::string& inTrackName)
{
    std::map<std::string, Track*>::const_iterator trackIterator = mTracks.find(inTrackName);
    if(trackIterator != mTracks.end())
    {
        return trackIterator->second;
    }
    else
    {
        return nullptr;
    }
}


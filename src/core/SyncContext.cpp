#include <SyncContext.h>
#include <core/Track.h>
#include <core/FloatTrack.h>
#include <QtMath>

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

template <>
bool SyncContext::GetValue<bool>(const std::string& inTrackName)
{
    return false;
}

template <>
std::string SyncContext::GetValue<std::string>(const std::string& inTrackName)
{
    return "Undefined";
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

Track* SyncContext::GetTrack(unsigned int inTrackIndex)
{
    if(inTrackIndex >= mTrackList.size())
        return nullptr;

    return mTrackList[inTrackIndex];
}

const Track* SyncContext::GetTrack(const std::string& inTrackName) const
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

const Track* SyncContext::GetTrack(unsigned int inTrackIndex) const
{
    if(inTrackIndex >= mTrackList.size())
        return nullptr;

    return mTrackList[inTrackIndex];
}

Track* SyncContext::AddTrack(const std::string& inTrackName, eTrackType inTrackType)
{
    std::map<std::string, Track*>::const_iterator trackIterator = mTracks.find(inTrackName);
    if(trackIterator == mTracks.end())
    {
        switch(inTrackType)
        {
            case kTrackType_Float:
            {
                FloatTrack* floatTrack = new FloatTrack(inTrackName);
                mTracks[inTrackName] = floatTrack;
                mTrackList.push_back(floatTrack);
                break;
            }
        }
    }

    return nullptr;
}

void SyncContext::RemoveTrack(unsigned int inTrackIndex)
{
    std::vector<Track*>::iterator trackIterator = mTrackList.begin() + inTrackIndex;
    if(trackIterator != mTrackList.end())
    {
        std::map<std::string, Track*>::iterator mapTrackIterator = mTracks.find((*trackIterator)->GetName());
        mTrackList.erase(trackIterator);
        mTracks.erase(mapTrackIterator);
    }
}



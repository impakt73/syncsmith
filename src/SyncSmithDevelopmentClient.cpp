#include <SyncSmithDevelopmentClient.h>
#include <QtDebug>

bool SyncSmithDevelopmentClient::ConnectToServer(const std::string& inAddress, int inPort)
{
    return false;
}

void SyncSmithDevelopmentClient::Disconnect()
{

}

const FloatTrack* SyncSmithDevelopmentClient::GetFloatTrack(const std::string& inTrackName)
{
    std::map<std::string, Track*>::const_iterator trackIterator = mTracks.find(inTrackName);
    if(trackIterator != mTracks.end())
    {
        Q_ASSERT(trackIterator->second->GetType() == kTrackType_Float);

        return static_cast<FloatTrack*>(trackIterator->second);
    }
    else
    {
        FloatTrack* floatTrack = new FloatTrack(inTrackName);
        mTracks[inTrackName] = floatTrack;

        // Make sure the server reflects this change.

        return floatTrack;
    }
}

#pragma once

#include <map>
#include <Track.h>
#include <FloatTrack.h>

class SyncSmithDevelopmentClient
{
public:
    SyncSmithDevelopmentClient(void) {}
    ~SyncSmithDevelopmentClient(void) {}

    bool ConnectToServer(const std::string& inAddress, int inPort);
    void Disconnect(void);

    const FloatTrack* GetFloatTrack(const std::string& inTrackName);

    inline bool IsConnected(void) const { return mIsConnected; }

private:
    bool mIsConnected;
    std::string mAddress;
    int mPort;
    std::map<std::string, Track*> mTracks;
};

#pragma once

#include <map>
#include <vector>
#include <core/TrackType.h>

class Track;

class SyncContext
{
public:
    SyncContext(void) : mPosition(0.0) {}
    ~SyncContext(void) {}

    template <typename T>
    T GetValue(const std::string& inTrackName); // Will cause compilation failure if the user asks for a non-specialized type

    inline void SetPosition(double inPosition) { mPosition = inPosition; }
    inline double GetPosition(void) const { return mPosition; }

    inline unsigned int GetTrackCount(void) const { return mTrackList.size(); }

    Track* GetTrack(const std::string& inTrackName);
    Track* GetTrack(unsigned int inTrackIndex);

    const Track* GetTrack(const std::string& inTrackName) const;
    const Track* GetTrack(unsigned int inTrackIndex) const;

    Track* AddTrack(const std::string& inTrackName, eTrackType inTrackType);
    void RemoveTrack(const std::string& inTrackName);
    void RemoveTrack(unsigned int inTrackIndex);

private:
    std::map<std::string, Track*> mTracks;
    std::vector<Track*> mTrackList;
    double mPosition;
};

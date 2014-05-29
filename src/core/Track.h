#pragma once

#include <core/TrackType.h>
#include <string>

class Track
{
public:
    virtual ~Track(void) {}

    inline const std::string& GetName(void) const { return mName; }
    inline void SetName(const std::string& inName) { mName = inName; }
    inline eTrackType GetType(void) const { return mType; }

protected:
    Track(const std::string& inName, eTrackType inTrackType)
        : mName(inName)
        , mType(inTrackType) {}

    std::string mName;
    eTrackType mType;
};

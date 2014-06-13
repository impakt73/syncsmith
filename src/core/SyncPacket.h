#pragma once

#include <QtCore>
#include <core/TrackType.h>

enum eSyncPacketType
{
    kSyncPacketType_Undefined = 0,
    kSyncPacketType_Handshake,
    kSyncPacketType_AddTrack,
    kSyncPacketType_RenameTrack,
    kSyncPacketType_RemoveTrack,
    kSyncPacketType_AddKey,
    kSyncPacketType_ModifyKey,
    kSyncPacketType_RemoveKey,
    kSyncPacketType_Seek,
    kSyncPacketType_Play,
    kSyncPacketType_Pause,

    kSyncPacketType_Response
};

class SyncPacket
{
public:
    eSyncPacketType Type;
};

class SyncPacket_Handshake : public SyncPacket
{
public:
    QString Message;
};

class SyncPacket_AddTrack : public SyncPacket
{
public:
    QString TrackName;
    eTrackType TrackType;
};

class SyncPacket_RenameTrack : public SyncPacket
{
public:
    QString TrackName;
    QString NewTrackName;
};

class SyncPacket_RemoveTrack : public SyncPacket
{
public:
    QString TrackName;
};

class SyncPacket_AddKey : public SyncPacket
{
public:
    QString TrackName;
    eTrackType TrackType;
};

class SyncPacket_Seek : public SyncPacket
{
public:
    qreal Position;
};

class SyncPacket_Play : public SyncPacket
{
};

class SyncPacket_Pause : public SyncPacket
{
};

class SyncPacket_Response : public SyncPacket
{
public:
    bool Result;
};



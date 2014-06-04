#pragma once

// How to send track data?

enum eSyncCommand
{
    kSyncCommand_Undefined = 0,
    kSyncCommand_Handshake,
    kSyncCommand_AddTrack,
    kSyncCommand_RenameTrack,
    kSyncCommand_RemoveTrack,
    kSyncCommand_AddKey,
    kSyncCommand_ModifyKey,
    kSyncCommand_RemoveKey
};

struct SyncPacket
{
    eSyncCommand Command;
    void* Data;
};

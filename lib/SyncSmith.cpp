#include <SyncSmith.h>
#include <core/SyncServer.h>

ISyncSmithServer* CreateSyncSmith(void)
{
    SyncServer* syncServer = new SyncServer();
    return syncServer;
}

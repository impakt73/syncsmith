#pragma once

#include <SyncContext.h>

class ISyncSmithServer
{
public:
    virtual SyncContext* GetSyncContext(void) = 0;
    virtual void Release(void) = 0;

protected:
    ISyncSmithServer(void) {}
    virtual ~ISyncSmithServer(void) {}
};

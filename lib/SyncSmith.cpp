#include <SyncSmith.h>
#include <core/SyncServer.h>
#include <QCoreApplication>

static QCoreApplication* sApplication = nullptr;
static SyncServer* sSyncServer = nullptr;
static int sArgc;
static char** sArgv;

void ssCreate(int argc, char** argv)
{
    sArgc = argc;
    sArgv = argv;

    sApplication = new QCoreApplication(sArgc, sArgv);
    sSyncServer = new SyncServer();
}


void ssUpdate()
{
    if(sApplication != nullptr)
    {
        sApplication->processEvents();
    }
}


void ssDestroy()
{
    if(sSyncServer != nullptr)
    {
        delete sSyncServer;
        sSyncServer = nullptr;
    }

    if(sApplication != nullptr)
    {
        delete sApplication;
        sApplication = nullptr;
    }
}


int ssStartServer(int inPort)
{
    if(sSyncServer != nullptr)
    {
        return static_cast<int>(sSyncServer->Start(inPort));
    }

    return 0;
}


void ssStopServer()
{
    if(sSyncServer != nullptr)
    {
        sSyncServer->Stop();
    }
}


float ssGetFloat(const char *inTrackName)
{
    if(sSyncServer != nullptr)
    {
        // Get float
    }

    return 0.0f;
}


void ssSetSeekCallback(void (*inCallback)(double))
{
    if(sSyncServer != nullptr)
    {
        sSyncServer->SetSeekCallback(inCallback);
    }
}

#pragma once

#include <ISyncSmithServer.h>

#pragma once

// DLL Binding

#if defined(SYNCSMITH_EXPORTS)

#if defined(_MSC_VER)
    #define SYNCSMITH_API   __declspec(dllexport)
#else
    #define SYNCSMITH_API   __attribute__((visibility("default")))
#endif

#else

#if defined(_MSC_VER)
    #define SYNCSMITH_API   __declspec(dllimport)
#else
    #define SYNCSMITH_API
#endif

#endif


extern "C" SYNCSMITH_API ISyncSmithServer* CreateSyncSmith(void);

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


extern "C" SYNCSMITH_API void ssCreate(int argc, char** argv);
extern "C" SYNCSMITH_API void ssUpdate(void);
extern "C" SYNCSMITH_API void ssDestroy(void);
extern "C" SYNCSMITH_API int ssStartServer(int inPort);
extern "C" SYNCSMITH_API void ssStopServer(void);
extern "C" SYNCSMITH_API void ssSetSeekCallback(void (*inCallback)(double));
extern "C" SYNCSMITH_API float ssGetFloat(const char* inTrackName);


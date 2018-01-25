#ifndef __PPROFGO_H__
#define __PPROFGO_H__

#ifdef _WIN32
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

    DLLIMPORT void HeapProfilerStart(const char *prefix);
    DLLIMPORT void HeapProfilerDump(const char *reason);
    DLLIMPORT void HeapProfilerStop();

#ifdef __cplusplus
}
#endif

#endif

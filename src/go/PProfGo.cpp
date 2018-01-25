#include "PProfGo.h"
#include "../common/profiler.h"

#ifdef __cplusplus
extern "C" {
#endif

    DLLIMPORT void HeapProfilerStart(const char *prefix) {
        Profiler::HeapProfilerStart(prefix);
    }

    DLLIMPORT void HeapProfilerDump(const char *reason) {
        Profiler::HeapProfilerDump(reason);
    }

    DLLIMPORT void HeapProfilerStop() {
        Profiler::HeapProfilerStop();
    }

#ifdef __cplusplus
}
#endif
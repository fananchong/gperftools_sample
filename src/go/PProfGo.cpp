#include "PProfGo.h"
#include "../common/profiler.h"

#ifdef __cplusplus
extern "C" {
#endif

    DLLIMPORT void _HeapProfilerStart(const char *prefix) {
        Profiler::HeapProfilerStart(prefix);
    }

    DLLIMPORT void _HeapProfilerDump(const char *reason) {
        Profiler::HeapProfilerDump(reason);
    }

    DLLIMPORT void _HeapProfilerStop() {
        Profiler::HeapProfilerStop();
    }

#ifdef __cplusplus
}
#endif

#include <iostream>
#include "common/profiler.h"

int main(int argn, char *argv[]) {

    Profiler::HeapProfilerStart("test1.pprof");

    auto a = new char[1024*1024];

    Profiler::HeapProfilerDump("dump1");

    delete [] a;


    auto b = new char[1024*1024*2];

    Profiler::HeapProfilerDump("dump2");
    
    delete [] b;
    
    Profiler::HeapProfilerDump("dump3");

    Profiler::HeapProfilerStop();

    return 0;
}

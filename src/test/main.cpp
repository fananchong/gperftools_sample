#include <iostream>
#include "common/profiler.h"

int main(int argn, char *argv[]) {

    Profiler::HeapProfilerStart("test1.pprof");

    auto a = new char[1024*1024];

    Profiler::HeapProfilerDump("dump1");

    delete [] a;


    auto b = new char[100];

    Profiler::HeapProfilerDump("dump2");

    Profiler::HeapProfilerStop();

    return 0;
}
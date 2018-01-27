# gperftools_sample

使用gperftools的例子。

之所以把它做到github上，主要是，每次需要使用gperftools时，都需要重新撸一遍过程。（编译、重新使用gperftools时的试错、等等）

于是痛定思痛，完整撸一遍到github上。下次再用时，希望减少不必要的工作量！！！


### 编译

本项目只在linux下使用

执行下列语句：
```bash
cd deps/lib_linux/
./gen.sh
cd ../../build/
./domake.sh
```

### libPProf.so

go tool pprof 有局限性，只能查看golang语言层面的heap分配情况

因此可以结合 本项目编译出来的 libPProf.so

查看go语言与C混合编程下的C层面的heap分配情况


### libPProf.so 例子

```go

/*
#cgo LDFLAGS: -L./ -lPProfGo
#include "PProfGo.h"
#include "stdlib.h"
*/
import "C"

func main() {
  HeapProfilerStart("test.prof")

  // test1代码略

  HeapProfilerDump("test1")

  go func() {
    // test2代码略
  
    HeapProfilerDump("test2")
  }
  
  HeapProfilerStop()
}


func HeapProfilerStart(f string) {
  spath := C.CString(f)
  defer C.free(unsafe.Pointer(spath))
  C._HeapProfilerStart(spath)
}

func HeapProfilerDump(s string) {
  temps := C.CString(s)
  defer C.free(unsafe.Pointer(temps))
  C._HeapProfilerDump(temps)
}

func HeapProfilerStop() {
  C._HeapProfilerStop()
}

```

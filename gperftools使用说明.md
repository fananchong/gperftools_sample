# **gperftools 使用说明**

## 一、简介

gperftools是google开发的一款非常实用的工具集，主要包括：
1. tcmalloc 性能优异的内存分配器
2. heap-profiler 基于tcmalloc的堆内存检测、内存泄漏分析工具
3. profiler 基于tcmalloc实现的CPU性能监测工具


其中，
- tcmalloc 在 window/linux 下均支持
- heap-profiler 仅在linux平台支持
- profiler 仅在linux平台支持


## 二、编译

- 可以从github上下载源码:

  > https://github.com/gperftools/gperftools.git

- Window

  打开 gperftools.sln，默认编译DLL库。编译后输出 libtcmalloc_minimal.lib libtcmalloc_minimal.dll

  可以更改sln配置编译静态库

- linux

  依赖库 libunwind、可选依赖库 graphviz(用于图表输出结果)

  编译输出 libtcmalloc_minimal.a 、libprofiler.a 等静态库

  编译输出 pprof。pprof为分析工具

  编译命令如下：

  ```bash
   ./configure --enable-libunwind CPPFLAGS=-I/path_to_libunwind/include LDFLAGS=-L/path_to_libunwind/lib -enable-frame-pointers
  ```

## 三、tcmalloc

- 介绍

  tcmalloc 的内存分配，比glibc采用的ptmalloc，有以下优势：

  1. 对于小内存分配，据介绍几乎无消耗
  2. 对于多线程下分配，据介绍它有很好性能


- 如何使用

  要使用tcmalloc，仅需要做如下事情：
  1. 链接 libtcmalloc_minimal 库
  2. 随意调用以下 tc_ 开头的函数 （编译器会忽略链接 没有使用的库。本步骤是确保tcmalloc库有正确被链接进程序）

  完成以上步骤，不需要更改程序中的一行代码，即可实现 malloc/free、new/delete 等等被替换为tc_malloc/tc_free


## 四、自己封装的heap-profiler、profiler接口

heap-profiler、profiler接口已经非常简洁

在头文件heap-profiler.h、profiler.h中已经分别定义

因为该部分内容window不支持。封装下方便跨平台编程

```c++
class Profiler
{
public:
  // heap
  static void heap_profiler_start(const char *prefix);
  static void heap_profiler_dump(const char *reason);
  static void heap_profiler_stop();
  static bool heap_profiler_running();

  // cpu
  static int profiler_start(const char *prefix);
  static void profiler_stop();
  static void profiler_flush();
};
```

## 五、heap-profiler介绍

可以在想要检查内存使用情况的地方调用 Profiler::heap_profiler_start 、 Profiler::heap_profiler_stop

通常可以在程序开始时调用Profiler::heap_profiler_start

程序结束时调用 Profiler::heap_profiler_stop

Profiler::heap_profiler_dump 可以即时输出统计信息

heap-profiler 可以在内存增加一定量时也会自己输出统计信息



- 实例分析

  ```c++
  void my_malloc1()
  {
    void *ptr1 = malloc(1 * 1024 * 1024);
  }

  class A
  {
  public:
    char a[2 * 1024 * 1024];
  };
  std::shared_ptr<A> g_a;
  void my_malloc2()
  {
    g_a = std::make_shared<A>();
  }

  typedef std::function<void(const std::string &data)> test_func_type;
  test_func_type g_h;
  void do_nothing(const std::string &data) {}
  void my_malloc3()
  {
    g_h = std::bind(do_nothing, std::string(4 * 1024 * 1024, 0));
  }

  void test_heap()
  {
    Profiler::heap_profiler_set_allocation_interval(1 * 1024 * 1024);  // bytes
    Profiler::heap_profiler_start("heap");

    my_malloc1(); // 1M
    my_malloc2(); // 2M
    my_malloc3(); // 4M
    void *ptr1 = malloc(1 * 1024 * 1024); // 1M
    void *ptr2 = new char[2 * 1024 * 1024]; // 2M

    Profiler::heap_profiler_dump("xxx");
    Profiler::heap_profiler_stop();
  }
  ```

- 编译执行
- 使用pprof生成结果

  命令如下：

  ```bash
  pprof --text ./test_gperftools heap.0007.heap
  pprof --pdf ./test_gperftools heap.0007.heap > 1.pdf
  pprof ./test_gperftools heap.0007.heap
  ```


## 六、profiler介绍

可以在想要检查CPU使用情况的地方调用 Profiler::profiler_start 、 Profiler::profiler_stop

通常可以在程序开始时调用Profiler::profiler_start

程序结束时调用 Profiler::profiler_stop

Profiler::profiler_flush 可以即时输出统计信息

- 实例分析

  ```c++
  size_t g_count = 1000000;

  void my_thread(int no)
  {
    float a = 0.0f;
    for (size_t i = 0; i < g_count; i++)
    {
      a += rand();
    }
  }

  void test_cpu()
  {
    srand((unsigned)time(0));
    Profiler::profiler_start("cpu.prof");
    for (size_t i = 0; i < 2; i++)
    {
      std::thread(my_thread, i).detach();
    }
    float a = 0.0f;
    for (size_t i = 0; i < g_count * 10; i++)
    {
      a += rand();
    }
    Profiler::profiler_flush();
    Profiler::profiler_stop();
  }
  ```

- 编译执行
- 使用pprof生成结果

  ```bash
  pprof --text ./test_gperftools ./cpu.prof
  pprof --pdf ./test_gperftools ./cpu.prof > 2.pdf
  pprof ./test_gperftools ./cpu.prof
  ```


## 七、不足之处

gperftools 是对 C/C++ 下的一款工具集。由于服务器端现在基本是于脚本混合编程的。对于脚本方面的内存泄露、CPU占用情况就略有不足了

- 实例分析

  ```lua
  import gvoe
  import array

  objs = []
  def my_malloc1():
    global objs
    obj = array.array("L", xrange(0, 1 * 1024 * 1024))
    objs.append(obj)

  def start(run_container, args, cfg):
    gvoe.heap_profiler_set_allocation_interval(100 * 1024 * 1024)  # 100M
    my_malloc1()
  ```

- 执行py脚本
- 使用pprof生成结果

  ```bash
  pprof --text gvoe_node test.0001.heap
  pprof --pdf gvoe_node test.0001.heap  > 3.pdf
  ```


## 八、总体评价

gperftools 使用上，基本只要链接进程序，就可以使用，是非常便利的

gperftools 性能上，网络上的文章普遍对 tcmalloc 的内存分配效率有很好的评价

gperftools 分析方面上，能已文本及图表的方式呈现，对于分析热点有很好的帮助

总体来说，对于C/C++编程来说，gperftools是一个很好的工具集

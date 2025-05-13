
### 分析代码工具

clang-tidy 是 C++ 代码质量的“全能卫士”，适合提升代码规范性和性能。

clazy 是 Qt 项目的“专属医生”，深度解决框架相关陷阱。


### perf 性能分析工具

Perf（Performance Counters for Linux）是 Linux 内核自带的性能分析工具

### Valgrind 动态分析工具集
Valgrind 是一套用于 Linux 系统的开源动态分析工具集，主要用于检测内存错误、性能优化和多线程问题。

```
Memcheck（内存检测）

valgrind --leak-check=full --show-leak-kinds=all ./your_program

Callgrind（性能分析）
    函数调用统计：记录函数执行时间、调用次数及调用关系，生成可视化调用图（需配合 kcachegrind）

    valgrind --tool=callgrind  -trace-children=yes ./your_program
```



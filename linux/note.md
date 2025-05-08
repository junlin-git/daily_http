
### 编译ubuntu内核需要安装

### 一般内核是缺失源码 需要重新下载或安装

```

git clone https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git

```



```
sudo cp /boot/你的配置  内核的.config
sudo apt install bison
sudo apt install flex
sudo apt install libncurses-dev
sudo apt install  pkg-config


```



### 编译QT6.7

### 分析代码工具

clang-tidy​​ 是 C++ 代码质量的“全能卫士”，适合提升代码规范性和性能。

​​clazy​​ 是 Qt 项目的“专属医生”，深度解决框架相关陷阱。


### perf 性能分析工具

Perf（Performance Counters for Linux）是 Linux 内核自带的性能分析工具

### Valgrind 动态分析工具集
Valgrind 是一套用于 Linux 系统的开源动态分析工具集，主要用于检测内存错误、性能优化和多线程问题。

```
Memcheck（内存检测）

valgrind --leak-check=full --show-leak-kinds=all ./your_program

​​Callgrind（性能分析）​​
    ​​函数调用统计​​：记录函数执行时间、调用次数及调用关系，生成可视化调用图（需配合 kcachegrind）

    valgrind --tool=callgrind  -trace-children=yes ./your_program

```

```
sudo apt install libgl1-mesa-dev

sudo apt-get install libxcb-cursor0

sudo apt-get install mesa-utils

sudo apt install ninja-build

cmake .. -DCMAKE_INSTALL_PREFIX=/home/hejunlin/qt6


裁剪

cmake .. -DQT_BUILD_EXAMPLES=OFF \
    -DQT_FEATURE_gui=ON -DQT_FEATURE_widgets=ON -DQT_FEATURE_sql=OFF \
    -DCMAKE_INSTALL_PREFIX=/home/hejunlin/qt6

    


```


### GIT使用问题

```
需要在github 中生成key 才可以使用
Personal access tokens” -> “Tokens (classic)”


Fine-grained personal access tokens 和Personal access tokens (classic) 区别
在于前者只能访问特定的项目后者可以访问全部，后者的权限更大

```

```

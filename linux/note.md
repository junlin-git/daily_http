
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


```
sudo apt install libgl1-mesa-dev

sudo apt-get install libxcb-cursor0

sudo apt-get install mesa-utils

sudo apt install ninja-build

cmake .. -DCMAKE_INSTALL_PREFIX=/home/hejunlin/qt6

```

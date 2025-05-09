

### 1. git 下载libcoap对应的代码


```
git clone https://github.com/obgm/libcoap.git

git tag

git checkout v4.3.5

./autogen.sh

/configure  --disable-doxygen --disable-manpages --disable-shared -prefix=$PWD/install


export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib
```



下载太慢

```
sudo apt-get install uget

sudo apt-get install aria2

配置

--enable-rpc=true -D --disable-ipv6 --check-certificate=false

```
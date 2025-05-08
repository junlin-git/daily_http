

## 1. 编译mosquitto服务器并使用
```

git clone https://github.com/eclipse-mosquitto/mosquitto.git

mkdir build 

cd build 

cmake .. -DDOCUMENTATION=OFF -DWITH_STATIC_LIBRARIES=ON -DCMAKE_INSTALL_PREFIX=/home/junlin/opensource/mosquitto/build/install

```
## 2. 有5个应用程序

###  2.1  服务器程序 mosquitto
```
mosquitto -c /etc/mosquitto/mosquitto.conf -v  # -v显示详细日志

```
### 2.2 用户管理 mosquitto_passwd

```
./mosquitto_passwd  -c ./pwfile junlin

或 追加用户 -C -c会覆盖旧文件

./mosquitto_passwd  ./pwfile  junlin
```

### 2.3 客户端命令行操作 mosquitto_sub ​​订阅消息​​

 ```

./mosquitto_sub -h 127.0.0.1 -t "sensors/temperature"  -u junlin -P a -q 1

```
#### 说明 :
---
```


-q：QoS 等级（0/1/2）

-v：显示详细消息头

```

### 2.4 客户端命令行操作 mosquitto_pub ​​发布消息​​

```

/mosquitto_pub -h 127.0.0.1  -t "sensors/temperature" -m 0xffff  -u junlin -P a


```
#### 说明 :
---

```
-r：保留消息（新订阅者首次连接可获取）

--will-topic：设置遗嘱消息主题
```


## 3. 配置文件说明

### 3.1 mosquitto.conf

```
# 基础配置
listener 1883 0.0.0.0       # 监听所有IP的1883端口
allow_anonymous false       # 禁用匿名访问
persistence true            # 启用消息持久化存储

# 用户认证
password_file /home/junlin/opensource/mosquitto/build/install/test/pwfile  # 密码文件路径
acl_file /home/junlin/opensource/mosquitto/build/install/test/aclfile      # 权限控制文件

# 日志管理
log_dest file /var/log/mosquitto/mosquitto.log  # 日志输出到文件


```


### 3.2 pwfile 主要用于创建用户密码​

```
mosquitto_passwd -c /etc/mosquitto/pwfile username  # 新建用户（-c会覆盖旧文件）

#或者

mosquitto_passwd /etc/mosquitto/pwfile user2        # 追加用户

```


### 3.3 aclfile 主要控制主题和权限

```
user junlin
topic write sensors/#      # 允许发布以test开头的主题
topic read $SYS/#       # 允许订阅系统主题

```

### 3.4 mosquitto.log 用于保存LOG


### 客户端下载 MQTTX

```
https://www.emqx.com/en/downloads/MQTTX


```
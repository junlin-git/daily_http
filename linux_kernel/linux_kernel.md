### linux 各个版本的内核下载地址 https://kernel.googlesource.com/pub/scm/linux/kernel/git/stable/linux.git/+refs

### 版本下载地址 https://kernel.googlesource.com/pub/scm/linux/kernel/git/stable/linux.git/+archive/refs/heads/

### 编译对应的内核进行安装


其中的系统配置在 arch/x86/configs/



对应的内核版本号可以在 make menuconfig  中看

```

cp /boot/config-6.8.0-40-generic  .config 

make menuconfig  

make -j8 

sudo make modules_install # 安装模块

sudo make install         # 安装内核

sudo update-initramfs -c -k 6.8.12  # 替换为你的内核版本,读取当前系统的内核版本号,并在/boot目录下查找与该版本号匹配的内核映像文件和initramfs文件

sudo update-grub  #重新生成GRUB的启动菜单配置文件
 
sudo reboot


```

### 注意，编译KO模块如果版本号不一样，会内核KO加载失败或系统崩溃

### 编译 

```
make[3]: *** No rule to make target 'debian/canonical-certs.pem', 
needed by 'certs/x509_certificate_list'.  Stop.


可以禁用配置
sudo scripts/config --disable SYSTEM_TRUSTED_KEYS
sudo scripts/config --disable SYSTEM_REVOCATION_KEYS
make mrproper 等于 make disclean web

或者
openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes
然后在内核中
然后在内核配置中指定路径：
CONFIG_SYSTEM_TRUSTED_KEYS="path/to/cert.pem"

--------------------------------------------

或者

进入 Cryptographic API → Certificates for signature checking
清空以下选项的字段：

Additional X.509 keys for default system keyring
Additional X.509 keys for secondary system keyring

系统会自己生成,其实是让系统然后在内核配置中指定路径

```
diff命令

        -b：忽略所有空白字符的差别，包括制表符和空格。

        -B：忽略空白行。

        -c：输出一个简短的摘要，包括每组不同的行的上下文。

        -C n：输出每组不同的行及其前后 n 行的上下文。

        -u：以统一格式输出，这是默认格式的改进版，常用于版本控制系统。

        -w：忽略所有非字母数字字符，即只比较字母数字字符。

        -y：以并排格式显示两个文件的不同之处。

        -N：即使文件不存在也进行比较，如果文件在另一个文件中不存在，则认为文件内容为空。

        -q：只报告文件是否相同，不显示具体的差异。

        -s：如果文件相同则报告，否则不输出任何内容。

```


export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu/

sudo ldconfig  更新库依赖

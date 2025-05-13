obj-m += fb.o

#KDIR := /usr/src/linux-hwe-6.8-headers-6.8.0-40
#KDIR := /usr/src/linux-headers-6.8.0-40-generic/
KDIR :=/home/junlin/opensource/linux_6.8
PWD := $(shell pwd)


all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc  app_fb.c -o app_fb

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -rf app_fb

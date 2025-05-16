obj-m += net.o

#KDIR := /usr/src/linux-hwe-6.8-headers-6.8.0-40
#KDIR := /usr/src/linux-headers-6.8.0-40-generic/
KDIR :=/home/junlin/opensource/linux-6.14.6
PWD := $(shell pwd)


all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

obj-m += kernel_char_dev.o

#KDIR := /usr/src/linux-hwe-6.8-headers-6.8.0-40
#KDIR := /usr/src/linux-headers-6.8.0-40-generic/
KDIR :=/home/junlin/opensource/linux/
PWD := $(shell pwd)


all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

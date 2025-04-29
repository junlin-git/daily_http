obj-m += kernel_char_dev.o

KDIR := /usr/src/linux-hwe-6.8-headers-6.8.0-40

PWD := $(shell pwd)


all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
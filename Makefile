ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m := hello.o
ccflags-y += -g
else

# normal makefile
KDIR ?= /lib/modules/`uname -r`/build
CROSS_COMPILE ?= arm-eabi-

default:
	$(MAKE) -C $(KDIR) M=$(PWD)
	cp hello.ko hello.ko.unstripped
	$(CROSS_COMPILE)strip -g hello.ko

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

%.s %.i: %.c
	$(MAKE) -C $(KDIR) M=$(PWD) $@
endif
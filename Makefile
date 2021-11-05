MODULE_NAME := tcpjack
UNAME := $(shell uname -r)

obj-m := $(MODULE_NAME).o
$(MODULE_NAME)-y := src/main.o
clean-files := *.o *.ko *.mod.[co] *~
ccflags-y += -O2 -Wall -I$(src)/include

.PHONY: all clean install_files install test

all:
	$(MAKE) -C /lib/modules/$(UNAME)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(UNAME)/build M=$(PWD) clean

install_files: $(MODULE_NAME).ko
	mkdir -p /lib/modules/$(UNAME)/extra/tcpjack
	mkdir -p /etc/modprobe.d
	install -m 644 conf/tcpjack.modprobe.conf /etc/modprobe.d/tcpjack.conf
	install -m 755 $(MODULE_NAME).ko /lib/modules/$(UNAME)/extra/tcpjack/
	/sbin/depmod

install: install_files
	-rmmod $(MODULE_NAME)
	modprobe $(MODULE_NAME)

test:
	sudo dmesg -C
	sudo insmod tcpjack.ko
	sudo rmmod tcpjack.ko
	sudo dmesg
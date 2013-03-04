#ccflags-y += -std=gnu99
#EXTRA_CFLAGS	= -Wno-declaration-after-statement -std=gnu99
obj-m += dvb_net.o

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

info:
	modinfo dvb_net.ko

ins:
	sudo insmod dvb_net.ko
	$(MAKE) log

rem:
	sudo rmmod dvb_net
	$(MAKE) log

ls:
	lsmod | grep dvb_net

log:
	tail /var/log/syslog

KDIR = /lib/modules/`uname -r`/build

all:
	make -C $(KDIR) M=`pwd`

auto:
	make -C $(KDIR) M=`pwd`
	sudo make
	sudo insmod Hook.ko
	sudo rmmod Hook
	
clean:
	make -C $(KDIR) M=`pwd` clean

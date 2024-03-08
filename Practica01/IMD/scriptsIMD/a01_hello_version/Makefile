obj-m += hello_world.o

hello_world-objs := hello_version.o

KERNEL_DIR ?= $(HOME)/IMD/buildroot/buildroot/output/build/linux-6.1.76
all:
	make -C $(KERNEL_DIR) \
	ARCH=arm CROSS_COMPILE=/home/carlos/IMD/buildroot/buildroot/output/host/bin/arm-none-linux-gnueabihf- \
	M=$(PWD) modules
	
clean:
	make -C $(KERNEL_DIR) \
	ARCH=arm CROSS_COMPILE=/home/carlos/IMD/buildroot/buildroot/output/host/bin/arm-none-linux-gnueabihf- \
	M=$(PWD) clean
	
deploy:
	scp *.ko root@192.168.60.95:
	sudo cp *.ko /home/carlos/IMD/nfsroot/root/hello_world
	
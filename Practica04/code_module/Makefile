ifneq ($(KERNELRELEASE),)
obj-m += holamundo_i2c_driver.o
else
KDIR := /home/carlos/IMD/buildroot/buildroot/output/build/linux-6.6.22
all:
	$(MAKE) -C $(KDIR) M=$$PWD
endif

obj-m += char_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

# COP4600-HW2
## Kernel Character Driver
__________________________________________________

### Compiling the module
```sh
$ make
```

### Inserting the module
```sh
$ sudo insmod char_driver.ko
```

### Getting the assigned major number
```sh
$ cat /proc/devices
```
In the output, look for the name of the module (fjr) and there will be a number assigned to it (mine was 247)

### Creating the device in the /dev/ tree
```sh
$ cd /dev/
$ sudo mknod char_driver c 247 1
$ sudo chmod 666 char_driver
```
Replace 247 with the correct major number. You can replace "char_driver" with whatever name you want the device to be named in the /dev/ folder.

The chmod command changes permissions on char_driver to allow you to read and write to it.

### Reading and writing to the device
```sh
$ echo "Put some text here to write to the device" > /dev/char_driver
$ echo "You can write to it as much as you want (up to 1KB, then it stops)" > /dev/char_driver
$ cat /dev/char_driver

Put some text here to write to the device
You can write to it as much as you want (up to 1KB, then it stops)
```

### View the kernel output messages
```sh
$ dmesg
```
There are various printk statements in the code that output when the device is opened or modified.

### Easily read/write
```sh
$ gcc test.c -o test
$ ./test
```
Make sure you edit test.c to have the correct device open up. If you name your device char_driver, it will work.

This program asks for input, then reads it back to you if you want to see it. EZPZ.

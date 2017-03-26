#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#define DEVCE_NAME "chardriver"
#define CLASS_NAME "char"

// Kernel driver info things
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Farza, Jacob, Richie");
MODULE_DESCRIPTION("Linux character device driver");
MODULE_VERSION("9001");

// 1KB Buffer to write to
static char buffer[1024];
// Major number to be assigned to the device
static int major_number;

static int dev_open(struct inode *, struct file *);
static int dev_rls(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
    .read = dev_read,
    .open = dev_open,
    .write = dev_write,
    .release = dev_rls,
};

int init_module(void)
{
    
}
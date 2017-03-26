#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#define DEVICE_NAME "fjr"

// Kernel driver info things
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Farza, Jacob, Richie");
MODULE_DESCRIPTION("Linux character device driver");
MODULE_VERSION("9001");

typedef struct charq
{
    char c;
    struct charq *next;
} charq;

// Major number to be assigned to the device
static int major_number;

// Head of the character queue to be printed in FIFO order
charq *head = NULL;

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

charq *make_c(char c)
{
    charq *node = kmalloc(sizeof(charq), GFP_KERNEL);
    node->c = c;
    node->next = NULL;
    
    return node;
}

charq *freeq(charq *node)
{
    if (node == NULL)
        return NULL;
    
    freeq(node->next);
    kfree(node);
    
    return NULL;
}

int init_module(void)
{
    // register_chrdev returns a major number when 0 is passed to it
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    
    if (major_number < 0)
    {
        printk(KERN_ALERT "FJR could not register a major number :(\n");
        return major_number;
    }
    
    printk(KERN_INFO "FJR: assigned major number %d\n", major_number);
    
    return 0;
}

void cleanup_module(void)
{
    freeq(head);
    unregister_chrdev(major_number, DEVICE_NAME);
}

static int dev_open(struct inode *inod, struct file *fil)
{
    printk(KERN_ALERT "FJR opened");
    return 0;
}

static ssize_t dev_read(struct file *filp, char *buff, size_t len, loff_t *off)
{
    int count = 0;
    charq *temp;

    while (head != NULL)
    {
        put_user(head->c, buff++);

        temp = head;
        count++;
        head = head->next;
        
        kfree(temp);
    }

    return count;
}

static ssize_t dev_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{
    charq *last_node = head;
    int i = 0;
    
    while (last_node != NULL && last_node->next != NULL)
    {
        last_node = last_node->next;
    }
    
    while (i < len)
    {
        charq *new_node = make_c(buff[i]);
        if (head == NULL)
        {
            head = new_node;
            last_node = head;
        }
        else
        {
            last_node->next = new_node;
            last_node = last_node->next;
        }
            
        
        i++;
    }
    
    return i;
}

static int dev_rls(struct inode *inod, struct file *fil)
{   
    printk(KERN_ALERT "FJR closed\n");
    return 0;
}

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/fs.h>
#include <asm/uaccess.h>

#include <linux/gpio.h>
#include <asm/gpio.h>

#define DEVICE_NAME     "gpio_device"
#define DEVICE_MAJOR    240
#define GPIO_num        4

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GPIO driver");
MODULE_AUTHOR("RRV");

static unsigned long procfs_buffer_size = 0;
static char buffer_data[3];
static char led_status = 0;


static int dev_open(struct inode *inode,struct file *file);
static int dev_release(struct inode *inode,struct file *file);
static ssize_t dev_write(struct file *file, const char *buffer, size_t len, loff_t *offset);


static struct file_operations fops =
{
    .owner          = THIS_MODULE,
    .open           = dev_open,
    .release        = dev_release,
    .write          = dev_write,
};

int dev_init(void)
{
    int ret;

    ret = register_chrdev(DEVICE_MAJOR,DEVICE_NAME,&fops);
    if(ret < 0){
        printk(KERN_ALERT "Device registeration failed \n");
    }
    else{
        printk(KERN_ALERT "Device registeration succeed \n");
    }

    ret = gpio_request(GPIO_num,"gpio_test");
    if(ret != 0){
        printk(KERN_ALERT "GPIO%d is not requested\n",GPIO_num);
    }
    else{
        printk(KERN_ALERT "GPIO%d is requested\n",GPIO_num);
    }

    ret = gpio_direction_output(GPIO_num,0);
    if(ret != 0){
        printk(KERN_ALERT "GPIO%d in not set output\n",GPIO_num);
    }
    else{
        printk(KERN_ALERT "GPIO%d is set output and out is low\n",GPIO_num);
    }

    return 0;
}

void dev_exit(void)
{
        printk(KERN_ALERT "module exit\n");

    gpio_free(GPIO_num);

    unregister_chrdev(DEVICE_MAJOR,DEVICE_NAME);

}
static int dev_open(struct inode *inode,struct file *file)
{
    gpio_set_value(GPIO_num, led_status);

    printk(KERN_ALERT "GPIO%d is set high\n",GPIO_num);

    return 0;
}

static int dev_release(struct inode *inode,struct file *file)
{
    gpio_set_value(GPIO_num, led_status);

    printk(KERN_ALERT "GPIO%d is ser low\n",GPIO_num);

    return 0;
}

static ssize_t dev_write(struct file *file, const char *buffer, size_t len, loff_t *offset)
{
    procfs_buffer_size = len;
    if ( copy_from_user(buffer_data, buffer, procfs_buffer_size) )
    {
        return -EFAULT;
    }

    *offset += len;

    if(buffer_data[0] == '1')
    {
        led_status = 1;

    }
    else if(buffer_data[0] == '0')
    {
        led_status = 0;
    }

    pr_info("user input string: %s\n",buffer_data);
    pr_info("user input string len: %lu\n",procfs_buffer_size);

    return procfs_buffer_size;
}

module_init(dev_init);
module_exit(dev_exit);

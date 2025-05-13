#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DEVICE_NAME "simple_drv"
#define CLASS_NAME  "simple_class"

static int major_num;
static struct class *drv_class = NULL;
static struct cdev my_cdev;

// 设备缓冲区（最大256字节）
static char device_buffer[256];
static int buffer_offset = 0;

// 文件操作函数集合
static ssize_t drv_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    int bytes_to_copy = min(count, (size_t)(buffer_offset - *f_pos));

    if (bytes_to_copy == 0)
        return 0;

    if (copy_to_user(buf, device_buffer + *f_pos, bytes_to_copy))
        return -EFAULT;

    *f_pos += bytes_to_copy;
    return bytes_to_copy;
}

static ssize_t drv_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    int bytes_to_copy = min(count, sizeof(device_buffer) - buffer_offset);

    if (bytes_to_copy == 0)
        return -ENOSPC;

    if (copy_from_user(device_buffer + buffer_offset, buf, bytes_to_copy))
        return -EFAULT;

    buffer_offset += bytes_to_copy;
    return bytes_to_copy;
}

static int drv_open(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int drv_release(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "Device closed\n");
    return 0;
}

// 文件操作结构体
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = drv_read,
    .write = drv_write,
    .open = drv_open,
    .release = drv_release,
};

// 模块初始化
static int __init simple_drv_init(void)
{
    // 1. 分配设备号
    if (alloc_chrdev_region(&major_num, 0, 1, DEVICE_NAME) < 0) {
        printk(KERN_ALERT "Failed to allocate device number\n");
        return -1;
    }

    // 2. 创建字符设备
    cdev_init(&my_cdev, &fops);
    if (cdev_add(&my_cdev, major_num, 1) < 0) {
        unregister_chrdev_region(major_num, 1);
        printk(KERN_ALERT "Failed to add cdev\n");
        return -1;
    }

    // 3. 创建设备类
    drv_class = class_create(CLASS_NAME);
    if (IS_ERR(drv_class)) {
        cdev_del(&my_cdev);
        unregister_chrdev_region(major_num, 1);
        printk(KERN_ALERT "Failed to create class\n");
        return PTR_ERR(drv_class);
    }

    // 4. 创建设备节点
    device_create(drv_class, NULL, major_num, NULL, DEVICE_NAME);
    printk(KERN_INFO "Driver loaded: major=%d DEVICE_NAME=%s\n", major_num,DEVICE_NAME);
    return 0;
}

// 模块退出
static void __exit simple_drv_exit(void)
{
    device_destroy(drv_class, major_num);
    class_destroy(drv_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(major_num, 1);
    printk(KERN_INFO "Driver unloaded\n");
}

module_init(simple_drv_init);
module_exit(simple_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple Character Device Driver");

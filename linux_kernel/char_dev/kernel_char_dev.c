#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DEVICE_NAME "simple_char_dev"
#define CLASS_NAME "simplechar"

static int major_number;
static struct class* simple_char_class = NULL;
static struct device* simple_char_device = NULL;

static dev_t dev_num;
static struct cdev my_cdev;

static char *device_buffer;
static size_t buffer_size = 1024;

static int simple_char_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "%s: Device opened\n", DEVICE_NAME);
    return 0;
}

static int simple_char_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "%s: Device closed\n", DEVICE_NAME);
    return 0;
}

static ssize_t simple_char_read(struct file *file, char __user *buffer, size_t len, loff_t *offset) {
    size_t bytes_to_copy = min(len, buffer_size - *offset);
    if (bytes_to_copy == 0) {
        return 0; // End of file
    }
    if (copy_to_user(buffer, device_buffer + *offset, bytes_to_copy)) {
        return -EFAULT;
    }
    *offset += bytes_to_copy;
    return bytes_to_copy;
}

static ssize_t simple_char_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset) {
    size_t bytes_to_copy = min(len, buffer_size - *offset);
    if (bytes_to_copy == 0) {
        return -ENOSPC; // No space left on device
    }
    if (copy_from_user(device_buffer + *offset, buffer, bytes_to_copy)) {
        return -EFAULT;
    }
    *offset += bytes_to_copy;
    return bytes_to_copy;
}

static struct file_operations fops = {
    .open = simple_char_open,
    .release = simple_char_release,
    .read = simple_char_read,
    .write = simple_char_write,
};

static int __init simple_char_init(void) {
    int result;

    // Allocate a major number dynamically
    result = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (result < 0) {
        printk(KERN_ALERT "%s: Failed to allocate a major number\n", DEVICE_NAME);
        return result;
    }
    major_number = MAJOR(dev_num);
    printk(KERN_INFO "%s: registered correctly with major number %d\n", DEVICE_NAME, major_number);

    // Initialize the cdev structure and add it to the kernel
    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;
    result = cdev_add(&my_cdev, dev_num, 1);
    if (result < 0) {
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "%s: Failed to add cdev\n", DEVICE_NAME);
        return result;
    }

    // Create a class for the device
    simple_char_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(simple_char_class)) {
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "%s: Failed to register device class\n", DEVICE_NAME);
        return PTR_ERR(simple_char_class);
    }
    printk(KERN_INFO "%s: device class registered correctly\n", DEVICE_NAME);

    // Create a device node
    simple_char_device = device_create(simple_char_class, NULL, dev_num, NULL, DEVICE_NAME);
    if (IS_ERR(simple_char_device)) {
        class_destroy(simple_char_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "%s: Failed to create the device\n", DEVICE_NAME);
        return PTR_ERR(simple_char_device);
    }
    printk(KERN_INFO "%s: device class created correctly\n", DEVICE_NAME);

    // Allocate memory for the device buffer
    device_buffer = kmalloc(buffer_size, GFP_KERNEL);
    if (!device_buffer) {
        device_destroy(simple_char_class, dev_num);
        class_destroy(simple_char_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "%s: Failed to allocate memory for device buffer\n", DEVICE_NAME);
        return -ENOMEM;
    }
    memset(device_buffer, 0, buffer_size);

    printk(KERN_INFO "%s: initialized successfully\n", DEVICE_NAME);
    return 0;
}

static void __exit simple_char_exit(void) {
    kfree(device_buffer);
    device_destroy(simple_char_class, dev_num);
    class_unregister(simple_char_class);
    class_destroy(simple_char_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_INFO "%s: exited successfully\n", DEVICE_NAME);
}

module_init(simple_char_init);
module_exit(simple_char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple character device driver");
MODULE_VERSION("0.1");




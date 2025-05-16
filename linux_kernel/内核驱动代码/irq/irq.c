#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/interrupt.h>

#define DEVICE_NAME "simple_irq_dev"
#define CLASS_NAME "simple_irq_class"

static int major_number;
static struct class* simple_int_class = NULL;
static struct device* simple_int_device = NULL;
#define VIRTUAL_IRQ_NUMBER 19 // 自定义的虚拟中断号
// 中断号
static unsigned int irqNumber=VIRTUAL_IRQ_NUMBER;// 自定义的虚拟中断号

// ISR原型声明
static irqreturn_t simple_int_handler(int irq, void *dev_id);



static int  dev_open(struct inode *inode, struct file *file) {


    return 0;
}

static struct file_operations fops = {
    .open = dev_open,
};



static int __init simple_int_init(void) {
    printk(KERN_INFO "Simple Int: Initializing the Simple Interrupt LKM\n");

    // 注册一个字符设备
    major_number = register_chrdev(0, DEVICE_NAME, NULL);
    if (major_number < 0) {
        printk(KERN_ALERT "Simple Int failed to register a major number\n");
        return major_number;
    }
    printk(KERN_INFO "Simple Int: registered correctly with major number %d\n", major_number);

    // 注册设备类
    simple_int_class = class_create(CLASS_NAME);
    if (IS_ERR(simple_int_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(simple_int_class);
    }
    printk(KERN_INFO "Simple Int: device class registered correctly\n");

    // 注册设备驱动
    simple_int_device = device_create(simple_int_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(simple_int_device)) {
        class_destroy(simple_int_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(simple_int_device);
    }

    // 请求中断
    //irqNumber = /* 替换为你的设备的实际中断号 */;
    if (request_irq(irqNumber, simple_int_handler, IRQF_SHARED, DEVICE_NAME, &simple_int_device)) {
        device_destroy(simple_int_class, MKDEV(major_number, 0));
        class_unregister(simple_int_class);
        class_destroy(simple_int_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Simple Int: Failed to request interrupt %d\n", irqNumber);
        return -EIO;
    }
    printk(KERN_INFO "Simple Int: Registered IRQ %d\n", irqNumber);

    return 0;
}

static void __exit simple_int_exit(void) {
    free_irq(irqNumber, &simple_int_device);
    device_destroy(simple_int_class, MKDEV(major_number, 0));
    class_unregister(simple_int_class);
    class_destroy(simple_int_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "Simple Int: Goodbye from the LKM!\n");
}

static irqreturn_t simple_int_handler(int irq, void *dev_id) {
    printk(KERN_INFO "Simple Int: Handling the interrupt\n");
    return IRQ_HANDLED;
}

module_init(simple_int_init);
module_exit(simple_int_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Linux char driver that handles interrupts.");
MODULE_VERSION("0.1");




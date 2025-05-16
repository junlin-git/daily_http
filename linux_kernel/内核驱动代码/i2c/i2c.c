#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/slab.h>

#define DEVICE_NAME "simple_i2c_subdev"
#define I2C_SUBDEV_ADDR 0x50

// 设备结构体
struct i2c_subdev_data {
    struct i2c_client *client;
};

// I2C读函数
static ssize_t i2c_subdev_read(struct file *filp, char __user *buf, size_t count, loff_t *off) {
    struct i2c_subdev_data *data = filp->private_data;
    u8 reg = 0; // 假设我们要读取寄存器0
    int ret;

    // 发送寄存器地址
    ret = i2c_master_send(data->client, &reg, 1);
    if (ret != 1) {
        printk(KERN_ERR "Failed to send register address\n");
        return -EIO;
    }

    // 读取数据
    u8 value;
    ret = i2c_master_recv(data->client, &value, 1);
    if (ret != 1) {
        printk(KERN_ERR "Failed to read data\n");
        return -EIO;
    }

    // 将数据复制到用户空间
    if (copy_to_user(buf, &value, 1)) {
        return -EFAULT;
    }

    return 1;
}

// I2C写函数
static ssize_t i2c_subdev_write(struct file *filp, const char __user *buf, size_t count, loff_t *off) {
    struct i2c_subdev_data *data = filp->private_data;
    u8 msg[2];
    int ret;

    // 获取用户空间的数据
    if (count != 1 || copy_from_user(msg + 1, buf, 1)) {
        return -EINVAL;
    }

    msg[0] = 0; // 假设我们要写入寄存器0

    // 发送寄存器地址和数据
    ret = i2c_master_send(data->client, msg, 2);
    if (ret != 2) {
        printk(KERN_ERR "Failed to write data\n");
        return -EIO;
    }

    return 1;
}

// 设备打开函数
static int i2c_subdev_open(struct inode *inode, struct file *filp) {

    struct i2c_subdev_data *data;
    data = kmalloc(sizeof(*data), GFP_KERNEL);
    if (!data)
        return -ENOMEM;

    filp->private_data = data;
    data->client = container_of(inode->i_cdev, struct i2c_client, dev);

    printk(KERN_INFO "SimpleI2CSubDev: Device opened\n");
    return 0;
}

// 文件操作结构体
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = i2c_subdev_read,
    .write = i2c_subdev_write,
    .open=i2c_subdev_open
};


// 设备释放函数
static int i2c_subdev_release(struct inode *inode, struct file *filp) {
    kfree(filp->private_data);
    printk(KERN_INFO "SimpleI2CSubDev: Device released\n");
    return 0;
}

// I2C探测函数
static int i2c_subdev_probe(struct i2c_client *client, const struct i2c_device_id *id) {
    struct i2c_subdev_data *data;
    int result;

    if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
        printk(KERN_ALERT "I2C functionality not supported\n");
        return -ENODEV;
    }

    data = kzalloc(sizeof(*data), GFP_KERNEL);
    if (!data)
        return -ENOMEM;

    data->client = client;
    i2c_set_clientdata(client, data);

    // 注册字符设备
    cdev_init(&client->dev.cdev, &fops);
    client->dev.cdev.owner = THIS_MODULE;
    result = cdev_add(&client->dev.cdev, MKDEV(240, 0), 1);
    if (result) {
        kfree(data);
        return result;
    }

    printk(KERN_INFO "SimpleI2CSubDev: Probe successful\n");
    return 0;
}

// I2C移除函数
static int i2c_subdev_remove(struct i2c_client *client) {
    struct i2c_subdev_data *data = i2c_get_clientdata(client);

    cdev_del(&client->dev.cdev);
    kfree(data);

    printk(KERN_INFO "SimpleI2CSubDev: Remove successful\n");
    return 0;
}

// I2C设备ID表
static const struct i2c_device_id i2c_subdev_id[] = {
    {DEVICE_NAME, 0},
    {}
};
MODULE_DEVICE_TABLE(i2c, i2c_subdev_id);

// I2C驱动结构体
static struct i2c_driver i2c_subdev_driver = {
    .driver = {
        .name = DEVICE_NAME,
    },
    .probe = i2c_subdev_probe,
    .remove = i2c_subdev_remove,
    .id_table = i2c_subdev_id,
};

// 模块初始化函数
static int __init i2c_subdev_init(void) {
    int ret;

    ret = i2c_add_driver(&i2c_subdev_driver);
    if (ret)
        printk(KERN_ERR "Failed to register I2C driver\n");

    printk(KERN_INFO "SimpleI2CSubDev: Module loaded\n");
    return ret;
}

// 模块退出函数
static void __exit i2c_subdev_exit(void) {
    i2c_del_driver(&i2c_subdev_driver);
    printk(KERN_INFO "SimpleI2CSubDev: Module unloaded\n");
}

module_init(i2c_subdev_init);
module_exit(i2c_subdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Linux I2C sub-device driver for educational purposes.");
MODULE_VERSION("0.1");





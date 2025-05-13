#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/of.h>

#define DEVICE_NAME "my_i2c_sensor"
#define I2C_ADDR 0x50

// 设备私有数据结构
struct my_sensor_data {
    struct i2c_client *client;
    struct gpio_desc *irq_gpio;
};

// 挂起设备
static int my_sensor_suspend(struct device *dev) {
    dev_info(dev, "Suspending sensor\n");
    return 0;
}

// 恢复设备
static int my_sensor_resume(struct device *dev) {
    dev_info(dev, "Resuming sensor\n");
    return 0;
}

// 在驱动结构体中添加电源管理回调
.static const struct dev_pm_ops my_sensor_pm = {
    .suspend = my_sensor_suspend,
    .resume = my_sensor_resume,
};

// 读取寄存器函数
static int sensor_read_reg(struct i2c_client *client, u8 reg, u8 *val) {
    return i2c_smbus_read_byte_data(client, reg);
}

// 写入寄存器函数
static int sensor_write_reg(struct i2c_client *client, u8 reg, u8 val) {
    return i2c_smbus_write_byte_data(client, reg, val);
}

// Probe 函数（设备初始化）
static int my_sensor_probe(struct i2c_client *client, const struct i2c_device_id *id) {
    struct my_sensor_data *data;

    data = devm_kzalloc(&client->dev, sizeof(*data), GFP_KERNEL);
    if (!data) return -ENOMEM;

    data->client = client;
    i2c_set_clientdata(client, data);

    // 初始化硬件（如复位、中断配置）
    dev_info(&client->dev, "Sensor probed at 0x%02x\n", client->addr);

    return 0;
}

// Remove 函数（设备释放）
static int my_sensor_remove(struct i2c_client *client) {
    dev_info(&client->dev, "Sensor removed\n");
    return 0;
}

// 设备树匹配表
static const struct of_device_id my_sensor_of_match[] = {
    { .compatible = "my,custom-sensor" },
    { }
};
MODULE_DEVICE_TABLE(of, my_sensor_of_match);

// I2C 驱动结构体
static struct i2c_driver my_sensor_driver = {
    .driver = {
        .name = DEVICE_NAME,
        .of_match_table = my_sensor_of_match,
    },
    .probe = my_sensor_probe,
    .remove = my_sensor_remove,
};

// 模块初始化
static int __init my_sensor_init(void) {
    return i2c_add_driver(&my_sensor_driver);
}

// 模块退出
static void __exit my_sensor_exit(void) {
    i2c_del_driver(&my_sensor_driver);
}

module_init(my_sensor_init);
module_exit(my_sensor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YourName");
MODULE_DESCRIPTION("Custom I2C Sensor Driver");

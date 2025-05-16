#include <linux/module.h>
#include <linux/init.h>
#include <linux/fb.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_reserved_mem.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>

#define DEVICE_NAME "simple_fb_dev"
#define DEFAULT_WIDTH  640
#define DEFAULT_HEIGHT 480
#define DEFAULT_BPP    32

// 设备私有数据结构
struct simple_fb_dev {
    struct fb_info *fb_info;
    void *fb_mem;       // 显存虚拟地址
    dma_addr_t fb_dma;  // 显存物理地址
    struct mutex lock;
};

// 打开设备
static int fb_open(struct fb_info *info, int user) {
    struct simple_fb_dev *dev = info->par;
    mutex_lock(&dev->lock);
    dev_info(info->dev, "Framebuffer opened");
    mutex_unlock(&dev->lock);
    return 0;
}

// 释放设备
static int fb_release(struct fb_info *info, int user) {
    struct simple_fb_dev *dev = info->par;
    mutex_lock(&dev->lock);
    dev_info(info->dev, "Framebuffer closed");
    mutex_unlock(&dev->lock);
    return 0;
}

// 设置显示参数
static int fb_set_par(struct fb_info *info) {
    struct simple_fb_dev *dev = info->par;
    struct fb_var_screeninfo *var = &info->var;

    dev_info(info->dev, "Setting resolution: %dx%d@%dbpp\n",
             var->xres, var->yres, var->bits_per_pixel);

    // 配置硬件寄存器（此处为模拟操作）
    return 0;
}

// 内存映射
static int fb_mmap(struct fb_info *info, struct vm_area_struct *vma) {
    struct simple_fb_dev *dev = info->par;
    return dma_mmap_coherent(dev->fb_info->dev, vma, dev->fb_mem, dev->fb_dma, info->fix.smem_len);
}

// 填充fb_ops结构体
static const struct fb_ops fb_ops = {
    .owner          = THIS_MODULE,
    .fb_open        = fb_open,
    .fb_release     = fb_release,
    .fb_set_par     = fb_set_par,
    .fb_mmap        = fb_mmap,
    .fb_fillrect    = cfb_fillrect,  // 使用内核默认的矩形填充
    .fb_copyarea    = cfb_copyarea,
    .fb_imageblit   = cfb_imageblit,
};

// 初始化设备
static int __init fb_probe(struct platform_device *pdev) {
    struct simple_fb_dev *dev;
    struct fb_info *fb_info;
    int ret;

    // 分配fb_info结构体
    fb_info = framebuffer_alloc(sizeof(*dev), &pdev->dev);
    if (!fb_info) return -ENOMEM;

    dev = fb_info->par;
    platform_set_drvdata(pdev, dev);
    mutex_init(&dev->lock);

    // 分配显存（示例使用DMA内存）
    dev->fb_mem = dma_alloc_coherent(&pdev->dev, DEFAULT_WIDTH*DEFAULT_HEIGHT*DEFAULT_BPP/8,
                                    &dev->fb_dma, GFP_KERNEL);
    if (!dev->fb_mem) {
        ret = -ENOMEM;
        goto err_alloc_mem;
    }

    // 初始化fb_info参数
    strcpy(fb_info->fix.id, DEVICE_NAME);
    fb_info->fix.type = FB_TYPE_PACKED_PIXELS;
    fb_info->fix.visual = FB_VISUAL_TRUECOLOR;
    fb_info->fix.line_length = DEFAULT_WIDTH * DEFAULT_BPP / 8;

    fb_info->var.xres = DEFAULT_WIDTH;
    fb_info->var.yres = DEFAULT_HEIGHT;
    fb_info->var.xres_virtual = DEFAULT_WIDTH;
    fb_info->var.yres_virtual = DEFAULT_HEIGHT;
    fb_info->var.bits_per_pixel = DEFAULT_BPP;
    fb_info->var.red.offset = 16;
    fb_info->var.red.length = 8;
    fb_info->var.green.offset = 8;
    fb_info->var.green.length = 8;
    fb_info->var.blue.offset = 0;
    fb_info->var.blue.length = 8;

    // 设置硬件操作函数
    fb_info->fbops = &fb_ops;

    // 注册Framebuffer
    ret = register_framebuffer(fb_info);
    if (ret < 0) {
        dev_err(&pdev->dev, "Failed to register framebuffer\n");
        goto err_register_fb;
    }

    dev_info(&pdev->dev, "Framebuffer registered at /dev/fb%d\n", fb_info->node);
    return 0;

err_register_fb:
    dma_free_coherent(&pdev->dev, DEFAULT_WIDTH*DEFAULT_HEIGHT*DEFAULT_BPP/8,
                     dev->fb_mem, dev->fb_dma);
err_alloc_mem:
    framebuffer_release(fb_info);
    return ret;
}

// 卸载设备
static int __exit fb_remove(struct platform_device *pdev) {
    struct simple_fb_dev *dev = platform_get_drvdata(pdev);
    struct fb_info *fb_info = dev->fb_info;

    unregister_framebuffer(fb_info);
    dma_free_coherent(&pdev->dev, DEFAULT_WIDTH*DEFAULT_HEIGHT*DEFAULT_BPP/8,
                     dev->fb_mem, dev->fb_dma);
    framebuffer_release(fb_info);
    return 0;
}

// 设备树匹配表
static const struct of_device_id fb_of_match[] = {
    { .compatible = "my,simple-fb" },
    { }
};
MODULE_DEVICE_TABLE(of, fb_of_match);

// 平台驱动结构体
static struct platform_driver fb_driver = {
    .probe  = fb_probe,
    .remove = fb_remove,
    .driver = {
        .name   = DEVICE_NAME,
        .of_match_table = fb_of_match,
    },
};

module_platform_driver(fb_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YourName");
MODULE_DESCRIPTION("Simple Framebuffer Driver");

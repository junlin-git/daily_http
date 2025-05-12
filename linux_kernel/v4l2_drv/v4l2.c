#include <linux/module.h>
#include <linux/init.h>
#include <linux/videodev2.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>

#define DEVICE_NAME "simple_v4l2_dev"
#define MAX_FRAME_SIZE 640 * 480 * 2  // 假设YUYV格式

// 设备结构体
struct simple_v4l2_dev {
    struct v4l2_device v4l2_dev;
    struct video_device vdev;
    struct mutex lock;
    char *buffer;
    int users;
};

static struct simple_v4l2_dev *g_dev;

// 打开设备
static int v4l2_open(struct file *file) {
    struct simple_v4l2_dev *dev = video_drvdata(file);
    mutex_lock(&dev->lock);
    if (dev->users) {
        mutex_unlock(&dev->lock);
        return -EBUSY;
    }
    dev->users++;
    mutex_unlock(&dev->lock);
    printk(KERN_INFO "Device opened");
    return 0;
}

// 释放设备
static int v4l2_release(struct file *file) {
    struct simple_v4l2_dev *dev = video_drvdata(file);
    mutex_lock(&dev->lock);
    dev->users--;
    mutex_unlock(&dev->lock);
    printk(KERN_INFO "Device closed");
    return 0;
}

// 查询设备能力
static int v4l2_querycap(struct file *file, void *priv,
                        struct v4l2_capability *cap) {
    strscpy(cap->driver, DEVICE_NAME, sizeof(cap->driver));
    strscpy(cap->card, "Simple V4L2 Device", sizeof(cap->card));
    cap->capabilities = V4L2_CAP_VIDEO_CAPTURE | V4L2_CAP_STREAMING;
    return 0;
}

// 设置视频格式
static int v4l2_s_fmt_vid_cap(struct file *file, void *priv,
                             struct v4l2_format *fmt)
{
    struct simple_v4l2_dev *dev = video_drvdata(file);
    if (fmt->fmt.pix.pixelformat != V4L2_PIX_FMT_YUYV)
        return -EINVAL;
    dev->format = *fmt;
    printk(KERN_INFO "Set format: %dx%d, YUYV", fmt->fmt.pix.width, fmt->fmt.pix.height);
    return 0;
}

// IOCTL操作集
static const struct v4l2_ioctl_ops v4l2_ioctl_ops = {
    .vidioc_querycap    = v4l2_querycap,
    .vidioc_enum_fmt_vid_cap = v4l2_enum_fmt_vid_cap,
    .vidioc_g_fmt_vid_cap = v4l2_g_fmt_vid_cap,
    .vidioc_s_fmt_vid_cap = v4l2_s_fmt_vid_cap,
    .vidioc_reqbufs       = v4l2_reqbufs,
    .vidioc_qbuf          = v4l2_qbuf,
    .vidioc_dqbuf         = v4l2_dqbuf,
    .vidioc_streamon      = v4l2_streamon,
    .vidioc_streamoff     = v4l2_streamoff,
};

// 文件操作结构体
static const struct file_operations v4l2_fops = {
    .owner          = THIS_MODULE,
    .open           = v4l2_open,
    .release        = v4l2_release,
    .unlocked_ioctl = video_ioctl2,
    .mmap           = v4l2_mmap,
    .poll           = v4l2_poll,
};

// 初始化设备
static int __init v4l2_init(void) {
    int ret;
    g_dev = kzalloc(sizeof(*g_dev), GFP_KERNEL);
    if (!g_dev)
        return -ENOMEM;

    // 初始化V4L2设备
    v4l2_device_init(&g_dev->v4l2_dev, NULL);
    g_dev->v4l2_dev.name = DEVICE_NAME;

    // 注册字符设备
    video_set_drvdata(&g_dev->vdev, g_dev);
    g_dev->vdev.fops = &v4l2_fops;
    g_dev->vdev.ioctl_ops = &v4l2_ioctl_ops;
    g_dev->vdev.release = video_device_release_empty;
    g_dev->vdev.v4l2_dev = &g_dev->v4l2_dev;

    ret = video_register_device(&g_dev->vdev, VFL_TYPE_VIDEO, -1);
    if (ret < 0) {
        v4l2_device_unregister(&g_dev->v4l2_dev);
        kfree(g_dev);
        return ret;
    }

    printk(KERN_INFO "V4L2 driver registered as /dev/video%d\n", g_dev->vdev.num);
    return 0;
}

// 卸载设备
static void __exit v4l2_exit(void) {
    video_unregister_device(&g_dev->vdev);
    v4l2_device_unregister(&g_dev->v4l2_dev);
    kfree(g_dev);
    printk(KERN_INFO "V4L2 driver unloaded\n");
}

module_init(v4l2_init);
module_exit(v4l2_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YourName");
MODULE_DESCRIPTION("Simple V4L2 Video Driver");

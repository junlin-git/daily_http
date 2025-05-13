#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

int main() {
    int fb_fd = open("/dev/fb0", O_RDWR);
    if (fb_fd == -1) {
        perror("Open FB failed");
        return -1;
    }

    // 获取屏幕信息
    struct fb_var_screeninfo vinfo;
    ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);

    // 映射显存
    long screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    char *fbp = mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if (fbp == MAP_FAILED) {
        perror("Mmap failed");
        close(fb_fd);
        return -1;
    }

    // 绘制红色矩形
    int x = 100, y = 100, w = 200, h = 150;
    for (int i = y; i < y+h; i++) {
        for (int j = x; j < x+w; j++) {
            int offset = (i * vinfo.xres + j) * vinfo.bits_per_pixel / 8;
            *((unsigned int*)(fbp + offset)) = 0x00FF0000;  // ARGB格式
        }
    }

    munmap(fbp, screensize);
    close(fb_fd);
    return 0;
}

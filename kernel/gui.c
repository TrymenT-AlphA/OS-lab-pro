/*
 *  linux/kernel/gui.c
 */

#include <asm/io.h>
#include <asm/segment.h>

#define vga_graph_memstart 0xA0000
#define vga_graph_memsize 64000
#define cursor_side 6
#define vga_width 320
#define vga_heignt 200
// 调用了以后显卡进入到Mode0x13图形模式中
int sys_initgraphics(void)
{
    // 建立像素点阵与显存之间的映射
    outb(0x05, 0x3CE);
    outb(0x40, 0x3CF); // 设定256色，且取出方式位移动拼装
    outb(0x06, 0x3CE);
    outb(0x05, 0x3CF); // 设定显存的地址区域，禁止字符模式
    outb(0x04, 0x3C4);
    outb(0x08, 0x3C5); // 设定将4个显存片连在一起

    // 设置屏幕分辨率
    outb(0x01, 0x3D4);
    outb(0x4F, 0x3D5); // 设置End Horizontal Display为79
    outb(0x03, 0x3D4);
    outb(0x82, 0x3D5); // 设置Display Enable Skew为0 

    outb(0x07, 0x3D4);
    outb(0x1F, 0x3D5); // 设置Vertical Display End第8、9位为1、0
    outb(0x12, 0x3D4);
    outb(0x8F, 0x3D5); // 设置Vertical Display End的低7位为0x8F
    outb(0x17, 0x3D4);
    outb(0xA3, 0x3D5); // 设置SLDIV = 1，将Scanline clock除以2

    outb(0x14, 0x3D4);
    outb(0x40, 0x3D5); // 设置DW = 1
    outb(0x13, 0x3D4);
    outb(0x28, 0x3D5); // 设置Offset = 40
    // 开始绘制屏幕
    outb(0x0C, 0x3D4);
    outb(0x0, 0x3D5);
    outb(0x0D, 0x3D4);
    outb(0x0, 0x3D5); // 将Start Address设置为0xA0000

    // 绘制背景
    int i;
    char * p;
    p = (char *)vga_graph_memstart;
    for (i = 0; i < vga_graph_memsize; ++i)
        *p++ = 3; // 将背景颜色设置为蓝绿色
    
    return 0;
}


struct rect {
    long color; // 颜色
    long x; // 左下点x坐标
    long y; // 左下点y坐标
    long dx; // 矩形x边的长度
    long dy; // 矩形y边的长度
};

int sys_paintrect(struct rect * rect)
{
    int i, j;
    char * p;
    // 将rect中的数据写入变量中
    long color = get_fs_long(&rect->color);
    long x = get_fs_long(&rect->x);
    long y = get_fs_long(&rect->y);
    long dx = get_fs_long(&rect->dx);
    long dy = get_fs_long(&rect->dy);
    // 超出边界就忽略，在x~x+dx,y~y+dy上涂色
    for (i = x; i < x+dx; ++i) if (0 <= i && i < vga_width)
        for (j = y; j < y+dy; ++j) if (0 <= j && j < vga_heignt){
            p = (char *)vga_graph_memstart + vga_width*j + i;
            *p = color;
        }
    return 0;
}

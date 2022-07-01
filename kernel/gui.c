/*
 *  linux/kernel/gui.c
 */

#include <asm/io.h>

#define vga_graph_memstart 0xA0000
#define vga_graph_memsize 64000
#define cursor_side 6
#define vga_width 320
#define vga_heignt 200

int sys_initgraphics(void)
{
    outb(0x05, 0x3CE);
    outb(0x40, 0x3CF); // 设定256色，且取出方式位移动拼装
    outb(0x06, 0x3CE);
    outb(0x05, 0x3CF); // 设定显存的地址区域，禁止字符模式
    outb(0x04, 0x3C4);
    outb(0x08, 0x3C5); // 设定将4个显存片连在一起

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

    outb(0x0C, 0x3D4);
    outb(0x0, 0x3D5);
    outb(0x0D, 0x3D4);
    outb(0x0, 0x3D5); // 将Start Address设置为0xA0000

    // int i;
    // char * p = vga_graph_memstart;
    // for (i = 0; i < vga_graph_memsize; ++i)
    //     *p++ = 3;
    
    // return 0;
}

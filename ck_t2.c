#define __LIBRARY__
#include<unistd.h>
#include<stdlib.h>

struct rect {
    long color;
    long x;
    long y;
    long dx;
    long dy;
};

_syscall0(int,initgraphics);
_syscall1(int,paintrect,struct rect *,rect);

int main(void)
{
    struct rect my_rect;
    initgraphics();
    my_rect.color = 12;
    my_rect.x = 50;
    my_rect.y = 50;
    my_rect.dx = 10;
    my_rect.dy = 10;
    paintrect(&my_rect);
}

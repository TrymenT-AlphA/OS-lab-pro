#define __LIBRARY__
#include<unistd.h>

_syscall0(int,initgraphics)

int main(void)
{
    initgraphics();
}

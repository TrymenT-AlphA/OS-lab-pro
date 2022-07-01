/*
 *  Flappy Bird
 */
#include <unistd.h>

struct message {
    long mid; /* 消息的id */
    long pid; /* 消息的目标进程，如果是当前进程，设置为-1 */
} message;

struct rect {
    long color;
    long x;
    long y;
    long dx;
    long dy;
};

struct user_timer{
	long init_jiffies;
	long jiffies;
	int type; /* 类型为1表示只定义了一次闹钟
			     类型为0表示定义了无数次闹钟 */
	int pid; /* 哪个进程创建的定时器 */
	struct user_timer * next;
};

#define MSG_MOUSE_LEFT_DOWN 1
#define MSG_MOUSE_RIGHT_DOWN 2
#define MSG_MOUSE_CENTER_DOWN 3
#define MSG_USER_TIMER 4
#define TYPE_USER_TIMER_INFTY 5
#define TYPE_USER_TIMER_ONCE 6

_syscall0(int,initgraphics);
_syscall1(void,get_message,struct message *,msg);
_syscall1(int,paintrect,struct rect *,rect);
_syscall2(int,timercreate,long,ms,int,type);

#define BIRD_X 120
#define BIRD_Y 100
#define BIRD_WIDTH 10
#define BIRD_HEIGHT 8

struct obj {
    int x; /* 对象坐标(x,y) */
    int y;
    int dx; /* 对象宽高 */
    int dy;
};

struct obj bird;

int init_bird(void)
{
    bird.x = BIRD_X;
    bird.y = BIRD_Y;
    bird.dx = BIRD_WIDTH;
    bird.dy = BIRD_HEIGHT;
    return 0;
}

#define MAX_OBJS 20
struct obj obj_que[MAX_OBJS];
int fron, rear;

int init_obj_que(void)
{
    fron = rear = 0;
    return 0;
}

int push_obj(struct obj * obj)
{

}

int pop_obj(void)
{

}

#define CLOCK_TRIGGER 10 /* ms */
#define DROP_PER_TRIGGER 1 /* 像素 */

int main(void)
{
    /* Flappy Bird框架 */
    struct message msg;
    timercreate(CLOCK_TRIGGER, TYPE_USER_TIMER_INFTY);
    while(1)
    {
        get_message(&msg);
        if (get_message(&msg))
        {
            switch(msg.mid)
            {
            case MSG_MOUSE_LEFT_DOWN:
            /* 更新小鸟位置 */
            break;
            case MSG_USER_TIMER:
            /* 绘制背景 */
            /* 绘制障碍物和小鸟 */
            /* 判断游戏是否结束 */
            goto GAME_OVER;
            /* 超出屏幕的障碍物出队，生成新障碍物入队 */
            break;
            }
        }
        else continue;
    }
GAME_OVER:
    /* 绘制游戏结束画面 */
    return 0;
}
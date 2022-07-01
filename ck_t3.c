#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>

struct message {
    int mid; /* 消息的id */
    int pid; /* 消息的目标进程，如果是当前进程，设置为-1 */
} message;

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

_syscall1(void,get_message,struct message *,msg);
_syscall2(int,timercreate,long,ms,int,type);

struct message msg;

int main()
{
    timercreate(1000,TYPE_USER_TIMER_INFTY);
    while(1)
    {
        get_message(&msg);
        printf("msg.mid:%d\n", msg.mid);
        if (msg.mid > 0)
        {
            switch(msg.mid)
            {
            case MSG_USER_TIMER:
                printf("MSG_USER_TIMER\n");
                break;
            case MSG_MOUSE_LEFT_DOWN:
                printf("MSG_MOUSE_LEFT_DOWN\n");
                break;
            case MSG_MOUSE_RIGHT_DOWN:
                printf("MSG_MOUSE_RIGHT_DOWN\n");
                break;
            }
        }
    }
}
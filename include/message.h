#ifndef __MESSAGE_H__
#define __MESSAGE_H__
struct message{
    int mid; // 消息的id
    int pid; // 消息的目标进程，如果是当前进程，设置为-1
    struct message *next;
} message;
struct message msg_que[1024];
extern void post_message(int type);
extern void sys_get_message(message *msg);

// mid取值
#define MSG_MOUSE_LEFT_DOWN 1
#define MSG_MOUSE_RIGHT_DOWN 2
#define MSG_MOUSE_CENTER_DOWN 3

#endif
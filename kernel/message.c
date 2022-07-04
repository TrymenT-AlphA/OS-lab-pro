#include<linux/sched.h>
#include<asm/segment.h>
#include<message.h>
// 1024为消息队列长度，首尾相通
struct message msg_que[1024];
// msg_que_fron为队列头，msg_que_rear为队列尾
unsigned int msg_que_fron = NULL, msg_que_rear = NULL;
// 将消息存入msg_que中
void post_message(int type){
	// 如果没有存满消息
    if (msg_que_rear != msg_que_fron - 1) {
		struct message msg;
		msg.mid = type;
		msg.pid = current->pid;
		msg_que[msg_que_rear] = msg;
		msg_que_rear = (msg_que_rear + 1) % 1024;
	}
}
// 将消息取出放在msg中
void sys_get_message(struct message *msg) {
    struct message tmp;
	// 如果队列里没有消息
	if(msg_que_rear == msg_que_fron){
		put_fs_long(-1,&msg->mid);
		put_fs_long(-1,&msg->pid);
		return;
	}
	
	tmp = msg_que[msg_que_fron];
	msg_que[msg_que_fron].mid = 0;
	msg_que_fron = (msg_que_fron + 1) % 1024;;
	put_fs_long(tmp.mid,&msg->mid);
	put_fs_long(current->pid,&msg->pid);
}
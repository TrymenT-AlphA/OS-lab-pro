## 一、实验目的和内容

（一）实验目的：在Linux0.11中能驱动鼠标，实现一个简单的图形接口，并实现一个简单的可视化应用来完成对项目的验证。

（二）实验内容：在Linux0.11上实现鼠标驱动和图形用户接口，并利用这个图形用户接口实现一个图形应用程序，该程序能显示图形元素，能接受并响应鼠标的点击操作。

1. ##### 驱动鼠标

鼠标驱动的核心是编写鼠标中断处理程序，将鼠标输入放入一个缓冲队列，再由事件（消息）驱动子系统从该队列取出鼠标事件并处理：如果是鼠标移动，则在新的位置上重新绘制光标；如果是鼠标点击，则根据当前进程注册的鼠标点击函数相应事件。

2. ##### 让显示器在图形模式下工作

在图形模式下，输出到屏幕的不是字符而是像素。向显存的合适位置写一个像素，然后显卡会自动地将这个像素点输出到屏幕上。显卡上有一个控制寄存器，能够设置显卡的工作方式。

3. ##### 设计并实现一个事件（消息）驱动系统

顾名思义，一个事件（消息）驱动系统就是在操作系统中有一个子系统一直在查看是否有事件发生。当发现有事件发生时，该子系统会根据事件的具体含义做出相应的响应。显然，可以写一个一直循环的消息处理程序来查看是否有事件发生。

4. ##### 实现一个可视化应用程序

实现一个可视化应用：这个应用程序应该能够在屏幕上绘制图形元素；应该能捕捉用户的鼠标点击动作，并能对点击动作做出相应响应；响应结果应该在屏幕上实时展现出来，以方便项目验证。

## 二、操作方法与实验步骤

#### 	(一) 鼠标的驱动

##### 	1.新建汇编文件mouse.s,用汇编语言实现mouse_interrupt，修改Makefile文件。

鼠标对应的数据寄存器端口地址是0x60，通过inb命令将该数据寄存器中存放的一个字节信息读入到寄存器AL（包含用户对鼠标操作的全部信息）中。

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image002.jpg)

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image004.jpg)

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image006.jpg)

2. ##### 键盘控制器i8042和中断控制器8259A

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image008.jpg)

##### 3.鼠标输入数据的解码

###### （1）各个变量的含义

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image010.jpg)

###### （2） readmouse函数主体

   主要功能是读取以下信息并处理，计算鼠标的位置。

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image012.jpg)

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image014.jpg)

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image016.jpg)

#### (二) 显示器的图形工作模式

1. ##### sys_initgrahics系统调用

功能：调用了以后显卡进入到Mode0x13图形模式中

2. ##### 修改sys.h,unistd.h,main.c文件，添加sys_initgrahics系统调用

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image018.jpg)

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image020.jpg)

 

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image022.jpg)

3. ##### 在gui.c中实现sys_initgraphics

​	(1)启动图形模式:找到显示器控制器(实际上就是我们常说的显卡)中合适的寄存器,将显示器设置成以像素为基础的工作方式。

​	(2)设置屏幕分辨率:找到想要的像素点阵的行数和列数(实际上就是我们常说的屏幕分辨率),通过向显卡寄存器中写人合适的参数来得到这样的屏幕分辨率。

​	(3)完成像素点阵和显存的映射 。![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image024.jpg)

4. ##### 修改Makefile处理gui.c

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image026.jpg)

#### (三) 消息驱动框架的实现

​	(1)定义消息队列数据结构;

​	(2)实现函数post_message将消息放入到消息队列中;

​	(3)提供函数实现从消息队列中取出消息,即实现系统调用get_message对应的内核函数sys get message; 

​	(4)在事件发生时,能根据事件含义创建一个新消息并放人消息队列,实际上就是在操作系统代码中找到合适的位置,创建消息并调用 post message

1. ##### 增加sys_get_message系统调用

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image028.jpg)

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image030.jpg)

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image032.jpg)

2. ##### 增加message.h文件，用作提供message的数据结构和各类变量定义

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image034.jpg)

3. ##### 增加message.c文件，实现sys_get_message函数及post_message函数

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image036.jpg)

4. ##### 修改Makefile，编译message.c文件

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image038.jpg)

#### (四) 可视化应用程序的设计与实现

为了让屏幕不断变化,应用程序创建一个定时器,该定时器是操作系统内核中的一个对象,每隔一定的时间间隔(由用户指定)就会给应用程序发一个定时消息,应用程序在得到这个定时消息以后会改变屏幕上各个图形元素的信息,并重画整个屏幕,使整个屏幕活动起来。

1. ##### 添加定时器user_timer数据结构

##### ![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image040.jpg)

2. ##### 增加系统调用sys_timercreate（函数已更名为sys_timercreate）

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image042.jpg)

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image044.jpg)

3. ##### 实现sys_timercreate

硬件会在每一个滴答 (Linux 0.11将一个滴答设置为10 ms) 处向CPU发出一次时钟中断,操作系统在时钟中断处理函数do_timer() 中会扫描user_timer_list中的所有定时器,将每个定时器中的jiffies值减1。如果每个定时器的jiffies减为0,说明用户进程定义的定时器到时,时钟中断处理程序就产生一个定时器到时消息,用post_message发到系统消息队列中。![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image046.jpg)

4. ##### 增加和实现绘制矩形接口调用sys_paintrect函数

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image048.jpg)

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image050.jpg)

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image052.jpg)

 

#### (五) 测试函数的实现

1. ##### ck_t1.c测试文件

功能：调用并测试sys_initgrahics系统调用是否正确及正确运行，其运行结果应当是整个Bochs同一颜色

函数实现：

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image054.jpg)

测试结果：

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image056.jpg)

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image058.jpg)

2. ##### ck_t2.c测试文件

功能：在ck_t1.c的基础上使用了paintreect函数，将ck_t1.c测试结果中的一定大小的颜色块颜色设定为红色

实现代码：

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image060.jpg)

测试结果：

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image062.jpg)

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image064.jpg)

#####        3. ck_t3.c测试文件

功能：检测鼠标按键响应，按下左键打印信息MSG_MOUSE_LEFT_DOWN,按下右键打印信息MSG_MOUSE_RIGHT_DOWN

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image066.jpg)

测试结果：

![img](file:///C:/Users/16207/AppData/Local/Temp/msohtmlclip1/01/clip_image068.gif)

4. ##### 综合:FlappyBird.c的实现

将上面描述的各个部分综合起来就是Flappy Bird小游戏要实现的核心工作:

(1) 创建所有的屏幕对象;

(2) 创建一个定时器;

(3) 产生一个无限循环;

(4) 在循环中利用get_message系统调用逐个取出发给自己的系统消息;

(5) 如果取出的消息是定时器到时消息,就更新屏幕对象位置、判断游戏是否失败,并重画整个屏幕;

(6) 如果取出的消息是鼠标左键消息,就更新小鸟位置。

```
/* Flappy Bird框架 */
    timercreate(CLOCK_TRIGGER, TYPE_USER_TIMER_INFTY); /* 创建定时器 */
    initgraphics(); /* 进入图形模式 */
    if (init_all() < 0) /* 初始化游戏 */
        return -1;
    while(1) { 
        get_message(&msg); /* 从消息队列取出一个消息 */
        if (msg.mid < 0)
            continue;
        switch(msg.mid) { /* 消息处理 */
        case MSG_MOUSE_LEFT_DOWN:
            bird.y -= UP_PER_CLICK; /* 芜湖，起飞 */
            break;
        case MSG_USER_TIMER:
            if (paint_all() < 0) /* 更新画面 */
                return -1;
            for (i = fron; i != rear; i = (i+1)%MAX_BARRIER) /* 吃饱喝饱，一路走好 */
                if (barrier[i].x < bird.x+bird.dx && bird.x < barrier[i].x+barrier[i].dx)
                    if (barrier[i].y < bird.y+bird.dy && bird.y < barrier[i].y+barrier[i].dy)
                        goto GAME_OVER;
            bird.y += DROP_PER_TRIGGER; /* 地心引力 */
            for (i = fron; i != rear; i = (i+1)%MAX_BARRIER) /* 动的不是鸟，是障碍物 */
                barrier[i].x -= LEFT_PER_TRIGGER;
            if (fron == rear) { /* 没有障碍物，直接添加障碍物 */
                obj.dx = BARRIER_WIDTH;
                obj.dy = BARRIER_HEIGHT;
                obj.x = VAG_WIDTH;
                obj.y = 0;
                push_obj(&obj);
            }
            else { /* 添加新障碍物，删除旧障碍物 */
                if (barrier[(rear+MAX_BARRIER-1)%MAX_BARRIER].x+barrier[(rear+MAX_BARRIER-1)%MAX_BARRIER].dx+BARRIER_INTERVAL <= VAG_WIDTH) {
                    obj.dx = BARRIER_WIDTH;
                    obj.dy = BARRIER_HEIGHT;
                    obj.x = VAG_WIDTH;
                    if (barrier[(rear+MAX_BARRIER-1)%MAX_BARRIER].y)
                        obj.y = 0;
                    else
                        obj.y = VGA_HEIGHT - obj.dy;
                    push_obj(&obj);
                }
                if (barrier[fron].x+barrier[fron].dx <= 0)
                    pop_obj(NULL);
            }
            break;
        }
    }

GAME_OVER: /* 完了，这下真寄了 */
    paint_background(GAME_OVER_COLOR);
    return 0;
```



5. ##### 实现过程中出现的问题

(1) 在用户态绘图会发生花屏现象，即initgrahic()产生的蓝绿图像中掺杂了许多颜色，上部分为有颜色的部分，下部分为黑色的部分。问题在于在用户态进行绘图，而在内核态进行绘图调用sys_initgrahic()就可以恢复正常。

​      (2) 鼠标响应延迟过大,一次按键需要较长时间来响应，所以最后的可视化应用程序无法做到即按即动，与正常的游戏还有很大的区别。尚不知道这种现象的原因。
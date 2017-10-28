/*******************************************************************************

    This file is part of the debug.c.
    Copyright wilddog.com
    All right reserved.

    File:    observer_queue.h

    description : 实现 类 epoll 推送型队列。
    notice: 目前list 的 node 均是由单一线程创建，删除，释放。所以仅在修改，删除时需要上锁，读取不需要。

    TIME LIST:
    CREATE  skyli   2017-05-06 13:47:55

*******************
************************************************************/
#include "up2p.h"

#ifndef _observer_queue_h

#define _observer_queue_h

typedef enum {
    EVENT_NON = 0,
    EVENT_RECV_DATA,

    EVENT_MAX
}Observer_Event;

typedef struct ELEMENT_ST{
    Observer_Event observer_event;
    Observer_Event event_happen;
    void *p_user;
    void *p_user_data;
    struct Element_ST *next;
} Element_ST;
typedef struct LIST_CT{
    Element_ST *p_head;
    int el_count;
    int el_max;
}List_ST;


// 初始化 queue 最大值，max queue 应该根据服务器的内存和实际的业务确定。
int observer_creat(int maxqueue);

Element_ST *observer_list_head_get(void);
int observer_list_lock(void);
void observer_list_unlock(void);
// 把 buffer 指针加入 observer queue.
void *observer_add(u8 envent,void *p_user,void *p_user_data);

int observer_delete(Element_ST *el_del);
// 查询 event 是否触发
int observer_event_happent(void *el,const u8 event);

// block 等待 event 发生，并把需要的数据写入 userdata
int observer_event_wait(void *el, u8 event,const int timeout_ms,void *pp_user_data);

// 订阅事件，并等待。
int observer_event_add_wait( u8 event,const int timeout_ms,void *p_user,void *p_user_data);
int observer_distory(void);

// unit test code 
#if 1
void unittest_observer_queue(void);
#endif

#endif// _observer_queue_h
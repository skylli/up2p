/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    vlink.h

   1. mamage socket,and link connection.

    TIME LIST:
    CREATE  skyli   2017-10-23 17:30

*******************************************************************************/
#ifndef _vlink_h_
#define _vlink_h_


#include "up2p.h"
#include "observer_queue.h"
#include "protocol_up2p.h"

typedef struct{
    int fd;
    UDP_INFO info;
    Session_ST session;
} LINK_ST;
typedef struct{
    void *p_package;
    u32 len;
} PACKAGE_ST;

// 建立session
LINK_ST *link_open(int fd,u16 part,u32 mydevid0,u32 mydevid1,u32 dstdevid0,u32 dstdevid1,u32 dstip,u32 dstport,u32 aeskey0,u32 aeskey1);
// 发送请求
int link_request_send(LINK_ST *p_link,u32 cmd,const u8 *data,int len);
// 注册事件,当 event 发生，对应的数据会存放在 p_package 指针中。
void *link_event_register(LINK_ST *p_link,Observer_Event event,u8 *p_package);
// 查询事件是否发生
int link_event_happend(void *p_el,Observer_Event event);
//注册事件的监听并 阻塞等待事件的发生
int link_event_wait_block(LINK_ST *p_link,Observer_Event event,u8 *p_package, u32 timeout);

int link_request_wait_response(LINK_ST *p_link,u32 req_cmd,const u8 *data,int len,u32 resp_cmd,u8 **pp_recv,u32 *p_len_recv,int timeout_ms);


#if 1
int unittest_link(void);

#endif
#endif //  end _vlink_h_

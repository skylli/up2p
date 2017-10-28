/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    vlink.c

   1.mamage link cennction.

    TIME LIST:
    CREATE  skyli   2017-10-23 17:30

*******************************************************************************/
#include "up2p.h"
#include "utlist.h"
#include "util.h"
#include "vlink.h"

// 发送请求
int link_request_send(LINK_ST *p_link,u32 cmd,const u8 *data,int len){
    // pack protocol package
    int len_send = 0, ret = 0;
    PRO_UP2P_HEAD *p_pack= NULL;

    if(cmd > CMD_DATA)
        p_pack = protocol_up2p_encry_pack_alloc(&p_link->session,cmd,&len_send,data,len);
    else
        p_pack = protocol_up2p_pack_alloc(&p_link->session,cmd,&len_send,data,len);

    if( p_pack== NULL){
        log_level(U_LOG_ERROR,"protocol pack failt");
    }
    ret = socket_udp_send(p_link->fd,p_pack,len_send,&p_link->info);

    ufree(p_pack);
    return ret;
}
// 1、发送 req_cmd.
// 2、等待回应，并每隔 INTERVAL_RETRANSMIT_MS 重发.
// 3、收到回应会 malloc 和 cpy 回应的数据并输出，所以由外界释放.
int link_request_wait_response(LINK_ST *p_link,u32 req_cmd,const u8 *data,int len,u32 resp_cmd,u8 **pp_recv,u32 *p_len_recv,int timeout_ms){
    // pack protocol package
    int len_send = 0, ret = 0,len_data = 0;
    u8 *p_data = NULL;
    PACKAGE_ST package_recv;
    PRO_UP2P_HEAD *p_pack= NULL;
    int timeout = 0;
    struct timeval star_tm,pre_tm,current_tm;
    // alloc send package.
    if(req_cmd > CMD_DATA)
        p_pack = protocol_up2p_encry_pack_alloc(&p_link->session,req_cmd,&len_send,data,len);
    else
        p_pack = protocol_up2p_pack_alloc(&p_link->session,req_cmd,&len_send,data,len);

    if( p_pack== NULL){
        log_level(U_LOG_ERROR,"protocol pack failt");
        return -1;
    }
    
    memset(&package_recv,0,sizeof(PACKAGE_ST));
    
    // register event in case miss the response data.
    void *p_node_event = observer_add(EVENT_RECV_DATA,p_link,&package_recv);

    if( p_node_event == NULL){
        ufree(p_pack);
        p_pack = NULL;
        return -1;
    }

    // mark current time .
    gettimeofday(&current_tm,NULL);
    gettimeofday(&pre_tm,NULL);
    gettimeofday(&star_tm,NULL);    

    //star send package.
    ret = socket_udp_send(p_link->fd,p_pack,len_send,&p_link->info);
    timeout = (timeout_ms > 0)? timeout_ms:MAX_RERANSMITS_TIMEOUT_ms;

    do{
        
    // check response.
        if(observer_event_happent(p_node_event,EVENT_RECV_DATA) && package_recv.len  && package_recv.p_package){
            // receive buffer.
            log_level(U_LOG_INFO, "receive response data");
            // check it 
            if(protocol_up2p_filter(p_link,package_recv.p_package,package_recv.len)){
                log_level(U_LOG_ERROR,"source device id was not match");
                // drop it.
                ufree(package_recv.p_package);
                package_recv.p_package = NULL;
                }
            // distpathch it 
            p_data = protocol_up2p_data_alloc(p_link,&len_data,package_recv.p_package,package_recv.len);
            // free it. we don't need it blew.
            ufree(package_recv.p_package);
            package_recv.p_package = NULL;
            // 返回 0 标记已经接收到回应.
            ret = 0;
            break;
        }        
        //send out request.
        // check time to retransmit 
        gettimeofday(&current_tm,NULL);
        if(_TM_MSEC(current_tm.tv_sec,current_tm.tv_usec) - _TM_MSEC(pre_tm.tv_sec,pre_tm.tv_usec) > INTERVAL_RETRANSMIT_MS){
            
                gettimeofday(&pre_tm,NULL);
                log_level(U_LOG_DEBUG,"retransmit one time");
                ret = socket_udp_send(p_link->fd,p_pack,len_send,&p_link->info);
        }
    }while(_TM_MSEC(current_tm.tv_sec,current_tm.tv_usec) - _TM_MSEC(star_tm.tv_sec,star_tm.tv_usec) < MAX_RERANSMITS_TIMEOUT_ms);

    if(p_data && len_data ){
        log_level(U_LOG_DEBUG,"receive data %s",p_data);
        *pp_recv = p_data;
        *p_len_recv = len_data;
    }
    // free send package.
    ufree(p_pack);
    return ret;
}

// 建立session
LINK_ST *link_open(int fd,u16 part,u32 mydevid0,u32 mydevid1,u32 dstdevid0,u32 dstdevid1,u32 dstip,u32 dstport,u32 aeskey0,u32 aeskey1){
    int ret = 0;
    LINK_ST *p_link = (LINK_ST*)umalloc(sizeof(LINK_ST));
    u8 *p_recv = NULL;
    int len_recv = 0;
    if(!p_link){
        log_level(U_LOG_ERROR,"malloc failt");
        return NULL;
    }
    p_link->fd = fd;
    p_link->info.dip = dstip;
    p_link->info.dport = dstport;
    p_link->session.mydevid0 = mydevid0;
    p_link->session.mydevid1 = mydevid1;
    p_link->session.dstdevid0 = dstdevid0;
    p_link->session.dstdevid1 = dstdevid1;
    p_link->session.aeskey0 = aeskey0;
    p_link->session.aeskey1 = aeskey1;

    // get session token 
    // link_request_wait_response(LINK_ST *p_link,u32 req_cmd,const u8 *data,int len,u32 resp_cmd,u8 **pp_recv,u32 *p_len_recv,int timeout_ms)；
    ret = link_request_wait_response(p_link,CMD_GET_TOKEN,NULL,0,CMD_GET_TOKEN_ACK,&p_recv,&len_recv,0);
    if(ret !=0 ){
        log_level(U_LOG_WARN,"link open failt!! ");
        ufree(p_link);
        return NULL;
    }
    
    // 获取token
    if(p_recv == NULL || len_recv == 0){
        log_level(U_LOG_WARN,"can't get session token.");
        ufree(p_link);
        return NULL;
    }
    p_link->session.token = *(u32*)p_recv;
    log_level(U_LOG_DEBUG,"get session token %x",p_link->session.token);
    
    return p_link;
    //ret = link_request_send(p_link,CMD_GET_TOKEN,NULL,0);
    
}

// 注册事件,当 event 发生，对应的数据会存放在 p_package 指针中。
void *link_event_register(LINK_ST *p_link,Observer_Event event,u8 *p_package){
    return observer_add(EVENT_RECV_DATA,p_link,p_package);
}
// 查询事件是否发生
int link_event_happend(void *p_el,Observer_Event event){

    return observer_event_happent(p_el,event);

}
//注册事件的监听并 阻塞等待事件的发生
int link_event_wait_block(LINK_ST *p_link,Observer_Event event,u8 *p_package, u32 timeout){
    
}

    // 1、接收包的协议是匹配
    // 2、遍历链表，是否匹配的 devid
    // 3、 匹配则把 data 挂到对应的节点
    // 4、设置事件。
    // 4、若不匹配，接到到底额包直接丢弃。
// 处理接收
int link_recv_handle(const u8 *p_recv,int len_recv,UDP_INFO *p_info){
    Session_ST *p_s = NULL;

    // 1、接收包的协议是匹配
    if(protocol_up2p_magic_notMatch(p_recv,len_recv))
        return 0;
    // 2、遍历链表，是否匹配的 devid
    Element_ST *head,*el,*tmp;
    
    head = observer_list_head_get();
    // 锁住链表
    if(observer_list_lock() < 0){
        log_level(U_LOG_ERROR, "lock list failt");
        return 0;
    }
#if 1
    LL_FOREACH_SAFE(head, el, tmp){
    
         p_s = el->p_user;
        // 3、 匹配则把 data 挂到对应的节点，并把节点摘掉。
        if(protocol_up2p_devid_match(p_s->mydevid0,p_s->mydevid1,p_recv)){
                // push 
                if(el->p_user_data){
                    // alloc 
                    PACKAGE_ST *p_package = (PACKAGE_ST*) el->p_user_data;

                    p_package->p_package = umalloc(len_recv);
                    if(!p_package->p_package){
                        log_level(U_LOG_ERROR,"malloc failt !!");
                        return -1;
                    }
                    
                    p_package->len = len_recv;
                    memcpy(p_package->p_package,p_recv,len_recv);
                }
                 // 4、设置事件。
                 el->event_happen = el->observer_event;
                
        }
    }
    #endif
    //解锁
    observer_list_unlock();
    // 4、若不匹配则丢弃,直接返回。
    return 0;
}
#if 1
#define _TEST_LISTEN_PORT (12331)
#define _TEST_REMOTEF_PORT  (UP2PC_PORT)
#define _TEST_HOST ("127.0.0.1")

//recv_proc(const char *recv_data, int len, UDP_INFO *info)
int fd = -1;
static void _test_recv_proc_thread()
{

    while(1){
            int len;
            UDP_INFO info;
            char recvbuf[MAX_NET_PACKAGE_SIZE];
            
            memset(&info,0,sizeof(UDP_INFO));
            memset(recvbuf,0,MAX_NET_PACKAGE_SIZE);

            if(socket_udp_recv(fd,recvbuf,MAX_NET_PACKAGE_SIZE,&len, &info) > 0){
                link_recv_handle(recvbuf, len, &info);
            }
   }        
}

// 1、建立socket 监听 port.
// 2、发送命令.
// 3、 释放
int unittest_link(void){

    fd = socket_listenbin_creat(_TEST_LISTEN_PORT);
    logLevel_set(0);
    if(fd == -1 ){
        log_level(U_LOG_ERROR,"creat socket failt !!");
        return -1;
    }
    // 创建线程
    pthread_t thread;
    pthread_create(&thread, NULL, (void *)_test_recv_proc_thread, NULL);

    sleep(1);
    // 获取 host ip
    u32 host_ip = socket_gethost_ip(_TEST_HOST);
    log_level(U_LOG_DEBUG,"host ip =  %d",host_ip);
    // 发送请求。
    //LINK_ST *link_open(int fd,u16 part,u32 mydevid0,u32 mydevid1,u32 dstdevid0,u32 dstdevid1,u32 dstip,u32 dstport,u32 aeskey0,u32 aeskey1){
    LINK_ST *p_link = link_open(fd,0,1,1,2,2,host_ip,_TEST_REMOTEF_PORT,0,0);
    if(p_link == NULL){
        log_level(U_LOG_ERROR,"link open failt !!");
        return -1;
    }
    else log_level(U_LOG_INFO,"link open successfully!!");

    return 0;
}
#endif // test 

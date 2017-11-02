/*******************************************************************************

    This file is part of the debug.c.
    Copyright wilddog.com
    All right reserved.

    File:    observer_queue.c

    description : 实现 类 epoll 推送型队列。
    notice: 目前list 的 node 均是由单一线程创建，删除，释放。所以仅在修改，删除时需要上锁，读取不需要。

    TIME LIST:
    CREATE  skyli   2017-05-06 13:47:55

*******************
************************************************************/
#include "observer_queue.h"
#include "util.h"
#include <sys/time.h>
#include <unistd.h>
#include "utlist.h"
#include <pthread.h>

pthread_mutex_t mutex_list;



static List_ST list_ct;


// 初始化 queue 最大值，max queue 应该根据服务器的内存和实际的业务确定。
int observer_creat(int maxqueue){

    memset(&list_ct,0,sizeof(List_ST));
    
    list_ct.el_max = maxqueue;
    // init mutex
    pthread_mutex_init(&mutex_list, NULL);
    log_level(U_LOG_INFO,"observer list init.");
    return 0;
}
int observer_queue_element_count_get(void){
    return list_ct.el_count;
}
Element_ST *observer_list_head_get(void){
    return list_ct.p_head;
}
int observer_list_lock(void){
    if(pthread_mutex_lock(&mutex_list) !=0){
        log_level(U_LOG_ERROR, "lock list error");
        return -1;
    }

    return 0;
}
void observer_list_unlock(void){
    pthread_mutex_unlock(&mutex_list);
}
// 把 buffer 指针加入 observer queue.
void *observer_add(u8 envent,void *p_user,void *p_user_data){

    if(list_ct.el_count > list_ct.el_max){
        
        log_level(U_LOG_WARN," count %d max count %x ,observer queue was overflow!",list_ct.el_count,list_ct.el_max);
        return NULL;
    }
    
    // alloc new element
    Element_ST *el_new = (Element_ST*)malloc(sizeof(Element_ST));
    if(!el_new){
        log_level(U_LOG_ERROR, "alloc failt !!");
        return NULL;
        }
   // memset(el_new,0,sizeof(Element_ST));
    el_new->observer_event = envent;
    el_new->event_happen = EVENT_NON;
    // we need address to register.
    el_new->p_user_data = p_user_data;
    el_new->p_user = p_user;

    //log_level(U_LOG_DEBUG, " p_user  %p add  %p",p_user,el_new->p_user);
    // unlock
    if(pthread_mutex_lock(&mutex_list) !=0){
        log_level(U_LOG_ERROR, "lock list error");
        free(el_new);
        return NULL;
    }
    
    LL_APPEND(list_ct.p_head,el_new);
    list_ct.el_count++;
    // unlock it.
    pthread_mutex_unlock(&mutex_list);
    
    //log_level(U_LOG_DEBUG,"observer event node %p added", el_new);
    return el_new;
}

int observer_delete(Element_ST *el_del){
    if(!el_del)
        return -1;

        // todo wait list unlock
    if(pthread_mutex_lock(&mutex_list) !=0){
        log_level(U_LOG_ERROR, "lock list error");
        return -1;
    }

    LL_DELETE(list_ct.p_head, el_del);
    free(el_del);
    list_ct.el_count--;

    pthread_mutex_unlock(&mutex_list);

    //log_level(U_LOG_DEBUG,"observer event node %p was delete !!",el_del);
    return 0;
}
// 查询 event 是否触发
int observer_event_happent(void *el,const u8 event){

    if(!el)
        return 0;
    
    Element_ST *p_el = (Element_ST*)el;
    // 任何时候 element 由一个线程创建，同时也由改线程 destory ，仅仅查询无需上锁。
    if( p_el->event_happen  == event)
        return 1;
    else
        return 0;
}

// block 等待 event 发生，并把需要的数据写入 userdata
int observer_event_wait(void *el, u8 event,const int timeout_ms,void *pp_user_data){
    struct timeval old_tm,current_tm;

    memset(&old_tm,0,sizeof(struct timeval));
    memset(&current_tm,0,sizeof(struct timeval));

    
    if(!el || !pp_user_data || event == EVENT_NON)
        return -1;
    
    Element_ST *p_el = (Element_ST*)el;
    // get current time
    if(timeout_ms > 0)
        gettimeofday(&old_tm,NULL);
    do{
        if( observer_event_happent(el,event) ){
            log_level(U_LOG_DEBUG,"event happend");
            observer_delete(el);
            return 0;
        }
            
    }while( timeout_ms > 0 && (_TM_MSEC(current_tm.tv_sec,current_tm.tv_usec) - _TM_MSEC(old_tm.tv_sec,old_tm.tv_usec)) < timeout_ms);

    // todo wait thread unlock
    log_level(U_LOG_INFO,"waitting event timeout !!");

    observer_delete(el);
    return ;
}

// 订阅事件，并等待。
int observer_event_add_wait( u8 event,const int timeout_ms,void *p_user,void *p_user_data){

    void *p_el = observer_add(event,p_user,p_user_data);

    if(!p_el)
        return -1;
    return observer_event_wait(p_el,event,timeout_ms,p_user_data);
}
int observer_distory(void){

    Element_ST *el,*tmp;
    // lock 
     if(pthread_mutex_lock(&mutex_list) !=0){
        log_level(U_LOG_ERROR, "lock list error");
        return -1;
    }
    LL_FOREACH_SAFE(list_ct.p_head, el, tmp){
        observer_delete(el);
    }
    
    log_level(U_LOG_INFO, "observer list have been destory");
    list_ct.p_head = NULL;

    pthread_mutex_unlock(&mutex_list);
    pthread_mutex_destroy(&mutex_list);
}
// test code 
#if 1
#define _MAX_OBSER_NODE   (100)

static int test_result_success = 0;
static void _test_thread_observer(void *id){
    int value = 0;
    int *p_recv = &value;

    //log_level(U_LOG_DEBUG,">>> id %d ,value adress %p ",*(int*)id,p_recv);
    int ret = observer_event_add_wait(EVENT_RECV_DATA, 1000, id, p_recv);
    
    if( !id || ret < 0){
        log_level(U_LOG_ERROR,"observer event add and wait failt !!! ");
        return ;
    }
    
    log_level(U_LOG_INFO,">>> id %d event add ",*(int*)id);
    if(!p_recv && !id){
        log_level(U_LOG_WARN, "no recv anything.");
        return ;
    }

    if((*(int*)p_recv) == (*(int*)id)){
        test_result_success++;
    }
}
static void _test_observer_push(void){
    Element_ST *el,*tmp;
    int *p_id = NULL,count = 0;
    log_level(U_LOG_DEBUG, " _test_observer_push !!");

    while(observer_queue_element_count_get() < _MAX_OBSER_NODE){
            sleep(2);
            log_level(U_LOG_DEBUG,"queue count %d",observer_queue_element_count_get());
        }
    if(observer_list_lock() < 0){
        log_level(U_LOG_ERROR, "lock list failt");
        return ;
    }
    //log_level(U_LOG_DEBUG, ">>>  _test_observer_push get lock ");
    for(count =0;count <_MAX_OBSER_NODE ;  count++ ){
        
            LL_FOREACH_SAFE(list_ct.p_head, el, tmp){
                if(count == (*(int*)el->p_user) ){
                    el->event_happen = el->observer_event;
                    *(int*)el->p_user_data = count;
                    log_level(U_LOG_DEBUG, ">>>  %d event push ",count);
                }
            }
        }
    
    observer_list_unlock();
}
// 1.功能测试：observer 队列是否能正常获取另一个线程的数据
// 2.线程锁是否正常工作。
// 3.是否存在内存泄露，和越界访问。

void unittest_observer_queue(void){
    int ret = 0;
    
    observer_creat(_MAX_OBSER_NODE);
    test_result_success = 0;
    
    pthread_t *pt = (pthread_t *)malloc(sizeof(pthread_t) * _MAX_OBSER_NODE);
    int * id = (int *)malloc(sizeof(int) * _MAX_OBSER_NODE);

    for (int i = 0; i < _MAX_OBSER_NODE; i++){
        // index 
        id[i] = i;
        if (pthread_create(&pt[i], NULL, _test_thread_observer, &id[i]) != 0){
                log_level(U_LOG_ERROR,"thread create failed!\n");
                return 1;
        }
    }
    
    // push all the data.
    _test_observer_push();
    
    for (int i = 0; i < _MAX_OBSER_NODE; i++){
              pthread_join(pt[i], NULL);
      }

    observer_distory();
    if(test_result_success == _MAX_OBSER_NODE)
        log_level(U_LOG_INFO, "unit test success");
    else 
        log_level(U_LOG_INFO, "unit test failt");

    free(pt);
    free(id);
    up2p_log_close();
    return ;
}
#endif

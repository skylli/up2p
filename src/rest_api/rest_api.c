/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    _up2ps_rest_api.c

   1.declere rest api.

    TIME LIST:
    CREATE  skyli   2017-10-23 17:30

*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include <ulfius.h>
#include "up2p_oauth2.h"
#include "vlink.h"
#include "util.h"
#include "rest_api.h"
#include "factory_evalogik.h"



//static u8 test_loop  = 10 ;
static Rest_Server_Info_ST rest_ctl;
// 1、接收点
static void _rest_recv_proc_thread(void)
{
    int len;
    UDP_INFO info;

    char recvbuf[MAX_NET_PACKAGE_SIZE];
    while(rest_ctl.recv_pthread_runing){

            
            memset(&info,0,sizeof(UDP_INFO));
            memset(recvbuf,0,MAX_NET_PACKAGE_SIZE);
            if(socket_udp_recv(rest_ctl.fd,recvbuf,MAX_NET_PACKAGE_SIZE,&len, &info) > 0){
                link_recv_handle(recvbuf, len, &info);
            }
   }        
    pthread_exit(0);
}

// 1、建立 observer 推送池子。
// 2、建立socket
// 3、初始化id 资源锁。
// 4. 建立 处理接收的线程
void restApi_init(u32    max_concurrent ){

    rest_ctl.devid_id_count = 0;
    observer_creat(max_concurrent);
    rest_ctl.fd = socket_listenbin_creat(UP2PS_REST_PORT);
    
    if(rest_ctl.fd == -1 ){
        log_level(U_LOG_ERROR,"creat socket failt !!");
        return -1;
    }
    pthread_mutex_init(&rest_ctl.dev_id_mutex, NULL);
    rest_ctl.recv_pthread_runing = 1;
    pthread_create(&rest_ctl.thread_recv, NULL, (void *)_rest_recv_proc_thread, NULL);
    log_level(U_LOG_INFO, "rest api have beed reset.");
    return ;
}
// 0. 退出接收线程
// 1. 关闭 socket。
// 2. 释放推送链表
// 3. destory 资源锁
// 4. 
void restApi_clean(void){

    rest_ctl.recv_pthread_runing = 0;
    
    log_level(U_LOG_ERROR, "befor pthread_join");
    pthread_join(rest_ctl.thread_recv,NULL);
    close(rest_ctl.fd);

    log_level(U_LOG_ERROR, "after pthread_join");

    observer_distory();
    pthread_mutex_unlock(&rest_ctl.dev_id_mutex);
    pthread_mutex_destroy(&rest_ctl.dev_id_mutex);
}
static u32 _device_id_increase(void){
    u32 id = 0;
    if( pthread_mutex_lock(&rest_ctl.dev_id_mutex ) !=0){
        log_level(U_LOG_ERROR, "lock device id error");
        return -1;
    }
    rest_ctl.devid_id_count++;
    id = rest_ctl.devid_id_count;
    pthread_mutex_unlock(&rest_ctl.dev_id_mutex);
    return id;
}
static u32 _device_id_reduce(void){

    u32 id = 0;

    if( pthread_mutex_lock(&rest_ctl.dev_id_mutex ) !=0){
        log_level(U_LOG_ERROR, "lock device id error");
        return -1;
    }
    rest_ctl.devid_id_count--;
    id = rest_ctl.devid_id_count;
    pthread_mutex_unlock(&rest_ctl.dev_id_mutex);
    return id;


}

// 全局的 device id自增, dev0 的值为 device id
// dev1 位随机数
static int _device_id_get(u32 *devid0,u32 *devid1){
    if(!devid0 || !devid1)
        return U_ERROR_INVALID;
// 全局的 device id自增, dev0 的值为 device id
    *devid0 = _device_id_increase();
    //*devid1 = (rand() | (rand() << 16);
    *devid1 = 0xff00000000000000;
    return 0;
}

/*
* 1、目前的 http server 框架使用单个线程处理独立的https 请求
* 2、共用一个 socket，udp socket 接收的数据使用类 epoll 的方式推送到各个http 处理线程。
* 3
*
*/  
// remember value was borrow. u can't free it, or modify it.
char *_jason_str_get_value(json_t *json,const char *key)
{
    // find  key
     char *pvalue =NULL;
     
     json_t *jfind = json_object_get(json,key);
     if(!jfind || !json_is_string(jfind) ) 
       return NULL;
     
     return json_string_value(jfind);
}
// 1. 建立session,建立失败则直接退出
// 2. 发送请求。
// 3. 获取设备回应的消息。
// todo 返回码。
static int _device_ctrl(Product_CtlInfo *p_ctl_info, DEV_CmdInfo *p_cmd_info,u8 **pp_output,u32 **pp_outlen){

    // 1. 建立session,建立失败则直接退出
    // 1.1 获取devide
    // 1.2 获取 key
    u32 mydev0,mydev1;
    int ret =-1;
    
    // 1.3 构造自身的临时的 devi id
    _device_id_get(&mydev0,&mydev1);
    log_level(U_LOG_DEBUG,"mydevid %x %x",mydev0,mydev1);
    // 1.4 获取 远端ip
    u32 host_ip = socket_gethost_ip(DEVICE_SERVER0);
    // 1.4 获取 session token
    LINK_ST *p_link = link_open(rest_ctl.fd,0,mydev0,mydev1,p_ctl_info->devid0,
                    p_ctl_info->devid1,host_ip,htons(UP2PS_PORT),p_ctl_info->aeskey0,p_ctl_info->aeskey1);
    
    if(p_link == NULL){
        log_level(U_LOG_ERROR,"build session failt!!");
        return -1;
    } 
    // 2. 发送请求。
    ret = link_request_wait_response(p_link,p_cmd_info->cmd,p_cmd_info->p_payload,p_cmd_info->len_paylod,(p_cmd_info->cmd + 1),pp_output,pp_outlen,0);
    
    // 3. 获取设备回应的消息。
    // 释放 p_link
    link_close(p_link);
    return ret;
}
#if 0
// 1. 解析jason 的function 转换为u32 设备可以理解的形式。
// 2. 解析 value，并把内容封装到 alloc 的地址里以便发送到设备。
static int _factory_evalogik_cmd_value_alloc(Product_CtlInfo *p_ctl_info,DEV_CmdInfo *p_cmdinfo){
    u8 *p_ret_payload = NULL,*p_svalue,*p_scmd;
    
    if(!p_cmdinfo || !p_ctl_info ){
        return U_ERROR_INVALID;
    }
    p_scmd = _jason_str_get_value(p_ctl_info->j_function ,REQUEST_JKEY_FUNC_CMD);
    if( !p_scmd ){
        log_level(U_LOG_WARN,"Can't find any function!!");
        return U_ERROR_NULL;
    }
    // 获取设备层面的控制命令
    // cmd turnOn
    if( !strcmp(p_scmd,_EVALOGIK_FUNCTION_TURNON)){
        p_cmdinfo->cmd = CMD_SEND_SERIAL;
        
      char *p = json_dumps(p_ctl_info->j_function,JSON_COMPACT);
      if(p){
           log_level(U_LOG_INFO,">>>>>> function jason is : %s",p);
          free(p);
          p = NULL;
      }
        // 获取 value
        p_svalue = _jason_str_get_value(p_ctl_info->j_function,REQUEST_JKEY_FUNC_VALUE);
      
        log_level(U_LOG_INFO,">>>>>> value is : %p",p_svalue);
        if(p_svalue)
            log_level(U_LOG_INFO,">>>>>> value is : %s",p_svalue);

        if( p_svalue && strlen(p_svalue) > 0){
            p_ret_payload = umalloc(strlen(p_svalue)+1);
            if( !p_ret_payload){
                log_level(U_LOG_ERROR,"malloc value failt !!");
                return U_ERROR_NULL;
            }
            else {
                memcpy(p_ret_payload,p_svalue,strlen(p_svalue));
                p_cmdinfo->p_payload = p_ret_payload;
                p_cmdinfo->len_paylod = strlen(p_svalue);
            }
        }
        return 0;
    }
    
    return U_ERROR_NULL;
}

#endif
/* send interface */
U_Func_T _u_fac_funcTable[U_FAC_MAX + 1] = 
{
    (U_Func_T)_factory_evalogik_payload_alloc,//evalogik

    (U_Func_T)NULL
};
// 该数组储存厂家的名称，在收到 jason 时会查询该数组匹配，
// 匹配则调用该厂家的功能函数。
static const u8 *_u_fac_nameTable[U_FAC_MAX + 1] = 
{
    FAC_NAME_EVALOGIK
};

// 1. 匹配厂家，
// 2.调用厂家函数解析获取 需要控制设备的 payload.
static int _device_factory_function_parse(Product_CtlInfo *p_ctl_info,DEV_CmdInfo *p_cmdinfo){
    // todo 解析各类厂家
    int i = 0;
    
    for(i = 0; i < U_FAC_MAX;i++){

        //  1.匹配厂家
        if(_u_fac_nameTable[i]  && ( strcmp(_u_fac_nameTable[i], p_ctl_info->p_factory) == 0 ) ){
            if( _u_fac_funcTable[i]){

                // 2. 调用厂家功能函数解析，获取控制设备的payload .
                return (_u_fac_funcTable[i])(p_ctl_info,p_cmdinfo);
            }
            return U_ERROR_NULL;
        }
    }
    return U_ERROR_NULL;
    // sofar just one factory
}

/*
接收json 的格式 ：
请求: 
{
    "device_id":"123234",  //string
    "token":"123234",     // device session aes key
    "index": 123,        // int
    "category":"socket",
    "batch":"20171031",
    "factory":"evalogik",
    "function":{
            "command":"turnon",
            "value":"null"
    }
}

回应： 
{
    "index":123 
    "statue": 0,
    "data": "ok"
}
*/

int restApi_handle_dev_type_value_request (const struct _u_request * request, struct _u_response * response, void * user_data) {

    // 1. check if it's an jason packet.
    // 2. get devid and aes key.
    // 3. open device. 
    // 4. send out cmd request.

    // 1. check if it's an jason packet.
    int ret = 0,ret_dev,ret_key,len_dev_respond;
    Product_CtlInfo ctl_info;
    DEV_CmdInfo  cmd_info;
    u8 str_print[128],*p_dev_respond = NULL;
    
    json_t * jreq = ulfius_get_json_body_request(request, NULL);
    json_t *jresponse = json_object();
    
    char *p = json_dumps(jreq,JSON_COMPACT);
      if(p){
           log_level(U_LOG_INFO,">>>>>> request jason is : %s",p);
          free(p);
          p = NULL;
      }

    memset(&ctl_info,0,sizeof(Product_CtlInfo));
    memset(&cmd_info,0,sizeof(DEV_CmdInfo));
    memset(str_print,0,128);

    
    json_object_set_new(jresponse, RESPONSE_JKEY_INDEX,  json_copy(json_object_get(jreq,REQUEST_JKEY_INDEX))); // json_string
    if(jreq == NULL){
        log_level(U_LOG_WARN,"request don't have any json packet.");
        json_object_set_new(jresponse, RESPONSE_JKEY_STATUS,json_integer(400)); // json_string
        json_object_set_new(jresponse, RESPONSE_JKEY_DATA,json_string("request don't have any json packet.")); // json_string
    
        ulfius_set_json_body_response(response, 400, jresponse);
        
        json_decref(jresponse);
        jresponse = NULL;
        
        return U_CALLBACK_CONTINUE;
    }
    // 2. 解析 jason 获取产品的 id、aeskey 、品类、厂家、批次    
    ret_dev = ascii_to_2u32(_jason_str_get_value(jreq,REQUEST_JKEY_DEVID),&ctl_info.devid0,&ctl_info.devid1);
    ret_key = ascii_to_2u32(_jason_str_get_value(jreq,REQUEST_JKEY_TOKEN),&ctl_info.aeskey0,&ctl_info.aeskey1);

    
    ctl_info.j_function = json_object_get(jreq,REQUEST_JKEY_CMD);
    ctl_info.p_batch = _jason_str_get_value(jreq,REQUEST_JKEY_BATCH);
    ctl_info.p_factory = _jason_str_get_value(jreq,REQUEST_JKEY_FACTORY);
    ctl_info.p_category = _jason_str_get_value(jreq,REQUEST_JKEY_CATEGOGY);

    if(ret_dev < 0 || ret_dev < 0 ){
        u8 *p_str = NULL;
        p_str = (ret_dev < 0)?REQUEST_JKEY_DEVID:REQUEST_JKEY_TOKEN;
        sprintf(str_print,"request don't have %s\n",p_str);
        log_level(U_LOG_WARN,"%s.",str_print);
        
        json_object_set_new(jresponse, RESPONSE_JKEY_STATUS,json_integer(400)); // json_string
        json_object_set_new(jresponse, RESPONSE_JKEY_DATA,json_string(str_print)); // json_string
        ulfius_set_json_body_response(response, 400, jresponse);
        goto _rest_return;
    }
    else if(!ctl_info.p_factory || !ctl_info.p_category || !ctl_info.p_batch ){

        u8 *p_need_str = NULL;
        p_need_str = (!ctl_info.p_factory)?REQUEST_JKEY_FACTORY:p_need_str;
        p_need_str = (!ctl_info.p_category)?REQUEST_JKEY_CATEGOGY:p_need_str;
        p_need_str = (!ctl_info.p_factory)?REQUEST_JKEY_BATCH:p_need_str;
        sprintf(str_print,"request don't have %s\n",p_need_str);
        log_level(U_LOG_WARN,"%s.",str_print);
        
        json_object_set_new(jresponse, RESPONSE_JKEY_STATUS,json_integer(400)); // json_string
        json_object_set_new(jresponse, RESPONSE_JKEY_DATA,json_string(str_print)); // json_string
        ulfius_set_json_body_response(response, 400, jresponse);

        goto _rest_return;
    }
    // 3. 解析 function 获取设备端的控制命令。
    // notice: the cmd_info.p_payload must be free.
    ret = _device_factory_function_parse(&ctl_info,&cmd_info);
    if(ret < 0){
        
        log_level(U_LOG_WARN,"unknow function.");
        json_object_set_new(jresponse, RESPONSE_JKEY_STATUS,json_integer(400)); // json_string
        json_object_set_new(jresponse, RESPONSE_JKEY_DATA,json_string("unknow function ")); // json_string
        ulfius_set_json_body_response(response, 400, jresponse);

        
        ufree(cmd_info.p_payload);
        cmd_info.p_payload = NULL;
        goto _rest_return;
    }
    // 4. 发送请求.
    // dev 回应的 data 必须 free
    ret = _device_ctrl(&ctl_info,&cmd_info,&p_dev_respond,&len_dev_respond);

    if(ret < 0 ){
        
        log_level(U_LOG_WARN,"device didn't respond.");
        json_object_set_new(jresponse, RESPONSE_JKEY_STATUS,json_integer(404)); // json_string
        json_object_set_new(jresponse, RESPONSE_JKEY_DATA,json_string("device didn't respond")); // json_string
        ulfius_set_json_body_response(response, 404, jresponse);
        
        
        ufree(p_dev_respond);
        p_dev_respond = NULL;
        ufree(cmd_info.p_payload);
        cmd_info.p_payload = NULL;
        goto _rest_return;
    }
    else{
        json_object_set_new(jresponse, RESPONSE_JKEY_STATUS,json_integer(200)); // json_string
        json_object_set_new(jresponse, RESPONSE_JKEY_DATA,json_string(p_dev_respond)); // json_string
        ulfius_set_json_body_response(response, 200, jresponse);

        ufree(p_dev_respond);
        p_dev_respond = NULL;
        ufree(cmd_info.p_payload);
        cmd_info.p_payload = NULL;

//        test_loop--;
        log_level(U_LOG_WARN,"OK respond request.");
    }
_rest_return:
    if(jreq) 
                json_decref(jreq);
        jreq = NULL;
    if(jresponse)
        json_decref(jresponse);
    jresponse = NULL;
        
    return U_CALLBACK_CONTINUE;

}

#if 1
void unittest_rest_api(void){

    struct _u_instance instance;
    
    logLevel_set(0);
      // Initialize instance with the port number  HTTPS_REST_PORT
  if (ulfius_init_instance(&instance, 9511, NULL, NULL) != U_OK) {
    log_level(U_LOG_ERROR, "Error ulfius_init_instance, abort\n");
    return (-1);
  }
    // oauth2  config 
    struct _glewlwyd_resource_config g_config;
    
    g_config.method = G_METHOD_URL;
    g_config.oauth_scope = "scope1";
    g_config.jwt_decode_key = "secret";
    g_config.jwt_alg = JWT_ALG_HS512;
    g_config.realm = "example";

    restApi_init(1000);
    
    // Endpoint list declaration
    //ulfius_add_endpoint_by_val(&instance, "POST", SKILL_PATH_DISCOVER, NULL, 0, &skill_handle_distcoverDev_request, NULL);
    ulfius_add_endpoint_by_val(&instance, "POST", "/rest/control", NULL, 0,&restApi_handle_dev_type_value_request, NULL);
    log_level(U_LOG_DEBUG,"bind url path end.");

    // Start the framework
    if (ulfius_start_framework(&instance) == U_OK) {
        log_level(U_LOG_DEBUG,"Start framework on port %d\n", instance.port);
     // Wait for the user to press <enter> on the console to quit the application
    while(1){
        sleep(1);
        }
    } else {
        log_level(U_LOG_DEBUG, "Error starting framework\n");
    }
    
    rest_ctl.recv_pthread_runing = 0;
    sleep(3);
    
    log_level(U_LOG_DEBUG, "ending ....\n");
    restApi_clean();
    
    log_level(U_LOG_INFO, "test end\n");
    // cmd
    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);

}

#endif

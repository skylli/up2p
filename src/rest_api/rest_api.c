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

#include "up2ps.h"
#include "rest_api.h"

/*
* 1、目前的 http server 框架使用单个线程处理独立的https 请求
* 2、共用一个 socket，udp socket 接收的数据使用类 epoll 的方式推送到各个http 处理线程。
* 3、
*
*/ 
// remember value was borrow. u can't free it, or modify it.
static char *_jason_str_get_value(json_t *json,const char *key)
{
    // find  key
     char *pvalue =NULL;
     
     json_t *jfind = json_object_get(json,key);
     if(!jfind || !json_is_string(jfind) ) 
       return NULL;
     
     return json_string_value(jfind);
}
// creat an socket 
static int _dev_open(u32 dev0,u32 dev1,u32 key0, u32 key1,u32 *p_token)
{
    int ret;
    char *sbuf[MAX_NET_PACKAGE_SIZE];

    memset(sbuf,0,MAX_NET_PACKAGE_SIZE);
#if 0
    // is dev online.
    if( !user_find(dev0,dev1)){
        log_level(U_LOG_INFO,"dev offline");
        return U_ERROR_LIINKDEV;
    }

    // update session token
    ret = s_send_data_cmd(dev0,dev1,key0,key1,0,CMD_GET_TOKEN,NULL,0,&sbuf);
    if(ret  < 0)
        return ret;
    // GET respond
    ret = s_wait_data_cmd(dev0,dev1,key0,key1,0,CMD_GET_TOKEN_ACK,p_token,sizeof(u32),&sbuf);
    log_level(U_LOG_DEBUG," >>> TOKEN %p \n", *p_token);
#endif

    return ret;
}

int restApi_handle_dev_type_value_request (const struct _u_request * request, struct _u_response * response, void * user_data) {

    // 1. check if it's an jason packet.
    // 2. get devid and aes key.
    // 3. open device. 
    // 4. send out cmd request.

    // 1. check if it's an jason packet.
    json_t * jreq = ulfius_get_json_body_request(request, NULL), * jresponse = NULL;
    if(jreq == NULL){
        log_level(U_LOG_WARN,"request don't have any json packet.");
        json_object_set_new(jresponse, "return",json_integer(-2)); // json_string
        json_object_set_new(jresponse, "reason",json_string("request don't have any json packet.")); // json_string
    
        ulfius_set_json_body_response(response, 400, jresponse);
    }
    // 2. get devid and aes key.
    char *p_devid = _jason_str_get_value(jreq,REQUEST_JKEY_DEVID);
    char *p_aeskey = _jason_str_get_value(jreq,REQUEST_JKEY_TOKEN);

    if(!p_devid || !p_aeskey){
        log_level(U_LOG_WARN,"request don't have device id and token .");
        json_object_set_new(jresponse, "return",json_integer(-2)); // json_string
        json_object_set_new(jresponse, "reason",json_string("cant't find device_id and token.")); // json_string
        ulfius_set_json_body_response(response, 400, jresponse);
    }
    // 3. open device. 
}

#if 1
void restApi_unittest_rest_api(void){

    log_level(U_LOG_INFO, "rest api test.");
    
}

#endif

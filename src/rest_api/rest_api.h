/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    _up2ps_rest_api.h

   1.declere rest api.

    TIME LIST:
    CREATE  skyli   2017-10-23 17:30

*******************************************************************************/
#ifndef _up2ps_rest_api_h_
#define _up2ps_rest_api_h_

/*** jason key define*********/
#define REQUEST_JKEY_DEVID      "device_id"
#define REQUEST_JKEY_TOKEN      "token"
#define REQUEST_JKEY_INDEX      "index"
#define REQUEST_JKEY_CMD        "function"
#define REQUEST_JKEY_FUNC_CMD   "command"
#define REQUEST_JKEY_FACTORY    "factory"
#define REQUEST_JKEY_CATEGOGY    "category"
#define REQUEST_JKEY_BATCH    "batch"

#define REQUEST_JKEY_FUNC_VALUE      "value"

// response 回应的 key
#define RESPONSE_JKEY_INDEX "index"
#define RESPONSE_JKEY_STATUS    "status"
#define RESPONSE_JKEY_DATA      "data"


typedef enum U_FACTORY_NAME_TYPE
{
    U_FAC_EVALOGIK,
    U_FAC_KPXMALION,

    U_FAC_MAX
}U_Factory_Name_T;


typedef struct{
    int fd;
    int recv_pthread_runing;
    unsigned long devid_id_count;// 所有请求的设备 id 通过自增获取。用完则自减.
    pthread_mutex_t dev_id_mutex; // id count 锁
    pthread_t thread_recv;
    
}Rest_Server_Info_ST;

typedef struct{
    u32 devid0;
    u32 devid1;
    u32 aeskey0;
    u32 aeskey1;
    u8 *p_factory;  // 厂家
    u8 *p_category; // 品类
    u8 *p_batch;    // 出厂批次
    json_t *j_function; 

}Product_CtlInfo;

typedef struct{
    u32 cmd;
    u32 len_paylod;
    u8 *p_payload;
}DEV_CmdInfo;

/***** 提供给 https-server 的入口 ***/
// type:value request 
int restApi_handle_dev_type_value_request (const struct _u_request * request, struct _u_response * response, void * user_data);
char *_jason_str_get_value(json_t *json,const char *key);

/*****设备端提供：  封装设备控制指令，并发送到设备处理***********/


/********** 单元测试 *******************************/
void unittest_rest_api(void);

#endif //  end _up2ps_rest_api_h_

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
#define REQUEST_JKEY_VALUE      "value"


/***** 提供给 https-server 的入口 ***/
// type:value request 
int restApi_handle_dev_type_value_request (const struct _u_request * request, struct _u_response * response, void * user_data);

/*****设备端提供：  封装设备控制指令，并发送到设备处理***********/


/********** 单元测试 *******************************/
void restApi_unittest_rest_api(void);

#endif //  end _up2ps_rest_api_h_

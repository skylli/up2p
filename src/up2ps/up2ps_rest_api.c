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
#include "util.h"
#include "apc_color_conversions.h"

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

int restApi_handle_dev_type_value_request (const struct _u_request * request, struct _u_response * response, void * user_data) {

    // 1. check if it's an jason packet.
    // 2. get devid and aes key.
    // 3. open device. 
    // 4. send out cmd request.

    // 1. check if it's an jason packet.
    json_t * jreq = ulfius_get_json_body_request(request, NULL), * jresponse = NULL;
    if(jreq == NULL){
        log_level(U_LOG_WARN,"request don't have any json packet.",);
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

    
}


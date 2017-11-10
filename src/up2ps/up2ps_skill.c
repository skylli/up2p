/**
 * up2ps_skill.c
 * the interface of amazon skill home.
 * gcc -o test test.c -lulfius
 */
#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include <ulfius.h>

#include "util.h"
#include "up2p.h"
#include "apc_color_conversions.h"
#include "up2ps.h"
#include "up2ps_skill.h"
#include "application.h"

#define VERSION_SKILL   ("V0.0.1")

#define VENDOR  ("SHOWX")

#define SKILL_DEVID     ("applianceId")
#define SKILL_FACTORY   ("manufacturerName")
#define SKILL_DEV_NAME  ("modelName")
#define SKILL_FRIEND_NAME   ("friendlyName")
#define SKILL_DEV_DESCR     ("friendlyDescription")
#define SKILL_REACH         ("isReachable")
#define SKILL_ACTIONS       ("actions")
#define SKILL_VERSION       ("version")
#define SKILL_DETAIL        ("additionalApplianceDetails")
#define SKILL_DISCOVER_APP  ("discoveredAppliances")
#define SKILL_PAYLOAD       ("payload")

#define SKILL_ACC_TOKEN     ("accessToken")
#define SKILL_CTRL_CMD     ("cmd")
#define SKILL_CTRL_RESULT   ("result")


#define SKILL_ACTION_TURNOFF               ("turnOff")
#define SKILL_ACTION_TURNON                ("turnOn")
#define SKILL_ACTION_INCREMENTPERCENTAGE    ("incrementPercentage")
#define SKILL_ACTION_DECREMENTPERCENTAGE    ("decrementPercentage")
#define SKILL_ACTION_SETCOLOR               ("setColor")
#define SKILL_ACTION_SETPERCENTAGE          ("setPercentage")


/*
* search json 
* noti : return obj need to be unrelate. use json_decref().
**/
json_t *sub_json_find(const char *key,json_t *obj)
{
    json_t *value;
    int len = strlen(key);
    void *iter = json_object_iter(obj);
    while(iter)
    {
        const char* src_key = json_object_iter_key(iter);
        if( src_key && strlen(src_key) == len && strcmp(key,src_key) == 0 ){
            value = json_object_iter_value(iter);
            return value;
        }
        iter = json_object_iter_next(obj, iter);
    }
    
    return NULL;
}
/*
* creat switch dev desciptin for skill
*/
json_t *skill_creat_device_switch(const char *dev_id,const char *dev_name,int inonline){

    json_t *dev = json_object();
    json_t *detail = json_object();
    json_t *action = json_array();
    if( !dev || !action ){
         log_level(U_LOG_WARN,"creat json failt!");
        return NULL;
    }
    /* add */
    json_object_set_new (dev, SKILL_DEVID, json_string(dev_id));
    json_object_set_new (dev, SKILL_FACTORY, json_string(VENDOR));
    json_object_set_new(dev,SKILL_DEV_NAME,json_string(dev_name));
    json_object_set_new(dev,SKILL_FRIEND_NAME,json_string("christmas trees"));
    json_object_set_new(dev,SKILL_DEV_DESCR,json_string("christmas trees"));
    json_object_set_new(dev,SKILL_REACH,json_boolean(inonline));

    json_array_append_new(action,json_string(SKILL_ACTION_TURNOFF));
    json_array_append_new(action,json_string(SKILL_ACTION_TURNON));
    
    json_array_append_new(action,json_string(SKILL_ACTION_INCREMENTPERCENTAGE));
    json_array_append_new(action,json_string(SKILL_ACTION_DECREMENTPERCENTAGE));

    json_array_append_new(action,json_string(SKILL_ACTION_SETCOLOR));
    json_array_append_new(action,json_string(SKILL_ACTION_SETPERCENTAGE));

    json_object_set_new(dev,SKILL_VERSION,json_string(VERSION_SKILL));
    json_object_set_new(dev,SKILL_ACTIONS,action);

    json_object_set_new(detail,"extraDetail1",json_string("socket that reachable"));
    
    json_object_set_new (dev, SKILL_DETAIL, detail);



    char *p = json_dumps(dev,JSON_COMPACT);
    if(p){
         log_level(U_LOG_INFO,"dev is : %s",p);
        free(p);
        p = NULL;
    }
    
    return dev;
}

json_t *skill_get_device(const char *acc_token){

    // search db to get the device id and name 
    // todo add mysql 
    const char todo_devid[] = "123456";
    const char todo_devNmae[] = "abcdef";
    // device info  translate into json type
    json_t *devinfo = skill_creat_device_switch(todo_devid,todo_devNmae,1);    
    if( !devinfo){
         log_level(U_LOG_WARN,"creat device info failt !!");
        return NULL;
    }
    // creat header
   json_t *app = json_object();
   json_t *devlist = json_array();
    
   json_array_append_new(devlist,devinfo);
   json_object_set_new(app,SKILL_DISCOVER_APP,devlist);

   char *p = json_dumps(app,JSON_COMPACT);
   if(p){
        log_level(U_LOG_INFO,"payload is : %s",p);
       free(p);
       p = NULL;
   }

   return app;
}
// find value string and malloc it which need tobe free.
char *value_string_alloc(json_t *json,const char *key){
    
    // find  key
    char *pvalue =NULL;
    
    json_t *jfind = json_object_get(json,key);
    if(!jfind || !json_is_string(jfind) ) 
      return NULL;
    
    const char *svalue = json_string_value(jfind);
    if(!svalue){
      return NULL;
      }
    
    // copy the strlen and unreference the json obj.
    log_level(U_LOG_DEBUG,"alloc len = %d",strlen(svalue));
    pvalue = (char*)malloc(strlen(svalue) + 1);
    if( !pvalue){
        log_level(U_LOG_ERROR, "Alloc failt !!");
        return NULL;
    }
    strcpy(pvalue,svalue);
    
    return pvalue;
}
int value_int_get(json_t *json,const char *key){
    
    // find  key
    char *pvalue =NULL;
    
    json_t *jfind = json_object_get(json,key);
    if(!jfind || !json_is_integer(jfind) ) 
      return -1;
    
    int value = json_integer_value(jfind);
    
    // copy the strlen and unreference the json obj.
    log_level(U_LOG_DEBUG," %s :value =  %d",key, value);    

    return value;
}

// discover device 
json_t *skill_discover_device(json_t *j_req){
    
    // find access token 
    char *ptoken =NULL;
    
    ptoken = value_string_alloc(j_req,SKILL_ACC_TOKEN);
    if(!ptoken)
        return NULL;
    
    // find dev
    json_t *jret =  skill_get_device(ptoken);

    free(ptoken);
    ptoken = NULL;
    
    return jret;
}
#if 1

#define SKILL_REQUE_COLOR_PAYLOAD   ("payload")
#define SKILL_REQUE_PAYLOAD   ("payload")

#define SKILL_REQUE_COLOR_PAYLOAD_H   ("hue")
#define SKILL_REQUE_COLOR_PAYLOAD_S   ("saturation")
#define SKILL_REQUE_COLOR_PAYLOAD_B   ("brightness")

#define SKILL_NUMBER_GET(j)     ((json_is_integer(j) ? json_integer_value(j):json_real_value(j)))



static int _resquest_payloadValue_integer_get(json_t *req,const char *key){
    // find  key
    char *pvalue =NULL;
    int ret = 0;
    json_t *jfind = json_object_get(req,SKILL_REQUE_PAYLOAD);
    
    if(!jfind || !json_is_object(jfind) ) {
        log_level(U_LOG_WARN,"can't find %s !!!",SKILL_REQUE_PAYLOAD);
        return U_ERROR_SERVER_REQUEST_PAYLOAD;
     }    
    //JSON_REAL
    json_t *sub = json_object_get(jfind,key);
    if( !sub || !json_is_number(sub) ){
             
        log_level(U_LOG_WARN," can't find %s !!!",key);
        return -1;
        }
             
    return (int)SKILL_NUMBER_GET(sub);

}

static int _resquest_getHsb(json_t *req,HsvColor *hsb){
    // find  key
    char *pvalue =NULL;
    int ret = 0;
    json_t *jfind = json_object_get(req,SKILL_REQUE_COLOR_PAYLOAD);
    
    if(!jfind || !json_is_object(jfind) ) {
        log_level(U_LOG_WARN,"color can't find %s !!!",SKILL_REQUE_COLOR_PAYLOAD);
        return U_ERROR_SERVER_REQUEST_PAYLOAD;
     }    
    //JSON_REAL
    json_t *hub = json_object_get(jfind,SKILL_REQUE_COLOR_PAYLOAD_H);
    json_t *s = json_object_get(jfind,SKILL_REQUE_COLOR_PAYLOAD_S);
    json_t *v = json_object_get(jfind,SKILL_REQUE_COLOR_PAYLOAD_B);
    if( !hub || !s || !v || !json_is_number(hub) 
             || !json_is_number(s) || !json_is_number(v) ){
             
        log_level(U_LOG_WARN,"color can't find hsb !!!");
          ret =  U_ERROR_SERVER_REQUEST_PAYLOAD;
        goto _RESQUEST_END;
        }
             
    hsb->H = (double)SKILL_NUMBER_GET(hub);
    hsb->S = (double)SKILL_NUMBER_GET(s);
    hsb->V = (double)SKILL_NUMBER_GET(v);
    
    log_level(U_LOG_INFO,"hsv is  %lf  %lf %lf",hsb->H,hsb->S,hsb->V);
    ret = 0;
_RESQUEST_END:
    return ret;
}

#endif
static int _hsb2rgb(RgbIColor *rgb,HsvColor *hsv){
    RgbFColor *rgbF = RgbF_CreateFromHsv(hsv->H, hsv->S, hsv->V);
    if(rgbF){

        RgbIColor *rgbI = RgbI_CreateFromRealForm(rgbF->R, rgbF->G, rgbF->B);

        if(rgbI){
            rgb->R = rgbI->R;
            rgb->G = rgbI->G;
            rgb->B = rgbI->B;
            free(rgbI);
        }
        free(rgbF);

        log_level(U_LOG_DEBUG,"hsv convers to rgb is : R = %d G = %d , B = %d",(u8)rgb->R,(u8)rgb->G,(u8)rgb->B);
        return 0;
    }

    log_level(U_LOG_WARN,"hsv conver to rgb failt !!!");
    return U_ERROR_SERVER_REQUEST_PAYLOAD;
}
static _ctrl_tree(json_t *req,int cmdindex){
     
      int ret = NULL;
      u8 sbuff[255];
      int rlen = 1024;
      APC_PACKET *apc_packet = (APC_PACKET*)sbuff;
      memset(apc_packet,0,255);

      log_level(U_LOG_DEBUG,"cmd = %d\n",cmdindex);
      switch(cmdindex){
        case SKILL_CMD_TREE_TIMER:
        {
            int v = _resquest_payloadValue_integer_get(req,"value");
            if(v > 0 && v < 24){
              apc_packet->version = UART_VERSION;
              apc_packet->cmd = cmdindex;
              apc_packet->idx = 0;
              apc_packet->len = 1;
              apc_packet->payload[0]  = (u8)v;
              log_level(U_LOG_DEBUG,">>>>>> value = %d",apc_packet->payload[0]);
              ret = s_cmd_send(CMD_SEND_SERIAL,CMD_SEND_SERIAL_ACK,"0000000C4326605A","0000000000000000",apc_packet,
                      (sizeof(APC_PACKET) + apc_packet->len ),NULL,&rlen);
            }else // we don't do any thing  whie the value is illege.
              return 0;
          }
            break;
        case SKILL_CMD_TREE_TOP_ON:
          {
              UP2P_IO gpio;
              gpio.pin = 4;
              gpio.val = 1;
              log_level(U_LOG_DEBUG,">>>>>> SKILL_CMD_TREE_TOP_ON ");
              ret = s_cmd_send(CMD_GPIO_WRITE,CMD_GPIO_WRITE_ACK,"0000000C4326605A","0000000000000000",&gpio,
                      sizeof(UP2P_IO),NULL,&rlen);
          }
              break;
              
       case SKILL_CMD_TREE_TOP_OFF:
        {
              UP2P_IO gpio;
              gpio.pin = 4;
              gpio.val = 0;
              log_level(U_LOG_DEBUG,">>>>>> SKILL_CMD_TREE_TOP_OFF ");
              ret = s_cmd_send(CMD_GPIO_WRITE,CMD_GPIO_WRITE_ACK,"0000000C4326605A","0000000000000000",&gpio,
                      sizeof(UP2P_IO),NULL,&rlen);
        }
              break;
        default:

              memset(apc_packet,0,255);
              apc_packet->version = UART_VERSION;
              apc_packet->cmd = cmdindex;
              apc_packet->idx = 0;
              apc_packet->len = 0;
              ret = s_cmd_send(CMD_SEND_SERIAL,CMD_SEND_SERIAL_ACK,"0000000C4326605A","0000000000000000",apc_packet,
                      (sizeof(APC_PACKET) + apc_packet->len ),NULL,&rlen);
              break;
      }
      
     
      
      log_level(U_LOG_DEBUG," SWITCH on ret = %d",ret);
      
      return ret;

}
int skill_ctrl_dev(json_t *req,const char *devid,const char *acc_token,const char *pcmd){

    int ret = NULL;
    if( !devid || !acc_token || !pcmd){
        return -1;
    }
    // search the user and device 
    // todo
    // get device ip and port info
    // todo
    // parse cmd and send control message
     log_level(U_LOG_DEBUG,"devid %s \n token %s \n cmd %s \n",devid,acc_token,pcmd);
    // control
    UP2P_IO sio;
    int rlen = 1024;
    sio.pin = 5;

    // get cmd index 
    int cmdindex =  value_int_get(req,SKILL_CMD_INDEX);
    // else compare the cmd index 
    if(cmdindex > 0){
        ret  = _ctrl_tree(req,cmdindex);
    }else if( str_equal(pcmd,skill_cmd[SKILL_CMD_ON]) ){
        // todo 
        u8 sbuff[255];
        APC_PACKET *apc_packet = (APC_PACKET*)sbuff;
        memset(apc_packet,0,255);
        apc_packet->version = UART_VERSION;
        apc_packet->cmd = APC_CMD_SOCKET_ON;
        apc_packet->len = 0;
        apc_packet->idx = 0;
        log_level(U_LOG_DEBUG,">>>>>> SWITCH OFF");

        ret = s_cmd_send(CMD_SEND_SERIAL,CMD_SEND_SERIAL_ACK,"0000000C4326605A","0000000000000000",apc_packet,sizeof(APC_PACKET),NULL,&rlen);
        log_level(U_LOG_DEBUG," SWITCH on ret = %d",ret);
    }
    else if(str_equal(pcmd,skill_cmd[SKILL_CMD_OFF])){
    // todo 
        u8 sbuff[255];
        APC_PACKET *apc_packet = (APC_PACKET*)sbuff;
        memset(apc_packet,0,255);
        apc_packet->version = UART_VERSION;
        apc_packet->cmd = APC_CMD_SOCKET_OFF;
        apc_packet->len = 0;
        apc_packet->idx = 0;
        log_level(U_LOG_DEBUG,">>>>>> SWITCH OFF");
    
        ret = s_cmd_send(CMD_SEND_SERIAL,CMD_SEND_SERIAL_ACK,"0000000C4326605A","0000000000000000",apc_packet,sizeof(APC_PACKET),NULL,&rlen);
        //ret = s_cmd_send(CMD_GPIO_WRITE,CMD_GPIO_WRITE_ACK,"0000000C4326605A","0000000000000000",&sio,2,NULL,&rlen);
        
        log_level(U_LOG_DEBUG,"SWITCH OFF ret = %d",ret);
    }else if(str_equal(pcmd,skill_cmd[SKILL_CMD_SETCOLOR])){
        // get hsl value 
        HsvColor src_hsb;
        RgbIColor des_rgb;
        ret = _resquest_getHsb(req,&src_hsb);
        if( ret < 0 ){
            // there no hsv value in payload 
            log_level(U_LOG_WARN,"get hsb form requet payload failt!!");
            return ret;
        }
        // conver to rgb 
        ret = _hsb2rgb(&des_rgb,&src_hsb);
        if(ret < 0){
            log_level(U_LOG_WARN,"hsb conver to rgb  failt!!");
            return ret;
        }
        // send send control
        // TODO packet in an functions
        {
            u8 sbuff[255];
            APC_PACKET *apc_packet = (APC_PACKET*)sbuff;
            memset(apc_packet,0,255);
            apc_packet->version = UART_VERSION;
            apc_packet->cmd = APC_CMD_COLOR_SET;
            apc_packet->len = sizeof(RgbIColor);
            apc_packet->idx = 0;
            
            memcpy( apc_packet->payload,&des_rgb,sizeof(RgbIColor));
            log_level(U_LOG_DEBUG,">>>>>> len = %d   %d",(sizeof(APC_PACKET)+sizeof(RgbIColor)),(4+sizeof(RgbIColor)));
            ret = s_cmd_send(CMD_SEND_SERIAL,CMD_SEND_SERIAL_ACK,"0000000C4326605A","0000000000000000",apc_packet,(sizeof(APC_PACKET)+sizeof(RgbIColor)),NULL,&rlen);
       }
        }
    
    // todo 
    return ret;
}


int skill_control_parse(json_t *req)
{
    char *devid = NULL, *acc_token = NULL,*pcmd = NULL;
    json_t *rep = NULL;
    int ret =-1;
    // get devid 
    devid = value_string_alloc(req,SKILL_DEVID);
    // get access token 
    acc_token = value_string_alloc(req,SKILL_ACC_TOKEN);

    // get control cmd
    pcmd = value_string_alloc(req,SKILL_CTRL_CMD);
    
    // control device 
    ret = skill_ctrl_dev(req,devid,acc_token,pcmd);

    // copy the strlen and unreference the json obj.
    if(devid)      
        free(devid);
    if(acc_token)   
        free(acc_token);
    if(pcmd)       
        free(pcmd);

    return ret;
}

/*** test *****/
// test discover_device function.
void test_distcover_device(void){
 //   char *stoken= "{\"accessToken\":\"123456789\"}";
    int cnt = 1;

    for(;cnt;cnt--){
        json_t *request = json_object();
        
        json_object_set_new(request,SKILL_ACC_TOKEN,json_string("Atza|IwEBIPsIF6mlnwpft5gTz3bh6ew_syNvT6Y6fS1DztEvG0vi8VXKDnw8XDefmrpydfZOaUjtjLDAo-HJj8uIBtvgl_3J6CBcB8TbHBlhKiJycpl1z-0JJJXJ8kJXRldFSfIpTpZUfCPHMelfU-itt3psMLLlNWzwy_PAWXCoNCFSGm_X2zq5boxwZpqVROtS59ul3kdneNWE2P0vOWqVyM3iLP9TzANAlXH86RAnsT54CmP9wncyKbVvi5lbNWj6aBag7kQycM29AP5fQ0H4pdBSsUuxwLGJrcDr7jQa20HlLwARWQahnOPhZytwzD9RCRAJ5bOlzjfuXf9UvRNItoCe7FpF96CdMAdTu8DuZwfYy9Q0IaWellJrz59VrXb_dSwPSX-gYZ-uUU_eUE0m4sYMvN4tzcibyoXC-fzjQNgH_XvNxu1SSlgSwGVdQICbeTJBapmmDmn7iuievWKnNYV4uQH5z2rua0Ue2zneeDa-c2uJl4QdUUg-UvTeMevbDEwoiGTpq075TujfdyKcoJXq9myg29z7I36OJK0upl0l_frY7sJLYkVigCfx2byEFYhhu7Q"));
        json_t *jres = skill_discover_device(request);
        char *p = json_dumps(jres,JSON_COMPACT);
        if(p){
            log_level(U_LOG_INFO,"dev is : %s\n",p);
            free(p);
            p = NULL;
        }
        json_decref(jres);
        json_decref(request);
   }
    
}
// test discover_device function.
void test_ctrl_device(void){

 //   char *stoken= "{\"accessToken\":\"123456789\"}";
    json_t *request = json_object();
    
    //json_object_set_new(request,SKILL_ACC_TOKEN,json_string("switchdevices"));
 
    json_object_set_new(request,SKILL_ACC_TOKEN,json_string("Atza|IwEBIPsIF6mlnwpft5gTz3bh6ew_syNvT6Y6fS1DztEvG0vi8VXKDnw8XDefmrpydfZOaUjtjLDAo-HJj8uIBtvgl_3J6CBcB8TbHBlhKiJycpl1z-0JJJXJ8kJXRldFSfIpTpZUfCPHMelfU-itt3psMLLlNWzwy_PAWXCoNCFSGm_X2zq5boxwZpqVROtS59ul3kdneNWE2P0vOWqVyM3iLP9TzANAlXH86RAnsT54CmP9wncyKbVvi5lbNWj6aBag7kQycM29AP5fQ0H4pdBSsUuxwLGJrcDr7jQa20HlLwARWQahnOPhZytwzD9RCRAJ5bOlzjfuXf9UvRNItoCe7FpF96CdMAdTu8DuZwfYy9Q0IaWellJrz59VrXb_dSwPSX-gYZ-uUU_eUE0m4sYMvN4tzcibyoXC-fzjQNgH_XvNxu1SSlgSwGVdQICbeTJBapmmDmn7iuievWKnNYV4uQH5z2rua0Ue2zneeDa-c2uJl4QdUUg-UvTeMevbDEwoiGTpq075TujfdyKcoJXq9myg29z7I36OJK0upl0l_frY7sJLYkVigCfx2byEFYhhu7Q"));
    json_object_set_new(request,SKILL_CTRL_CMD,json_string("off"));
    json_object_set_new(request,SKILL_DEVID,json_string("123456789"));
    
    int ret = skill_control_parse(request);

    log_level(U_LOG_INFO,"result %d \n",ret);
    
    json_decref(request);
}

/**
 * Callback function for the web application on /helloworld url call
 */
int skill_handle_distcoverDev_request (const struct _u_request * request, struct _u_response * response, void * user_data) {

  json_t *j_req = ulfius_get_json_body_request(request, NULL);
  
 log_level(U_LOG_DEBUG,"request %p\n",j_req);
  if( !j_req ){
      ulfius_set_string_body_response(response, 401, "no access token");  
      return U_CALLBACK_UNAUTHORIZED;
  }
  
  char *p = json_dumps(j_req,JSON_COMPACT);
    if(p){
         log_level(U_LOG_INFO,">>>>>> dev is : %s",p);
        free(p);
        p = NULL;
    }
  json_t *j_resp = skill_discover_device(j_req);

  if(!j_resp){
    ulfius_set_string_body_response(response, 401, "no access token");  
    
    if(j_req) json_decref(j_req);
    j_req = NULL;
    
    return U_CALLBACK_UNAUTHORIZED;
    }
  else{ 
    log_level(U_LOG_INFO,"\nrepond ok 200 \n");
    ulfius_set_json_body_response(response, 200, j_resp);
  
    json_decref(j_resp);
    j_resp = NULL;
    
    if(j_req) json_decref(j_req);
    j_req = NULL;
    
    return U_CALLBACK_CONTINUE;
    }
}
/**
 * Start a new counter by setting the number of sheeps to a specific value
 * return the current number of sheeps in a json object
 */
int skill_handle_controlDev_request (const struct _u_request * request, struct _u_response * response, void * user_data) {

  json_t * jreq = ulfius_get_json_body_request(request, NULL), * json_body = NULL;
  log_level(U_LOG_INFO,"request %p\n",jreq);
  
  int ret = skill_control_parse(jreq);

  json_body = json_object();
#if 1

  if(ret < 0){
      log_level(U_LOG_WARN," ret = %d",ret);
      json_object_set_new(json_body, "ret",json_integer(ret)); // json_string
      json_object_set_new(json_body, "reason",json_string("todo authon failt")); // json_string

      ulfius_set_json_body_response(response, 400, json_body);
   }
  else{
      json_object_set_new(json_body, "ret",json_integer(0));
      ulfius_set_json_body_response(response, 200, json_body);
  }
#endif

  if(json_body) json_decref(json_body);
  json_body = NULL;
  
  if(jreq) json_decref(jreq);
  jreq = NULL; 
  
  return U_CALLBACK_CONTINUE;
}


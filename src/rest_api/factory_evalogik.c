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
#include "util.h"
#include "factory_evalogik.h"

// 1. 获取 json value 的长度，为输出的 payload 申请堆栈空间 malloc.
// 2. 匹配 function 转换为 硬件能识别的功能指令.
// 3. 封装 payload
// 4. 输出.

static int _christmas_tree_ctl_payload_alloc(Product_CtlInfo *p_ctl_info,DEV_CmdInfo *p_cmdinfo){
     int ret = NULL;

     // 1. 获取 json value 的长度，为输出的 payload 申请堆栈空间 malloc.
      
      APC_PACKET *apc_packet = (APC_PACKET*)sbuff;
      memset(apc_packet,0,255);

      log_level(U_LOG_DEBUG,"cmd = %d\n",cmdindex);
      switch(cmdindex){
        case SKILL_CMD_TREE_TIMER:
        {
            int v = _resquest_payloadValue_integer_get(req,"value");
            if(v > 0 && v < 24){
              apc_packet->version = UART_VERSION;
              apc_packet->cmd = 0;
              apc_packet->idx = 0;
              apc_packet->len = 1;
              apc_packet->payload[0]  = (u8)v;
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


// 1. 解析jason 的function 转换为u32 设备可以理解的形式。
// 2. 解析 value，并把内容封装到 alloc 的地址里以便发送到设备。
int _factory_evalogik_payload_alloc(Product_CtlInfo *p_ctl_info,DEV_CmdInfo *p_cmdinfo){

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


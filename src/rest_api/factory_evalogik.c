/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    factory_evalogik.c

   1.declere rest api.

    TIME LIST:
    CREATE  skyli   2017-10-23 17:30

*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include "util.h"
#include "factory_evalogik.h"

#define _PROTO_EVALOG_UART_VERSION	0x55


typedef struct{
    u8 version;          // 验证数,表示协议和协议版本
    u8 cmd;            // 命令类型 
    u8 idx;			// 序号, 预留给 sensor 使用
	u8 len;            // 数据长度
    char payload[0];    // 数据
} APC_EVA_PACKET;

// 1. 获取 json value 的长度，为输出的 payload 申请堆栈空间 malloc.
// 2. 匹配 function 转换为 硬件能识别的功能指令.
// 3. 封装 payload
// 4. 输出.
typedef enum UART_CMD_T{
	_EVALOG_CMD_NONE  = 0x00,   // 空类型
	_EVALOG_CMD_ON	  = 0x10,   //控制开
	_EVALOG_CMD_OFF	  = 0x11,	//控制关
  	_EVALOG_CMD_TREE_TOP_ON	  = 0x12,	// 控制树顶开关开
	_EVALOG_CMD_TREE_TOP_OFF  = 0x13,	// 控制树顶开关关
	_EVALOG_CMD_COLOR_CHANGE  = 0x14,	// 控制颜色转换
	_EVALOG_CMD_TWINKLE = 0x15,	// 控制闪烁
	_EVALOG_CMD_TIMER = 0x16, //控制定时
	_EVALOG_CMD_MAX
}UART_Cmd_T;

static int _christmas_tree_ctl_payload_alloc(Product_CtlInfo *p_ctl_info,DEV_CmdInfo *p_cmdinfo){

     // 1. 获取 json value 的长度，为输出的 payload 申请堆栈空间 malloc.
    u8 *p_ret_payload = NULL,*p_svalue,*p_scmd;
    
    if(!p_cmdinfo || !p_ctl_info ){
        return U_ERROR_INVALID;
    }
    p_scmd = _jason_str_get_value(p_ctl_info->j_function ,REQUEST_JKEY_FUNC_CMD);
    if( !p_scmd ){
        log_level(U_LOG_WARN,"Can't find any function!!");
        return U_ERROR_NULL;
    }

    APC_EVA_PACKET *apc_packet = (APC_EVA_PACKET*)umalloc(sizeof(APC_EVA_PACKET)+sizeof(u32));
    if(!apc_packet){
        log_level(U_LOG_ERROR,"malloc failt !!");
        return U_ERROR_NULL;
    }

    log_level(U_LOG_DEBUG,"cmd = %s\n",p_scmd);
    
     p_cmdinfo->cmd = CMD_SEND_SERIAL;
    apc_packet->version = _PROTO_EVALOG_UART_VERSION;
    
    if(str_equal(p_scmd,_EVALOGIK_FUNCTION_TURNON)){
        apc_packet->cmd = _EVALOG_CMD_ON;
    }
    else if(str_equal(p_scmd,_EVALOGIK_FUNCTION_TURNOFF)){
        apc_packet->cmd = _EVALOG_CMD_OFF;
    }
    else if(str_equal(p_scmd,_EVALOGIK_FUNCTION_COLOR_CHANGE)){
        apc_packet->cmd = _EVALOG_CMD_COLOR_CHANGE;
    } else if(str_equal(p_scmd,_EVALOGIK_FUNCTION_LIGHTTIME)){
        int v = json_integer_get(p_ctl_info->j_function,REQUEST_JKEY_FUNC_VALUE);
        apc_packet->cmd = _EVALOG_CMD_TIMER;

        // 时间 超出限制
        if(v < 0 && v > 24){
            ufree(apc_packet);
            p_cmdinfo->cmd = 0;
            return U_ERROR_NULL;
        }
        
        apc_packet->len = 1;
        apc_packet->payload[0]  = (u8)v;
    } else if(str_equal(p_scmd,_EVALOGIK_FUNCTION_FLASH)){
        apc_packet->cmd = _EVALOG_CMD_TWINKLE;
    }

    p_cmdinfo->p_payload = apc_packet;
    p_cmdinfo->len_paylod = sizeof(APC_EVA_PACKET) + apc_packet->len;
    return 0;
}

int _factory_evalogik_payload_alloc(Product_CtlInfo *p_ctl_info,DEV_CmdInfo *p_cmdinfo){

    return _christmas_tree_ctl_payload_alloc(p_ctl_info,p_cmdinfo);
}
// 1. 解析jason 的function 转换为u32 设备可以理解的形式。
// 2. 解析 value，并把内容封装到 alloc 的地址里以便发送到设备。
#if 0
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

#endif

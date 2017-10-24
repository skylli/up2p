/*******************************************************************************

    This file is part of the project.
    Copyright wilddog.com
    All right reserved.

    File:    ulink.h

    No description

    TIME LIST:
    CREATE  skyli   2014-08-27 13:17:06

*******************************************************************************/
#ifndef _aplication_h_
#define _aplication_h_

#ifdef __cplusplus
extern "C" {
#endif
#include "up2p.h"

#define UART_VERSION	0x55
// 指令类型
typedef enum APC_CMD_T{
    // 不加密命令
    APC_CMD_NONE                = 0x00,   // 空类型
	APC_CMD_SOCKET_ON			= 0x10,   //控制开关
	APC_CMD_SOCKET_OFF			= 0x11,	
    APC_CMD_COLOR_SET			= 0x20,	// 控制发送串口
	
	APC_CMD_MAX
}APC_CMD_T;
    
	
typedef struct{
    u8 version;          // 验证数,表示协议和协议版本
    u8 cmd;            // 命令类型 
    u8 idx;			// 序号, 预留给 sensor 使用
	u8 len;            // 数据长度
    char payload[0];    // 数据
} APC_PACKET;

#if 0

typedef struct{
    double h;
    double s;
    double b;
} APC_HSB_T;

typedef struct{
    u8 r;
    u8 g;
    u8 b;
} APC_RGB_T;

#endif

#ifdef __cplusplus
}
#endif

#endif // end _aplication_h_
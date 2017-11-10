/*******************************************************************************

    This file is part of the up2p.
    Copyright showhome
    All right reserved.

    File:    up2p.h

    No description

    TIME LIST:
    CREATE  skyli   2017-06-26 13:46:05

*******************************************************************************/
#ifndef _up2p_h_
#define _up2p_h_

#ifdef      __cplusplus
extern "C" {
#endif

#include "config.h"
// 公共系统头文件
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef BOARD_ESP_8266

#include <memory.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <pthread.h>


// todo 
#ifndef __ANDROID__
#include <ifaddrs.h>
#else
#include <android/log.h>
#endif
#include <netdb.h>

#else // in esp8266
//#ifdef  esp 8266 
#include "osapi.h"
#include "os_type.h"
#include "user_interface.h"

#endif //ifndef  BOARD_ESP_8266

#define SOCKET int
typedef struct sockaddr SOCKADDR;
#define closesocket close

// 版本识别码
#define MAGIC 0x84F5AD94

//最大的网络包
//#define MAX_NET_PACKAGE_SIZE	(512)

// 最大的串口数据包大小
#define MAX_SERIAL_SIZE 1024

// ULINK连接超时时长(秒)
#define ULINK_OPEN_TIMEOUT  10

// wait函数的超时时长(秒)
#define MAX_WAIT_TIME 10

// 配置模块最大时长(秒)
#define MAX_CONFIG_TIME 120

// 在线时间(秒)
#define UP2P_ONLINE_TIME 60
#define SYSTIMERUIN      1000  //系统时间单位s
#define ONLINE_INTERVAL_SEC 3

//一个sock 占用的内存 单位为bytes
#define  SIZEOFSOCK   (sizeof(UDP_INFO))


// device id  地址分段

#define UP2PS_URL "www.wilddog.com"

/*  cmd**/
#define CMD_SEND_SERIAL_ACK_STRING     "ACK"
#define CMD_SERIAL_BUSY_STRING         "BUSY "

#define CMD_LAN_SCAN_ACK_STRING        "LAN "
#define CMD_SEND_KEY_STRING            "KEY "

#ifndef BOARD_ESP_8266

#if __x86_64__ || __LP64__
#pragma message("In 64Bit machine \n")

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

#else

#pragma message("In 32Bit machine \n")

// 基本类型
typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef long s32;
typedef unsigned long u32;

#endif // 32 bit or 64 bit

#endif //#ifndef BOARD_ESP_8266  

typedef size_t (*U_Func_T)();

 
// 指令类型
typedef enum U_CMD_T{
    // 不加密命令
    CMD_NONE                = 0x0000,   // 空类型
    CMD_ONLINE              = 0x0010,   // 在线指令
    CMD_ONLINE_ACK          = 0x0011,   // 在线回应
    CMD_CHECK_ONLINE        = 0x0020,   // 检查设备是否在线
    CMD_CHECK_ONLINE_YES    = 0x0021,   // 确认在线
    CMD_CHECK_ONLINE_NO     = 0x0022,   // 确认不在线
    CMD_PING                = 0x0030,   // 发送测试指令
    CMD_PING_ACK            = 0x0031,   // 测试回应
    CMD_LAN_SCAN            = 0x0040,   // 扫描局域网设备
    CMD_LAN_SCAN_ACK        = 0x0041,   // 回应扫描
    CMD_GET_HOST            = 0x0050,   // 获取服务器地址
    CMD_GET_HOST_ACK        = 0x0051,   // 返回服务器地址

    // 标识加密命令
    CMD_DATA                = 0x1000,   // 发送加密数据
    CMD_DATA_ACK            = 0x1001,   // 回应加密数据
    CMD_DATA_KEY_ERR        = 0x1002,   // 回应密钥或者令牌错误

    // 加密内部命令
    CMD_UPDATE_TOKEN        = 0x2000,   // 刷新token
    CMD_UPDATE_TOKEN_ACK    = 0x2001,   // 返回刷新的token
    CMD_SEND_KEY            = 0x2010,   // 发送新的KEY到客户端
    CMD_SEND_KEY_OK         = 0x2011,   // 客户端已经接受新的KEY
    CMD_CONFIG_HOST         = 0x2020,   // 发送配置服务器参数
    CMD_CONFIG_HOST_ACK     = 0x2021,   // 回应配置服务器参数成功
    CMD_GET_TOKEN           = 0X2030,
    CMD_GET_TOKEN_ACK       = 0X2031,

    // 状态更新命令    
    CMD_STATUS_PUSH       = 0X2041,
    CMD_STATUS_PUSH_ACK       = 0X2041,
    
    // 加密用户命令
    CMD_SEND_SERIAL         = 0x3000,   // 发送串口数据
    CMD_SEND_SERIAL_ACK     = 0x3001,   // 发送串口数据回应
    CMD_READ_SERIAL         = 0x3010,   // 读取串口数据
    CMD_READ_SERIAL_ACK     = 0x3011,   // 读取串口数据回应
    CMD_TXRX_SERIAL         = 0x3020,   // 以收发一体的形式发送串口数据
    CMD_TXRX_SERIAL_ACK     = 0x3021,   // 返收发一体的形式回应串口数据
    CMD_UPATA_SERIAL        = 0x3100,   // 上传串口数据
    CMD_UPATA_SERIAL_ACK    = 0x3101,   // 上传串口数据回应
    

    CMD_WIFISDKVER_QURY     = 0x3030,   // 询问wifi sdk 的版本信息
    CMD_WIFISDKVER_QURY_ACK = 0x3031,   // 询问wifi sdk 的版本信息回应
    CMD_WIFIUPDATE_TRI      = 0x3040,   // 触发更新wifiSDK命令
    CMD_WIFIUPDATE_TRI_ACK  = 0x3041,   // 回应更新
    CMD_WIFIUPDATE_DATA     = 0x3050,   // 传输wifi更新数据
    CMD_WIFIUPDATE_DATA_ACK = 0x3051,   // 传输wifi更新数据回应

    CMD_GPIO_INIT           = 0x3060,   // 配置gpio
    CMD_GPIO_INIT_ACK       = 0x3061,   // 配置gpio 回应
    CMD_GPIO_READ           = 0x3070,   // 读gpio
    CMD_GPIO_READ_ACK       = 0x3071,   // 读gpio 回应
    CMD_GPIO_WRITE          = 0x3080,   // 写gpio
    CMD_GPIO_WRITE_ACK      = 0x3081,   // 写gpio 回应
    CMD_PWM_INIT            = 0x3090,   // 配置pwm
    CMD_PWM_INIT_ACK        = 0x3091,   // 配置pwm 回应
    CMD_PWM_READ            = 0x30A0,   // 读pwm参数
    CMD_PWM_READ_ACK        = 0x30A1,   // 读pwm参数回应
    CMD_PWM_WRITE           = 0x30B0,   // 写pwm 参数
    CMD_PWM_WRITE_ACK       = 0x30B1,   // 写pwm 参数回应
    CMD_I2C_INIT            = 0x30C0,   // 配置I2C
    CMD_I2C_INIT_ACK        = 0x30C1,   // I2C配置回应
    CMD_I2C_READ            = 0x30D0,   // 读取i2c数据
    CMD_I2C_READ_ACK        = 0x30D1,   // 读取回应
    CMD_I2C_WRITE           = 0x30E0,   // 写i2c数据
    CMD_I2C_WRITE_ACK       = 0x30E1,   // 写i2c数据回应
    CMD_I2C_TXRX            = 0x30F0,   // 写i2c等待数据
    CMD_I2C_TXRX_ACK        = 0x30F1,   // 写读i2c数据回应

    CMD_FILE_NAME           = 0x30F2,   //传输文件名 接下来开始传输  内容
    CMD_FILE_NAME_ACK       = 0x30F3,
    CMD_FILE_TXT            = 0x30F4,   // 传输文件内容
    CMD_FILE_TXT_ACK        = 0x30F5,

    CMD_DEV_STATE_OBSERVER        = 0x3100,   //  订阅设备的状态
    CMD_DEV_STATE_OBSERVER_ACK    = 0x3101,   //  订阅设备的状态 回应
    CMD_DEV_STATE_UNOBSERVER      = 0x3102,   //  取消订阅
    CMD_DEV_STATE_UNOBSERVER_ACK  = 0x3103,  //  
    CMD_DEV_STATE_UPDATE        = 0x3104,   //  状态更新上报
    CMD_DEV_STATE_UPDATE_ACK    = 0x3105,  //  
    

    CMD_MAX
}U_Cmd_T;
    
typedef enum{
    CMD_TYPE_NONE = 0,
    CMD_TYPE_OBSERVER,
    CMD_TYPE_MAX
}CMD_TYPE;

/* 返回吗，用于判断错误.*/
typedef enum U_RETURN_T
{
    U_ERROR_NONE = 0,
    U_ERROR_NULL = -1,
    U_ERROR_INVALID = -2,
    U_ERROR_ALLOC = -3,
    U_ERROR_SOCKET = -4,
    U_ERROR_LIINKDEV = -5,
    U_ERROR_SERVER_REQUEST_PAYLOAD = -6,

    U_ERROR_RESPON_BAD_REQUEST = -7,
    U_ERROR_RESPON_NON_DEVICEREQPON = -8,
    U_ERROR_MAX
}U_Return_T;

typedef enum U_ONLINE_STATE_T{
    U_STATE_OUTLINE=0,
    U_STATE_WAN=1,
    U_STATE_LAN=2,
    U_STATE_MAX
}U_Online_State_T;
    
#ifdef __ANDROID__
#define up2p_printf(...)  __android_log_print(ANDROID_LOG_ERROR,"up2pa::", __VA_ARGS__)
#else

#define up2p_printf     printf

#endif // __ANDROID__


#define W_DEBUG

#define U_LOG_ALL    0
#define U_LOG_DEBUG  1
#define U_LOG_INFO    2
#define U_LOG_WARN   3
#define U_LOG_ERROR  4
#define U_LOG_NONE  5

#define DEBUG_LEVEL U_LOG_INFO

/********************************log thing***********************************************************/
#define LOG_INTIP2STR(a)  do{\
        up2p_printf("%d.%d.%d.%d",a[0],a[1],a[2],a[3]); \
    }while(0)
/********************************log thing end***********************************************************/

static const char *s_debug[] = {
    "[ ALL ]",
    "[DEBUG]",
    "[ LOG ]",
    "[WARN ]",
    "[ERROR]",
    "[ NON ]"
};

#ifdef W_DEBUG


#define __FILENAME__ (strrchr(__FILE__, '/')? strrchr(__FILE__, '/') + 1 : __FILE__) 


#define w_debug_level(level, format,...)    do{\
        if(level >= DEBUG_LEVEL){ \
            up2p_printf("%s %s func:%s LINE: %d: "format"\r",s_debug[level],__FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
        }}while(0)

#define w_debug(format,...) w_debug_level(U_LOG_DEBUG,format,##__VA_ARGS__)
#define w_dump(s,p,l) do{\
                if( DEBUG_LEVEL >= U_LOG_DEBUG){\
                    int i; \
                    up2p_printf("%s len = : %d \n",s,l);\
                    for(i=0;i<l;i++) up2p_printf("[%x]",(unsigned char)p[i]);\
                    up2p_printf("%s end\n",s);  \
                }\
            }while(0)
#else
#define w_debug_level(level, format,...) 
#define w_debug(format,...) do{up2p_printf("debug::"format"\r\n", ##__VA_ARGS__);}while(0)

#define w_dump(s,p,l) 
#endif  //W_DEBUG


// 以下为核心层定义

// UDP相关参数
typedef struct{
    u32 sip;        // 源IP
    u16 sport;      // 源端口
    u32 dip;        // 目标IP
    u16 dport;      // 目标端口
} UDP_INFO;

// 总包
typedef struct{
    u32 magic;          // 验证数,表示协议和协议版本
    u32 dst0;           // 设备名0
    u32 dst1;           // 设备名1，
    u32 src0;           // 源设备名0
    u32 src1;           // 源设备名1，源和目的设备号用于包的设备的控制管理
    u32 cmd;            // 命令类型,非加密命令
    u16 idx;            // 序号
    u16 len;            // 数据长度
    char payload[0];    // 数据
} UP2P_PACKET;

// 加密传输的数据包
typedef struct{
    u32 token;          // 令牌
    u32 cmd;            // 命令
    u16 len;            // 数据长度
    char payload[0];    // 数据
} UP2P_DATA;

// 获取服务器信息附加包
typedef struct{
    u32 dev0;
    u32 dev1;
} UP2P_DEVID;

// 服务器地址信息
typedef struct{
    u32 ip;
    u16 port;
} UP2P_HOST;

// 以下为应用层定义

// 设置密钥数据包
typedef struct{
    u32 key0;
    u32 key1;
} UP2P_SET_KEY;

// 令牌数据包
typedef struct{
    u32 token;
} UP2P_TOKEN;

// 配置服务器信息
typedef struct{
    u16 port;       // 服务器端口号
    char name[64];  // 服务器IP或者域名字符串
} UP2P_CONFIG_HOST;

typedef struct{
	u8 pin;
	u8 mode;
}UP2P_IO_MODE

;

typedef struct{
	u8 pin;
	u8 val;
}UP2P_IO;

//gpio 
// GPIO 数据结构体
typedef struct{
	u8 pin;		// gpio pin 脚号
	u8 mode;	// gpio 工作模式（输入/输出）
	u8 value;	// gpio pin 脚当前电平值
} UP2P_GPIO;

typedef enum FILE_CMD{
    FILE_CMD_NONE,
    FILE_CMD_TXT,

    FILE_CMD_ACK,
    FILE_CMD_RET,
    FILE_CMD_CAN,
    FILE_CMD_END,

    
    FILE_CMD_MAX
}File_CMD;

typedef struct{
    u8 type;
    u8 index;
    u16 len;
    u8 payload[0];
} FILE_T;

void delay_ms(u32 ms);

#ifdef __cplusplus
}
#endif

#endif

/*******************************************************************************

    This file is part of the project.
    Copyright wilddog.com
    All right reserved.

    File:    ulink.h

    No description

    TIME LIST:
    CREATE  skyli   2014-08-27 13:17:06

*******************************************************************************/
#ifndef _ulink_h_
#define _ulink_h_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _up2p_h_

// 最大的串口数据包大小
#define MAX_SERIAL_SIZE 1024

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef long s32;
typedef unsigned long u32;

void delay_ms(u32 ms);

// 指令类型
enum{
    CMD_SEND_SERIAL         = 0x3000,   // 发送串口数据
    CMD_SEND_SERIAL_ACK     = 0x3001,   // 发送串口数据回应
    CMD_READ_SERIAL         = 0x3010,   // 读取串口数据
    CMD_READ_SERIAL_ACK     = 0x3011,   // 读取串口数据回应
    CMD_TXRX_SERIAL         = 0x3020,   // 以收发一体的形式发送串口数据
    CMD_TXRX_SERIAL_ACK     = 0x3021,   // 返收发一体的形式回应串口数据
    CMD_UPATA_SERIAL        = 0x3100,   // 上传串口数据
    CMD_UPATA_SERIAL_ACK    = 0x3101,   // 上传串口数据回应
};

//gpio 控制模式
enum{
    GPIO_=0,        //普通gpio 输入输出
    GPIO_OUTPUT,
    GPIO_INPUT,
    GPIO_INTRTRI,   //中断输入
    GPIO_PWM,       //pwm模式
    GPIO_I2C_SCL,   // i2c时钟
    GPIO_I2C_SDA    //i2c数据脚
};

#endif // endif _up2p_h_

typedef struct ULINK ULINK;

/*
 * ulink_config 返回值
 */
typedef enum{
    ULINK_ERR_NONE = 0 ,                 // 成功
    ULINK_ERR_DEVID_INVILD = -1,         // 无效的devid
    ULINK_ERR_DEV_OFFLINE  = -2,          // 设备不在线
    ULINK_ERR_SERVER_OFFLINE = -3,       // 服务器不在线, 或者广域网络不通
    ULINK_ERR_INIT_TOKEN = -4,           // 初始化令牌错误, 可能是模块已经被配置过
    ULINK_ERR_CONFIG_HOST = -5,          // 配置服务器错误, 可能是模块网络不好或者flash写失败
    ULINK_ERR_INIT_KEY = -6,             // 初始化密钥错误, 可能是模块网络不好

    ULINK_ERR_TOKEN_NOMATCH = -7,        //  session token 不符
    
    
    
} ULINK_ERR;

/*
 * description: 初始化库
 * argv:
 *  host 服务器域名或者IP地址
 *  appid 代表本机唯一的ID码 16字节HEX字符串
 * return: 0成功
 */
int ulink_init(const char *host, const char *appid,u16 sport);

/*
 * 卸载库
 */
int ulink_deinit();

// WLAN加密类型
// 不加密的方式使用"ULINK_AUTH_OPENWEP", key设置为NULL
#define ULINK_AUTH_OPENWEP          0
#define ULINK_AUTH_SHAREKEY         1
#define ULINK_AUTH_AUTOSWITCH       2
#define ULINK_AUTH_WPA              3
#define ULINK_AUTH_WPAPSK           4
#define ULINK_AUTH_WPANONE          5
#define ULINK_AUTH_WPA2             6
#define ULINK_AUTH_WPA2PSK          7
#define ULINK_AUTH_WPA1WPA2         8
#define ULINK_AUTH_WPA1PSKWPS2PSK   9
/*
 * description : 连接设备
 * arg：
 *      devid：16字节的设备ID字符串
 *      ssid：将要连接到的接入点SSID
 *      key：接入点的密钥
 *      type：加密类型
 * return： ULINK_ERR 当且仅当返回ULINK_ERR_NONE时表示成功
 * outkey 返回密钥字符串
 */
int ulink_config(const char *devid, const char *ssid, const char *key, int type, char *outkey);

/*
 * description: 连接到设备
 * argv:
 *      devid:目标设备ID 
 *      key: 密钥 (均为8字节HEX字符串)
 * return: 返回 ulink 指针.
 */
ULINK *ulink_open(const char *devid, const char *key);

/*
 * description: 关闭连接,释放该链接的资源
 *  argv:
 *      ulink: ulink指针
 */
int ulink_close(ULINK *ulink);

/*
 * description: 发送命令和数据
 *  argv:
 *      ulink ulink指针
 *      cmd 命令字
 *      param 附加数据
 *      len 附加数据长度
 *      返回已发送字节数，-1为失败
 */
int ulink_cmd_send(ULINK *ulink, u32 cmd, void *param, int len);

/*
 * description: 阻塞等待数据包，最多阻塞 MAX_WAIT_TIME.等待过程中会重发请求包。
 * argv: 
 *      ulink ulink指针
 *      cmd 命令字
 *      param 附加数据缓冲区
 *      maxlen 缓冲区长度
 * return: 返回收到的附加数据长度 -1 失败
 */
int ulink_cmd_wait(ULINK *ulink, u32 cmd, void *param, int maxlen);

/*
 * description: 阻塞等待数据包，最多阻塞 MAX_WAIT_TIME.没有重发动作.
 * argv: 
 *      ulink ulink指针
 *      cmd 命令字
 *      param 附加数据缓冲区
 *      maxlen 缓冲区长度
 * return: 返回收到的附加数据长度 -1 失败
 */
int ulink_cmd_wait_noRetry(ULINK *ulink, u32 cmd, void *param, int maxlen);

/*
 *decriptions: 发送获取设备的在线状态请求,接收线程收到回包后，会自动更新设备状态到本地
 *  argv:
 *        ulink: 当前回话的结构指针.
 * return: -1 为失败，否则成功发送.
 *notice: 应用应该先调用 ulink_check_online 查询本地记录的设备状态，再调用该接口更新这边状态.
 */
int ulink_get_devState(ULINK *ulink);

/*
 * description：查询本地记录的设备状态.
 * return: 0 不在线 1 广域网在线 2 局域网在线 -1 服务器无响应
 */
int ulink_check_online(ULINK *ulink);

/*
 * 边界保护开始
 * 在多线程中通讯序列使用此函数作为开始
 */
void ulink_border_begin();

/*
 * 边界保护结束
 * 在多线程中通讯序列使用此函数作为结束
 */
void ulink_border_end();

/*
 * 调试信息输出, 0为关闭, 大于0为输出
 */
int ulink_debug(int level);


#ifdef __cplusplus
}
#endif

#endif // end _ulink_h_
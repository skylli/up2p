#ifndef _CONFIG_H
#define _CONFIG_H

#ifdef __cplusplus
extern "C"{
#endif

/* board define ***/
// suport esp8266
//#define BOARD_ESP_8266  

//#define __ANDROID__
#define MAC_LINUX
#define MAX_NET_PACKAGE_SIZE  512


#define MAX_RETRANSMITS (3)
#define MAX_RERANSMITS_TIMEOUT_ms  (3000)  

#define INTERVAL_RETRANSMIT_MS  (500)

#define SELF_TEST

/************ device id 分配 ******
* device id 一共 16 个字符，
------------------------------
|4位端标识|12位 mac 地址|
-------------------------
********/
#define HTTPS_REST_PORT 9510 // 监听和处理 http 请求.

#define ID_DEVICE_SMARTHOME (0X10)
#define ID_DEVICE_SERVER    (0X20)
#define ID_DEVIDE_REST      (0X30)

// dev  中转服务器 
#define DEVICE_SERVER0  ("127.0.0.1")

// 端口配置
#define UP2PC_PORT 9527
#define UP2PA_PORT 9528
#define UP2PS_PORT 9529
#define UP2PS_REST_PORT (9511)
/***** rest server setting */
// rest 服务最多缓存的请求节点，最大的并发量
#define REST_MAC_CONCURRENT (1000)

// 最大的订阅数量
#define MAX_OBSERS  (3)
#ifdef __cplusplus
}
#endif
#endif //_CONFIG_H

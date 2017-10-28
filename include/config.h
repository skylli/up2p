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

#define HTTPSERVER_PORT 9510

#define MAX_RETRANSMITS (3)
#define MAX_RERANSMITS_TIMEOUT_ms  (3000)  

#define INTERVAL_RETRANSMIT_MS  (500)

#define SELF_TEST
// 最大的订阅数量
#define MAX_OBSERS  (3)
#ifdef __cplusplus
}
#endif
#endif //_CONFIG_H

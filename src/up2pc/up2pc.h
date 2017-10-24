/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    up2pc.h

    No description

    TIME LIST:
    CREATE  skyli   2017-05-06 13:47:55

*******************************************************************************/
#ifndef _up2pc_h_
#define _up2pc_h_

#ifdef      __cplusplus
extern "C" {
#endif

#include "up2p.h"

#define VER_ESP_SDK    "VM101_HQ1.5.02"  //20140820

    
//------------------内部函数-----------------
/*
 * 接收数据处理函数
 */
int up2pc_recv(const char *data, int len, UDP_INFO *info);

/*
 * 定时更新在线状态的回调函数
 * 至少30秒调用一次
 */
int up2pc_set_online();

// 设置用户密钥
int up2pc_setkey(u32 key0, u32 key1);

/*
 * 查询在线状态
 * 返回0 在线
 * -1 不在线
 */
int up2pc_get_online();

// 设置服务器ip和端口
int up2pc_set_server(u32 ip, u16 port);

// 设置本机ip和端口
int up2pc_set_local(u32 ip, u16 port);

// 设置本机设备名
int up2pc_set_name(u32 dev0, u32 dev1);

/*
 * 更新服务器信息
 * 由应用层在长时间无法连接时调用(例如重新解析完DNS之后)
 * 之前必须设置过dev0 dev1
 */
int up2pc_update_host();

// 配置服务器信息
int sys_config_host(char *host, u16 port);

// 设置接受串口数据的设备名
int up2pc_set_serialRead_server(UDP_INFO *info,u32 dev0,u32 dev1);
int up2pc_udp_recv( char *recv_data,int buffer_size, int *plen, UDP_INFO *info);

//io modo init , mode was input or outuput
int io_mode_int(u8 pin,u8 mode);

//io wirte
int io_write(u8 pin,u8 val);

int up2pc_update_devState_ack(u32 sr0,u32 sr1,u32 ip, u16 port);

// update dev status 
int up2pc_update_dev_status(u8 *pdata,u32 len);
// retransmit 
int up2pc_retransmit(void);
int up2pc_observer_host_register(u32 sr0,u32 sr1,u32 ip, u16 port);
int up2pc_observer_host_unregister(u32 sr0,u32 sr1,u32 ip, u16 port);

//--------------外部函数-----------------------------

int udp_soket_init(u16 lport);

// UDP发送数据
int up2pc_udp_send(const char *data, int len, UDP_INFO *info);
//
//int udp_recv( char **recv_data, int len, UDP_INFO *info);

u32 dns_ip(const char *sname);

// 往串口写数据
int serial_write(const char *data, int len);

// 从串口读数据
int serial_read(char *data, int len);

// 系统获取时间戳(精确到秒)
u32 sys_get_time();

// 系统获取32位随机数
u32 sys_get_random();

// 保存密钥
int sys_set_key(u32 key0, u32 key1);
//外部函数
int Xmodem_Update_FW(u8 const *xbuff,u8 *txbuff);
int updatewifi_int(void);
int  updatestar_wifi(void);
int  updateing_wifi(void);
int setupdateing_wifi(void);
int updatewifistar_int(void);
int creatnewbinfile(char const *filename);

#ifdef __cplusplus
}
#endif

#endif

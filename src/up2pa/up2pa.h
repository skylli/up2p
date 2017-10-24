/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    up2pa.h

    No description

    TIME LIST:
    CREATE  skyli   2017-05-06 13:47:13

*******************************************************************************/
#ifndef _up2pa_h_
#define _up2pa_h_

#ifdef      __cplusplus
extern "C" {
#endif

#include "up2p.h"
#include "util.h"
/*
 * 初始化
 * sport 本地绑定端口 sname 服务器域名或者IP dport 服务器端口 cport客户端的端口
 */
int up2pa_init(u16 sport, const char *sname, u16 dport, u16 cport, u32 mac0, u32 mac1);

/*
 * 卸载
 */
int up2pa_deinit();

/*
 * 发送检查在线状态的数据包, 查询在线状态之前先调用
 */
int up2pa_send_online(u32 dev0, u32 dev1);

/*
* 检查在线状态
* 返回值 0 不在线 1 在线 -1 服务器无响应
*/
int up2pa_check_online(u32 dev0, u32 dev1);

/*
 * 清理在线状态
 */
int up2pa_clear_online(u32 dev0, u32 dev1);

/*
 * 发送远程指令
 * cmd 指令编码 param 指令附加数据 len 附加数据长度
 * 没有附加数据 param 可以为NULL, len为0
 */
int up2pa_send_cmd(u32 dev0, u32 dev1, u32 cmd, const char *param, int len);

/*
 * 发送带索引的远程指令
 * idx 索引号 cmd 指令编码 param 指令附加数据 len 附加数据长度
 * 没有附加数据 param 可以为NULL, len为0
 */
int up2pa_send_idx_cmd(u32 dev0, u32 dev1, u16 idx, u32 cmd, const char *param, int len);

/*
 * 发送加密远程指令
 * token 令牌 cmd 指令编码 param 指令附加数据 len 附加数据长度 
 * 没有附加数据 param 可以为NULL, len为0
 */
int up2pa_send_data_cmd(u32 dev0, u32 dev1, u32 key0, u32 key1, u32 token, u32 cmd, const char *param, int len);

/*
 * 读取返回指令
 * data 接收缓存 maxlen 缓存区最大长度
 * 返回值 附加数据的长度 长度也可能为0 -1 无数据
 */
int up2pa_recv_cmd(CMD_TYPE cmd_type,u32 dev0, u32 dev1, u32 *cmd, char *data, int maxlen);

/*
 * 读取带索引的返回指令
 * idx 索引号 data 接收缓存 maxlen 缓存区最大长度
 * 返回值 附加数据的长度 长度也可能为0 -1 无数据
 */
int up2pa_recv_idx_cmd(u32 dev0, u32 dev1, u16 idx, u32 *cmd, char *data, int maxlen);

/*
 * 阻塞并等待指定的指令
 * cmd 等待的指令 data 接收缓存 maxlen 缓存区最大长度
 * 返回值 附加数据的长度 长度也可能为0 -1 无数据
 */
int up2pa_wait_cmd(u32 dev0, u32 dev1, u32 cmd, char *data, int maxlen);

/*
 * 读取返回加密指令
 * data 接收缓存 maxlen 缓存区最大长度
 * 返回值 附加数据的长度 长度也可能为0 -1 无数据
 */
int up2pa_recv_data_cmd(CMD_TYPE cmd_type,u32 dev0, u32 dev1, u32 key0, u32 key1, u32 token, u32 *cmd, char *data, int maxlen);

/*
 * 阻塞并等待指定的加密指令
 * cmd 等待的指令 data 接收缓存 maxlen 缓存区最大长度
 * 返回值 附加数据的长度 长度也可能为0 -1 无数据
 */
int up2pa_wait_data_cmd(u32 dev0, u32 dev1, u32 key0, u32 key1, u32 token, u32 cmd, char *data, int maxlen);
int up2pa_wait_data_cmd_noRetry(u32 dev0, u32 dev1, u32 key0, u32 key1, u32 token, u32 cmd, char *data, int maxlen);

/*
 * 设置服务器参数
 */
int up2pa_set_host(u32 sip, u16 sport);

/*
 * 边界保护开始
 * 在多线程中通讯序列使用此函数作为开始
 */
void up2pa_border_begin();

/*
 * 边界保护结束
 * 在多线程中通讯序列使用此函数作为结束
 */
void up2pa_border_end();

/*
 * 调试信息输出, 0为关闭, 大于0为输出
 */
extern int up2pa_debug_level;



#ifdef __cplusplus
}
#endif

#endif

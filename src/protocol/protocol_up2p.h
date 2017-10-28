/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    protocol_up2p.h

    No description

    TIME LIST:
    CREATE  skyli   2017-05-06 13:47:55

*******************************************************************************/
#ifndef _protocol_up2p_h_
#define _protocol_up2p_h_

#ifdef      __cplusplus
extern "C" {
#endif

#include "up2p.h"

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
} PRO_UP2P_HEAD;

// 加密传输的数据包
typedef struct{
    u32 token;          // 令牌
    u32 cmd;            // 命令
    u16 len;            // 数据长度
    char payload[0];    // 数据
} PRO_UP2P_ENCRY_HEAD;

typedef struct{
    u32 dstdevid0;
    u32 dstdevid1;
    u32 mydevid0;
    u32 mydevid1;
    u32 aeskey0;
    u32 aeskey1;
    u32 token;
    u16 idx;
    
}Session_ST;


PRO_UP2P_HEAD *protocol_up2p_pack_alloc(Session_ST *p_session,u32 cmd,int *p_outlen,u8 *p_src,int slen);
PRO_UP2P_HEAD *protocol_up2p_encry_pack_alloc(Session_ST *p_session,u32 cmd,int *p_outlen,u8 *p_src,int slen);
int protocol_up2p_magic_notMatch(u8 *p_src,int slen);
int protocol_up2p_devid_match(u32 ldev0,u32 ldev1,u8 *p_src);
PRO_UP2P_ENCRY_HEAD *protocol_up2p_decode_alloc(Session_ST *p_session,int *p_len_dec,u8 *p_src,int slen);
int protocol_up2p_filter(Session_ST *p_session,u8 *p_src,int slen);
u8 *protocol_up2p_data_alloc(Session_ST *p_session,int *p_dlen,u8 *p_src,int slen);






#if 1

int unittest_protocol_test(void);

#endif

#ifdef __cplusplus
}
#endif

#endif //_protocol_up2p_h_

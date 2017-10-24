/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    up2pa.h

    No description

    TIME LIST:
    CREATE  skyli   2014-08-20 20:17:45

*******************************************************************************/
#ifndef _up2pa_crypt_h_
#define _up2pa_crypt_h_

#ifdef __cplusplus
extern "C"{
#endif

/*
 * 数据加密
 * src 源数据 dst 目标数据 len 源数据长度 key0 key1 密钥
 * 返回加密后的数据长度
 * 如果src不为16字节的整数倍,则补0后加密
 */
int data_enc(const char *src, char *dst, int len, u32 key0, u32 key1);

/*
 * 数据解密
 * src 源数据 dst 目标数据 len 源数据长度 key0 key1 密钥
 * 返回加密后的数据长度
 * 如果src不为16字节的整数倍,则补0后加密
 */
int data_dec(const char *src, char *dst, int len, u32 key0, u32 key1);

#ifdef __cplusplus
}
#endif

#endif

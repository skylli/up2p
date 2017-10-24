/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    up2pa.h

    No description

    TIME LIST:
    CREATE  skyli   2014-08-20 20:17:45

*******************************************************************************/

#include "up2p.h"
#include "aes.h"

/*
 * 数据加密
 * src 源数据 dst 目标数据 len 源数据长度 key0 key1 密钥
 * 返回加密后的数据长度
 * 如果src不为16字节的整数倍,则补0后加密
 */
int data_enc(const char *src, char *dst, int len, u32 key0, u32 key1)
{
    //  aes_context ctx;
    u32 newkey[4];
    u8  tmp_input[16];
    int count;

    newkey[0] = key0;
    newkey[1] = key1;
    newkey[2] = key0;
    newkey[3] = key1;

    //aes_set_key(&ctx, (unsigned char *)newkey, 128);
    
    count = 0;
    while(len >= 16)
    {
        //aes_encrypt(&ctx, (unsigned char*)(src + count), (unsigned char*)(dst + count));
        AES128_ECB_encrypt((uint8_t*)(src + count), (uint8_t*)newkey, (uint8_t*)(dst + count));
        count += 16;
        len -= 16;
    }
    if(len > 0)
    {
        memset(tmp_input, 0, 16);
        memcpy(tmp_input, &src[count], len);
        //aes_encrypt(&ctx, tmp_input, (unsigned char*)(dst + count));
        AES128_ECB_encrypt((uint8_t*)tmp_input,(uint8_t*) newkey, (uint8_t*)(dst + count));
        count += 16;
    }

    return count;
}

/*
 * 数据解密
 * src 源数据 dst 目标数据 len 源数据长度 key0 key1 密钥
 * 返回加密后的数据长度
 * 如果src不为16字节的整数倍,则补0后加密
 */
int data_dec(const char *src, char *dst, int len, u32 key0, u32 key1)
{
//  aes_context ctx;
    u32 newkey[4];
    u8  tmp_input[16];
    int count;

    newkey[0] = key0;
    newkey[1] = key1;
    newkey[2] = key0;
    newkey[3] = key1;

    //aes_set_key(&ctx, (unsigned char *)newkey, 128);
    
    count = 0;
    while(len >= 16)
    {
        //aes_decrypt(&ctx, (unsigned char*)(src + count), (unsigned char*)(dst + count));
        AES128_ECB_decrypt((uint8_t*)(src + count), (uint8_t*)newkey, (uint8_t*)(dst + count));
        count += 16;
        len -= 16;
    }

    return count;
}

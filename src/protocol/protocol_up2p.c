/*******************************************************************************
    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    protocol_up2p.c

    description: provide up2p package api. 

    TIME LIST:
    CREATE  skyli   2017-06-26 13:47:42

*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "protocol_up2p.h"
#include "util.h"

#include "up2p_crypt.h"


// 对输入的数据用 up2p 打包. 没有加密.
// 用完必须释放 p_dst
PRO_UP2P_HEAD *protocol_up2p_pack_alloc(Session_ST *p_session,u32 cmd,int *p_outlen,u8 *p_src,int slen){

    if( !p_outlen  ){
            log_level(U_LOG_WARN, "invalid input");
            return 0;
    }
    // 头部命令解析
    
    u8 *p_buf = umalloc((sizeof(PRO_UP2P_HEAD) + slen ));
    if( !p_buf ){
        log_level(U_LOG_ERROR,"malloc failt ");
        return 0;
    }
    memset(p_buf,0,sizeof(PRO_UP2P_HEAD) + slen);
    PRO_UP2P_HEAD *p_pack = (PRO_UP2P_HEAD*)p_buf;

    if(slen && p_src )
        memcpy(p_pack->payload,p_src,slen);
    
    p_session->idx++;

    p_pack->magic = MAGIC;
    p_pack->cmd = cmd;
    p_pack->idx = p_session->idx;
    p_pack->dst0 = p_session->dstdevid0;
    p_pack->dst1 = p_session->dstdevid1;
    p_pack->src0 = p_session->mydevid0;
    p_pack->src1 = p_session->mydevid1;
    p_pack->len = slen;

    *p_outlen = sizeof(PRO_UP2P_HEAD) + slen;
    
    return p_pack;
}

// 对输入的数据用 up2p 打包. 没有加密.
// 用完必须释放 p_dst
PRO_UP2P_HEAD *protocol_up2p_encry_pack_alloc(Session_ST *p_session,u32 cmd,int *p_outlen,u8 *p_src,int slen){

    if( !p_outlen  ){
            log_level(U_LOG_WARN, "invalid input");
            return U_ERROR_INVALID;
    }
    // 头部命令解析
    
    
    PRO_UP2P_HEAD *p_pack = umalloc( sizeof(PRO_UP2P_HEAD) + sizeof(PRO_UP2P_ENCRY_HEAD)+ slen + 20 );
    PRO_UP2P_ENCRY_HEAD *p_enc_src = umalloc( sizeof(PRO_UP2P_ENCRY_HEAD)+ slen + 20 );
    
    if( !p_pack || !p_enc_src ){
        log_level(U_LOG_ERROR,"malloc failt ");
        return U_ERROR_NULL;
    }

    // index incres
    p_session->idx++;
    // Creat packet head
    p_pack->magic = MAGIC;
    p_pack->cmd = CMD_DATA;
    p_pack->idx = p_session->idx;
    p_pack->dst0 = p_session->dstdevid0;
    p_pack->dst1 = p_session->dstdevid1;
    p_pack->src0 = p_session->mydevid0;
    p_pack->src1 = p_session->mydevid1;

    // creat encry
    p_enc_src->token = p_session->token;
    p_enc_src->cmd = cmd;
    p_enc_src->len = slen;

    if(slen && p_src)
        memcpy(p_enc_src->payload,p_src,slen);
    
    p_pack->len = data_enc((const char*)p_enc_src,p_pack->payload,(sizeof(PRO_UP2P_ENCRY_HEAD) + slen),p_session->aeskey0,p_session->aeskey1);

    // output 
    *p_outlen = sizeof(PRO_UP2P_HEAD) + p_pack->len;

    ufree(p_enc_src);
    p_enc_src = NULL;
    
    return p_pack;
}

// 解析 up2p 数据包,不会进行解密，仅仅是根据协议头过滤，由应用层过滤。
int protocol_up2p_magic_notMatch(u8 *p_src,int slen){
    if(!p_src || !slen){
        log_level(U_LOG_WARN,"invalid input");
        return 1;
    }

    PRO_UP2P_HEAD *p_pack = (PRO_UP2P_HEAD*)p_src;

    if(p_pack->magic == MAGIC){
        return 0;
    }
    else{
        log_level(U_LOG_INFO,"receive package magic was not match.");
        return 1;
    }
        
}
int protocol_up2p_devid_match(u32 ldev0,u32 ldev1,u8 *p_src){

    if(!p_src){
        log_level(U_LOG_WARN,"invalid input");
        return 0;
    }

    PRO_UP2P_HEAD *p_pack = (PRO_UP2P_HEAD*)p_src;

    if( ldev0 == p_pack->dst0 && ldev1 == p_pack->dst1){
        
            return 1;
    }
    
    log_level(U_LOG_WARN,"difference dev id recv %x  %x while my is %x %x",p_pack->dst0,p_pack->dst1,ldev0,ldev1);
    return 0;
}

u8 *protocol_up2p_payload_get(Session_ST *p_session,PRO_UP2P_HEAD *p_pack,int slen){

    if(!p_session || !p_pack ||  slen < sizeof(PRO_UP2P_HEAD)){
        return 0;
    }
    if(p_pack->payload && p_pack->len > 0)
        return p_pack->payload;
    return 0;
}
PRO_UP2P_ENCRY_HEAD *protocol_up2p_decode_alloc(Session_ST *p_session,int *p_len_dec,u8 *p_src,int slen){

    if( !p_session  || !p_src || !slen || !p_len_dec){
        log_level(U_LOG_WARN,"invalid input");
        return 0;
    }
    // decode 
    //decode 
    u8 *p_buf = umalloc(slen + 20);

    if( !p_buf){
        log_level(U_LOG_ERROR,"malloc failt !");
        return 0;
    }
    memset(p_buf,0,slen+20);
    int dec_len = data_dec(p_src,p_buf,slen,p_session->aeskey0,p_session->aeskey1);

    
    PRO_UP2P_ENCRY_HEAD *p_pack = (PRO_UP2P_ENCRY_HEAD*)p_buf;
    
    if(p_session->token != p_pack->token){
        log_level(U_LOG_INFO,"receive different token %x my token is %x",p_pack->token,p_session->token);
        return 0;
    }
    
    *p_len_dec = dec_len;
    return p_pack;
}
// 获取数据，如果有加密，则解密
// 返回的数据不再包含任何的协议头;=.
u8 *protocol_up2p_data_alloc(Session_ST *p_session,int *p_dlen,u8 *p_src,int slen){
    int len_payload = 0,len_dec= 0;
    u8 *p_payload = NULL,*p_data;
    PRO_UP2P_ENCRY_HEAD *p_dec = NULL;

    if( !p_src || !p_dlen || !slen){
        log_level(U_LOG_WARN,"invalue  input ");
        return NULL;
        }

    p_payload = protocol_up2p_payload_get(p_session,p_src,slen);
    PRO_UP2P_HEAD *p = (PRO_UP2P_HEAD*)p_src;
    len_payload = p->len;
    
    if(p->cmd >= CMD_DATA){
      p_dec = protocol_up2p_decode_alloc(p_session,&len_dec,p_payload,len_payload);
      if( p_dec  && len_dec){
            p_payload = p_dec->payload;
            len_payload = len_dec;
      }else{
            log_level(U_LOG_ERROR,"protocol_up2p_decode_alloc failt!!");
            return NULL;
      }
    }

    p_data = malloc(len_payload);
    if(p_data == NULL){
        log_level(U_LOG_ERROR,"memory alloc failt !!");
        ufree(p_dec);
        return NULL;
    }
    if(p_payload && len_payload){
       // log_level(U_LOG_DEBUG,"payload [%x][%x] len  = %d",p_data[0],p_data[1],len_payload);
        memcpy(p_data,p_payload,len_payload);
        }
    
    *p_dlen = len_payload;

    ufree(p_dec);
    return p_data;
}
// 根据 ulink 的 deviceid 和 index 过滤。
int protocol_up2p_filter(Session_ST *p_session,u8 *p_src,int slen){

    if( !p_session || !p_src || !slen){
        log_level(U_LOG_WARN,"invalid input");
        return 1;
    }
    PRO_UP2P_HEAD *p_pack = (PRO_UP2P_HEAD*)p_src;
    // dev 
    if( p_session->dstdevid0 != p_pack->src0 || p_session->dstdevid1 != p_pack->src1){
        
        log_level(U_LOG_INFO,"receive diff devid %x %x while my id is %x %x",p_pack->src0,p_pack->src1,p_session->dstdevid0,p_session->dstdevid1);
        return 1;
    }
    // index 
    if(p_session->idx != p_pack->idx){
        log_level(U_LOG_INFO,"index was not match %d my index is %d",p_pack->idx,p_session->idx);
        return 1;
    }

    return 0;
    // token 
    // get data 
}

#ifdef SELF_TEST

#define _TST_COUNT  2
u32 _test_devid[_TST_COUNT] = {1,2};
u32 _test_mydev[_TST_COUNT]  = {3,4};
u32 _test_aeskey[_TST_COUNT]= {0x55,0xaa};
u32 _test_token[_TST_COUNT] = {0x11,0x22};
u32 _test_idx[_TST_COUNT] = {3,4};
u32 _test_cmd[_TST_COUNT] = {CMD_CHECK_ONLINE,CMD_SEND_KEY};
u8 *str[_TST_COUNT] = {
        "test string 1",
        "string 2 test"};
int unittest_protocol_test(void){

    u8 *p_send[_TST_COUNT]= {},*p_recv[_TST_COUNT]={},*p_enc_data[_TST_COUNT] = {},*p_data[_TST_COUNT] = {};
    int len_send[_TST_COUNT],len_recv[_TST_COUNT] ,len_dec_data[_TST_COUNT],len_data[_TST_COUNT];
    u32 cmd_recv[_TST_COUNT];
    Session_ST ulink[_TST_COUNT];
    int i = 0,resoult[_TST_COUNT] = {};

    for(i=0; i<_TST_COUNT ;i++){

        resoult[i] = 0;
        ulink[i].dstdevid0 = _test_devid[i];
        ulink[i].dstdevid1 = _test_devid[i];
        ulink[i].mydevid0 = _test_mydev[i];
        ulink[i].mydevid1 = _test_mydev[i];
        ulink[i].aeskey0 = _test_aeskey[i];
        ulink[i].aeskey1 = _test_aeskey[i];
        ulink[i].token = _test_token[i];
        ulink[i].idx = _test_idx[i];   

        // pack up2p.
        if(_test_cmd[i] > CMD_DATA){
            p_send[i] = protocol_up2p_encry_pack_alloc(&ulink[i],_test_cmd[i],&len_send[i],str[i],strlen(str[i]));
        }
        else
            p_send[i] = protocol_up2p_pack_alloc(&ulink[i],_test_cmd[i],&len_send[i],str[i],strlen(str[i]));
        

        if(len_send[i] <= 0 || !p_send[i] ){
         log_level(U_LOG_ERROR,"up2p package 1 alloc failt !!");
         return -1;
        }

        ulink[i].mydevid0 = _test_devid[i];
        ulink[i].mydevid1 = _test_devid[i];
        ulink[i].dstdevid0 = _test_mydev[i];
        ulink[i].dstdevid1 = _test_mydev[i];
        // magic match 
        if(protocol_up2p_magic_notMatch(p_send[i],len_send[i])){
         log_level(U_LOG_ERROR,"package 1 magic was not match");
         return -1;
        }

        if( !protocol_up2p_devid_match(ulink[i].mydevid0,ulink[i].mydevid1,p_send[i])){
         log_level(U_LOG_ERROR,"packet  dev id was not match !!");
         return -1;
        }

        // source filter 
        if(protocol_up2p_filter(&ulink[i],p_send[i],len_send[i])){
         log_level(U_LOG_ERROR,"source device id was not match");
         return -1;
        }

        // get data and cmd 
        PRO_UP2P_HEAD *p_pack = (PRO_UP2P_HEAD*)p_send[i];

        p_recv[i] = protocol_up2p_payload_get(&ulink[i],p_send[i],len_send[i]);
        if( !p_recv[i]){
         printf("FAILT no receive anything\n");
         return -1;
        }

        p_data[i] = p_pack->payload;
        cmd_recv[i] = p_pack->cmd;

        len_dec_data[i] = NULL;
        if( p_pack->cmd == CMD_DATA ){
         // decodes 
         PRO_UP2P_ENCRY_HEAD *p_tm_dec = NULL; 
         
         printf("decoding \n");

         p_enc_data[i] = protocol_up2p_decode_alloc(&ulink[i],&len_dec_data[i],p_recv[i],p_pack->len);
         if(p_enc_data[i] && len_dec_data[i]){
             p_tm_dec = (PRO_UP2P_ENCRY_HEAD*)p_enc_data[i];
             p_data[i] = p_tm_dec->payload;
             cmd_recv[i] = p_tm_dec->cmd;
             }
        }


        printf(" cmd recv %x while my is %x \n",cmd_recv[i],_test_cmd[i]);
        if(cmd_recv[i] != _test_cmd[i]){
         printf("difference cmd recv %x while my is %x \n",cmd_recv[i],_test_cmd[i]);
         return -1;
        }

        //log_level(U_LOG_ERROR,"recv %s",p_data[i]);

        if(memcmp( p_data[i],str[i],strlen(str[i]))){
         printf("string was not match %s  !!== %s \n", p_data[i],str[i]);
         return -1;
        }
#if 1
        // test get data alloc 
        p_data[i] =  protocol_up2p_data_alloc(&ulink[i],&len_data[i],p_send[i],len_send[i]);
        if(p_data[i] == NULL || len_data[i] == 0){
            printf("protocol_up2p_data_alloc failt p_data[i] %x  len %x!!\n",p_data[i],len_data[i]);
            return -1;
        }

        if(memcmp( p_data[i],str[i],strlen(str[i]))){
             printf("string was not match %s  !!== %s \n", p_data[i],str[i]);
             return -1;
        }
        
        printf("cmd %x  <------>    succfully !! \n",cmd_recv[i]);
#endif
        resoult[i] = 1;
        
        ufree(p_data[i]);
        ufree(p_enc_data[i]);
        ufree(p_send[i]);
        p_send[i] = NULL;
    }
    

    for(i = 0;i< _TST_COUNT;i++){
        if(resoult[i] == 0){
            printf("protocol unittest failt !!\n");
            return -1;
        }
    }
    
    up2p_log_close();
    printf("protocol unittest successfully !!\n");
    return 0;
}

#endif


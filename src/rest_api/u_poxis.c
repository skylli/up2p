/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    u_socket.c

   1.mamage link cennction.

    TIME LIST:
    CREATE  skyli   2017-10-23 17:30

*******************************************************************************/
#include <arpa/inet.h>
#include "up2p.h"
#include "util.h"
#include "config.h"

int socket_listenbin_creat(int port){
    struct sockaddr_in sin;
    int fd = 0;
    // set log level 
    log_level(U_LOG_INFO,"Creat UP2PS relay server %s %s", __DATE__, __TIME__);
    
    // 创建socket
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    fd = socket(AF_INET,SOCK_DGRAM, 0);

    if(fd <= 0){
        log_level(U_LOG_ERROR,"open socket FAILT !!");
        return U_ERROR_NULL;
    }
    
    bind(fd, (SOCKADDR*)&sin, sizeof(SOCKADDR));
    log_level(U_LOG_INFO,"port %d listen socket creat successfully fd = %d",port,fd);
    
    return fd;
}

// UDP发送数据包
int socket_udp_send(int fd,const char *data, int len, UDP_INFO *info)
{
    int addrlen = sizeof(SOCKADDR);
    int ret;
    struct sockaddr_in dstaddr;

    dstaddr.sin_family = AF_INET;
    dstaddr.sin_addr.s_addr = info->dip;
    dstaddr.sin_port = info->dport;

    log_level(U_LOG_DEBUG,"data = %x payload len = %d",data ,len);
    log_level(U_LOG_DEBUG,"fd = %d remote ip = %x port %x",fd,info->dip,info->dport);
    ret = sendto(fd, data, len, 0, (SOCKADDR*)&dstaddr, addrlen);
    if(ret  < 0){
        perror("socket sendto error");
        w_debug_level(U_LOG_ERROR, "sendto failt, socket error!! ret = %d",ret);
    }

    return ret;
}

int socket_udp_recv(int fd,char *recv_data,int buffer_size, int *p_len_send, UDP_INFO *info)
{
    int size;
    struct sockaddr_in sin_recv;
    int addrlen = sizeof(SOCKADDR);

    while(1)
    {
        size = recvfrom(fd, recv_data, buffer_size, 0, (SOCKADDR*)&sin_recv, &addrlen);
        // todo 模拟随机"丢包"        

        if(DEBUG_LEVEL >= U_LOG_DEBUG  && size > 0 ){
            w_dump("udp receive : ",recv_data,size);
        }
            
        info->sip = sin_recv.sin_addr.s_addr;
        info->sport = sin_recv.sin_port;
        info->dip = 0;   // 实际上用不到这两个值
        info->dport = 0;
        *p_len_send = size;
        
        return size ;
    }
    return 0;
}
u32 socket_gethost_ip(const u8 *sname){

    u32 host_ip;
    struct hostent *host = gethostbyname(sname);

    if(host)
    {
        struct in_addr *addr = (struct in_addr *)host->h_addr_list[0];
        host_ip = addr->s_addr;
    }
    else
    {
        host_ip = inet_addr(sname);
    }

    if(host_ip == 0xFFFFFFFF)
    {
        log_level(U_LOG_ERROR,"link server error");
        host_ip = 0;
    }
    return host_ip;

}

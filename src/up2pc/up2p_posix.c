/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    up2pc_vc.c

    No description

    TIME LIST:
    CREATE  skyli   2017-6-26 13:48:12

*******************************************************************************/

#include "up2pc.h"

static SOCKET socket_main;


int udp_soket_init(u16 lport)
{
    
    struct sockaddr_in sin;
    // 创建socket
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(lport);
    socket_main = socket(AF_INET,SOCK_DGRAM, 0);

    if(socket_main <= 0){
        return -1;
        }
    
    bind(socket_main, (SOCKADDR*)&sin, sizeof(SOCKADDR));
    
    return socket_main;
}

int up2pc_udp_recv( char *recv_data,int buffer_size, int *plen, UDP_INFO *info)
{
    int size;
    struct sockaddr_in sin_recv;
    int addrlen = sizeof(SOCKADDR);

    while(1)
    {

        size = recvfrom(socket_main, recv_data, buffer_size, 0, (SOCKADDR*)&sin_recv, &addrlen);
        // todo 模拟随机"丢包"        

        if(DEBUG_LEVEL >= U_LOG_DEBUG  && size > 0 ){
            w_dump("udp receive : ",recv_data,size);
        }
            
        info->sip = sin_recv.sin_addr.s_addr;
        info->sport = sin_recv.sin_port;
        info->dip = 0;   // 实际上用不到这两个值
        info->dport = 0;
        *plen = size;
        #if 1
        if(size>0)
            w_dump("udp receive : ",recv_data,size);
        #endif
        return size ;
    }
    return 0;
}

int serial_read(char *data, int len)
{
    memcpy(data,"uart wirte test",strlen("uart wirte test"));
    len =  strlen("uart wirte test") + 1;
    return len;
}

int serial_write(const char *data, int len)
{
      printf("uart len : %d\n", len);
      int i=0;
      for(i;i<len;i++){
        printf("[%d]",(u8)data[i]);

      }
      printf("\tend\n");
    if( len < 4 || data[0] != 0x55){
      printf("receive unsupport data");
      return 0;
    }
    switch(data[1]){
      case 0x10:
        printf("receive on \n");
        break;
      case 0x11:
        printf("receive off\n");
        break;
      case 0x20:
        printf("receive rgb \n");
        if(data[3] >= 3){
          printf("uart write :  r %d g %d b %d\n", (u8)data[4],(u8)data[5],(u8)data[6]);
        }
      break;
    }
   
    return len;
}

// UDP发送数据包
int up2pc_udp_send(const char *data, int len, UDP_INFO *info)
{
    int addrlen = sizeof(SOCKADDR);
    int ret;
    struct sockaddr_in dstaddr;

    dstaddr.sin_family = AF_INET;
    dstaddr.sin_addr.s_addr = info->dip;
    dstaddr.sin_port = info->dport;

    ret = sendto(socket_main, data, len, 0, (SOCKADDR*)&dstaddr, addrlen);

    return ret;
}

// 获取系统时间
u32 sys_get_time()
{
    return time(NULL);
}

// 保存密钥  
// 保存到 flash
int sys_set_key(u32 key0, u32 key1)
{
    printf("update key %x %x\n", (int)key0, (int)key1);
    return 0;
}

// 生成随机数
u32 sys_get_random()
{
    return rand() | (rand() << 16);
}
// 获取 host 的ip。
u32 dns_ip(const char *sname)
{
    struct hostent *host;
    u32 host_ip = 0;
    
    host = gethostbyname(sname);
    
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
        printf("link server error\n");
        host_ip = 0;
    }

    return host_ip;
}
// 配置服务器信息
int sys_config_host(char *host, u16 port)
{
    u32 server_ip = dns_ip(host);
    u16 server_port = port;

    // 解析成功则,设置服务器信息
    if(server_ip != 0)
    {
        up2pc_set_server(server_ip, server_port);
        // 更新服务器信息
    }

    return 0;
}
/********************* cmd support *******************************/
int io_write(u8 pin,u8 val){
	printf(">>>>>>>>>>>>>> pin setting pin = %d,value = %d \n",pin,val);
	return 0;
}
int io_mode_int(u8 pin,u8 mode){
	printf(">>>>>>>>>>>>>> pin setting pin = %d,mode = %d \n",pin,mode);
	return 0;

}




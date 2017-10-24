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


#if 1
//recv_proc(const char *recv_data, int len, UDP_INFO *info)
static void recv_proc_thread()
{

    while(1){
            int len;
            UDP_INFO info;
            char recvbuf[MAX_NET_PACKAGE_SIZE];
            
            memset(&info,0,sizeof(UDP_INFO));
            memset(recvbuf,0,MAX_NET_PACKAGE_SIZE);
            
            if(up2pc_udp_recv(recvbuf,MAX_NET_PACKAGE_SIZE,&len, &info) > 0)
                up2pc_recv(recvbuf, len, &info);
   }        
}

#endif

int main(int argc, const char *argv[])
{
    u16 local_port, server_port;
    u32 local_ip, server_ip;
    u32 dev0, dev1, key0, key1;

    u32 current_tm = 0,last_update_tm = 0,last_retransmit_tm = 0;
      
    pthread_t thread;

    if(argc != 6)
    {
        printf("argc %d local_port server_ip server_port dev0 dev1 key0 key1\n",argc);
        return -1;
    }
    w_debug_level(U_LOG_DEBUG,"argv %s %s %s %s %s",argv[1],argv[2],argv[3],argv[4],argv[5]);
    
    local_port = atoi(argv[1]);
    //server_ip = inet_addr(argv[2]);
    server_ip = dns_ip(argv[2]);
    server_port = htons(atoi(argv[3]));
    
    ascii_to_2u32(argv[4],&dev0,&dev1);
    ascii_to_2u32(argv[5],&key0,&key1);

    if(udp_soket_init(local_port) < 0){
        w_debug_level(U_LOG_ERROR,"creat socket FAILT !!!");
        return -1;
        }

    // 设置各种参数
    up2pc_setkey(key0, key1);
    up2pc_set_server(server_ip, server_port);
    up2pc_set_local(0, htons(local_port));
    up2pc_set_name(dev0, dev1);
    
    pthread_create(&thread, NULL, (void *)recv_proc_thread, NULL);
    // 更新服务器信息
    up2pc_update_host();

    while(1)
    {
        current_tm = time(NULL);
        if((current_tm - last_update_tm) > 10){
          up2pc_update_dev_status("test data\n",strlen("test data\n"));
          last_update_tm  = time(NULL);
          last_retransmit_tm  = last_update_tm;
        }
        
        if((current_tm - last_retransmit_tm) > 2){
          up2pc_retransmit();
          last_retransmit_tm  =  time(NULL);
        }
        // 定时发送在线信息
        up2pc_set_online();
        sleep(5);
    }
    return 0;
}



/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    u_socket.h

   1. mamage socket.

    TIME LIST:
    CREATE  skyli   2017-10-23 17:30

*******************************************************************************/
#ifndef _vlink_h_
#define _vlink_h_


#include "up2p.h"

int socket_listenbin_creat(int port);
int socket_udp_send(const char *data, int len, UDP_INFO *info);
int socket_udp_recv(int fd,char *recv_data,int buffer_size, int *p_len_send, UDP_INFO *info);

u32 socket_gethost_ip(const u8 *sname);


#endif //  end _vlink_h_

/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    ctrl.c

    No description

    TIME LIST:
    CREATE  skyli   2014-08-15 10:30:58

*******************************************************************************/

#include "up2p.h"
#include "up2ps_ctrl.h"

SOCKET _creat_tcp(const char *ip, int port)
{
    SOCKET socket_main;
    struct sockaddr_in addr_main;
    int ret;
    int len;
    
    socket_main = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    addr_main.sin_family = AF_INET;
    addr_main.sin_port = htons(port); 
    addr_main.sin_addr.s_addr = inet_addr(ip);

    ret = connect(socket_main, (struct sockaddr *)&addr_main, sizeof(addr_main));
    if(ret != 0)
    {
        printf("connect err\n");
        goto err;
    }

    return socket_main;
err:
    closesocket(socket_main);
    return 0;
}

static int get_info()
{
    int ret;
    CTRL_LOGIN login;
    CLIENT_INFO_HEAD head;
    CLIENT_INFO *info;
    int i;
    SOCKET sk = _creat_tcp("127.0.0.1", SERVER_PORT);

    if(sk == 0)
        goto _err;

    // 发送请求
    login.magic = CTRL_MAGIC;
    strcpy(login.username, "test");
    strcpy(login.password, "test");
    login.cmd = CTRL_CMD_LIST;
    login.len = 0;

    ret = send(sk, (char *)&login, sizeof(CTRL_LOGIN), 0);
    if(ret != sizeof(CTRL_LOGIN))
    {
        goto _err;
    }
    
    // 接收用户总数
    ret = recv(sk, &head, sizeof(CLIENT_INFO_HEAD), 0);
    if(ret != sizeof(CLIENT_INFO_HEAD))
    {
        goto _err;
    }
    printf("head.count %d\n", head.count);

    // 分配内存,接收所有用户
    info = malloc(head.count * sizeof(CLIENT_INFO));
    ret = recv(sk, info, head.count * sizeof(CLIENT_INFO), 0);
    if(ret != head.count * sizeof(CLIENT_INFO))
    {
        goto _err2;
    }

    // 打印所有用户
    for(i = 0; i < head.count; i++)
    {
        printf("%x %x\n", info[i].dev0, info[i].dev1);
    }

    free(info);
    closesocket(sk);
    return 0;
_err2:
    free(info);
    closesocket(sk);
_err:
    return -1;
}

int main(int argc, const char *argv[])
{
    int ret;
    int i;

#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    // 循环取回数据并打印出来
    ret = get_info();

_err:
    return 0;
}

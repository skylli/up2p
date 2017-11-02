/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    up2pa.c

    No description

    TIME LIST:
    CREATE  skyli   2017-05-06 13:46:51

*******************************************************************************/
#include "up2pa.h"
#include "up2p_crypt.h"
#include <errno.h>

#ifdef __ANDROID__
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <android/log.h>
#endif

#define MAX_USER 256

static SOCKET socket_main;
static u16 local_port;
static u16 server_port;
static u32 server_ip;
static u16 client_port;

static char recvbuf[1024];
static char sendbuf[1024];
static u32 local_mac0;
static u32 local_mac1;

#ifdef _WIN32

void delay_ms(u32 ms)
{
    Sleep(ms);
}
#else
void delay_ms(u32 ms)
{
    usleep(ms * 1000);
}
#endif

int up2pa_debug_level = 0;

#if 0
static char *time_str()
{
    time_t rawtime;
    struct tm* timeinfo;
    static char time_s[256];
    time(&rawtime);
    timeinfo=localtime(&rawtime);
    strftime(time_s, sizeof(time_s), "%Y-%m-%d %I:%M:%S ",timeinfo);
    return time_s;
}

int debug_log(const char *fmt, ...)
{
    if(up2pa_debug_level > 0)
    {
        char buffer[0x1000];
        char *time_s = time_str();
        va_list argptr;
        va_start(argptr, fmt);
        vsprintf(buffer,fmt,argptr);
        va_end(argptr);

#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_INFO, "ulink", "%s", buffer);
#else
        printf("%s", time_s);
        printf("%s", buffer);
#endif
    }
    
    return 0;
}

#endif
static volatile int flag_run;
static volatile int flag_break;
static volatile u16 packet_idx;
static volatile u16 packet_idx_observer;
static volatile u32 g_session_token; // todo

UP2P_PACKET *packetrecv;
static FILE *fcopy = NULL;

// 在线类型
typedef enum{
    ONLINE_NO,
    ONLINE_WAN,
    ONLINE_LAN,
    ONLINE_MAX
} ONLINE_TYPE;

typedef struct{
    u32 dev0;
    u32 dev1;
    u32 ip;
    u16 port;
    u16 idx;
    int online;
} ONLINE;

static ONLINE *onlinelist;

// --------------------------------------------------------------------
/*
* description: malloc 接收队列
* ouput: packetrecv 所 malloc 的缓存指针.
* return: -1 malloc 失败.
* todo: 
*       一次性 malloc。
*       payload 保存在哪里.
**/ 
static int packetrecv_init()
{
    packetrecv = (UP2P_PACKET *)malloc(sizeof(UP2P_PACKET) * MAX_USER);
    if(!packetrecv){
        log_level(U_LOG_ERROR,"alloc failt !!");
        return U_ERROR_ALLOC;
    }
    
    memset(packetrecv, 0, sizeof(UP2P_PACKET) * MAX_USER);

    return 0;
}
/*
* description: 寻找属于 dev 的数据包。
* argv: dev 查询的设备 id.
*/
static UP2P_PACKET *packetrecv_find(u32 dev0, u32 dev1)
{
    int i;
    u32 now_time = (u32)time(NULL);

    // 寻找空闲块
    for(i = 0; i < MAX_USER; i++)
    {
        // 相同的dev
        if(packetrecv[i].src0 == dev0 && packetrecv[i].src1 == dev1 
            && packetrecv[i].magic == MAGIC)
        {
            return &packetrecv[i];
        }
    }

    return NULL;
}
/*
* description: 数据包保存在接收队列.
* argv: 
*       packet: 数据包的指针。
*       len: 数据长度.
*/
static int packetrecv_add(UP2P_PACKET *packet, int len)
{
    int i;
    u32 now_time = (u32)time(NULL);

    // 寻找空闲块
    for(i = 0; i < MAX_USER; i++)
    {
        // 超时的或者空闲的,都可以使用
        if(packetrecv[i].magic == 0)
        {
            memcpy(&packetrecv[i], packet, len);
            log_debug("packetrecv_add %x %x\n", packet->src0, packet->src1);
            break;
        }
    }

    return 0;
}
/*
* description: 移除接收缓存的数据.
* argv: packet 要移除的 packet 指针.
* todo: 没有处理 payload.
*/
static int packetrecv_remove(UP2P_PACKET *packet)
{
    memset(packet, 0, sizeof(UP2P_PACKET));
    return 0;
}
/*
* 是否数据接收队列.
*/
static int packetrecv_deinit()
{
    free(packetrecv);

    return 0;
}

/*
* description: malloc 本地设备状态缓存.
* return: U_ERROR_ALLOC malloc failt
*/
static int online_init()
{
    onlinelist = (ONLINE *)malloc(sizeof(ONLINE) * MAX_USER);
    if(!onlinelist){
        log_level(U_LOG_ERROR,"alloc failt !!");
        return U_ERROR_ALLOC;
    }    
    memset(onlinelist, 0, sizeof(ONLINE) * MAX_USER);

    return 0;
}
/*
* description: 更新设备状态.
* argv:
*       dev: 设备id
*       idx： 最新的indx,依此判断数据该 online 包的新旧.
*/
static int online_update(u32 dev0, u32 dev1, u16 idx, int online)
{
    int i;

    // 寻找空闲块
    for(i = 0; i < MAX_USER; i++)
    {
        // 超时的或者空闲的,都可以使用
        if((onlinelist[i].dev0 == dev0 && onlinelist[i].dev1 == dev1)
            || (onlinelist[i].dev0 == 0 && onlinelist[i].dev1 == 0))
        {
            onlinelist[i].dev0 = dev0;
            onlinelist[i].dev1 = dev1;

            // 一旦是LAN模式,即使收到了WAN模式,也一直认为是LAN模式
            if(onlinelist[i].online != ONLINE_LAN)
            {
                onlinelist[i].online = online;
                onlinelist[i].idx = idx;
            }

            // 索引一致, 说明是短时间内重复回应的数据包
            if(onlinelist[i].idx == idx)
            {
                // 短时间内重复的数据包, 如果是LAN模式, 则替换之
                if(online == ONLINE_LAN)
                {
                    onlinelist[i].online = online;
                    onlinelist[i].idx = idx;
                    log_debug("online_update %x %x type %d i = %d", dev0, dev1, online, i);
                }
            }
            else
            {
                onlinelist[i].online = online;
                onlinelist[i].idx = idx;
                log_debug("online_update %x %x type %d i = %d", dev0, dev1, online, i);
            }

            break;
        }
    }
    return 0;
}
/*
* description: 获取设备状态地址
* return: 设备状态地址.
*/
static ONLINE *online_find(u32 dev0, u32 dev1)
{
    int i;

    if(dev0 == 0 && dev1 == 0)
        return NULL;
    if(!onlinelist)
        return NULL;
    for(i = 0; i < MAX_USER; i++)
    {
        if(onlinelist[i].dev0 == dev0 && onlinelist[i].dev1 == dev1)
        {
            log_debug("online_find %x %x type %d i = %d", dev0, dev1, onlinelist[i].online, i);
            return &onlinelist[i];
        }
    }
    return NULL;
}
/*
* description: 查询设备的状态.
*/
static int online_check(u32 dev0, u32 dev1)
{
    ONLINE *online = online_find(dev0, dev1);

    if(online)
    {
        return online->online;
    }

    return -1;
}
/*
* 释放本地在线状态列表.
*/
static int online_deinit()
{
    free(onlinelist);
    return 0;
}
/*
* description: soket 发送.
* argv:
*       data : 发送的数据包.
*       len : data 的长度.
*       ip port: 目标的 ip 地址和 port 
* todo: 该接口应该放置到平台文件中.
*/
static int _send(const char *data, int len, u32 ip, u16 port)
{
    int addrlen = sizeof(SOCKADDR);
    int ret;
    struct sockaddr_in dstaddr;

    if(ip == 0)
        return -1;

    dstaddr.sin_family = AF_INET;
    dstaddr.sin_addr.s_addr = ip;
    dstaddr.sin_port = port;

    ret = sendto(socket_main, data, len, 0, (SOCKADDR*)&dstaddr, addrlen);

    return ret;
}
/*
* description:发送数据包.
* argv:
*       packet:要发送的数据包.
* return: 已发送的数据量.
*/
static int send_packet(UP2P_PACKET *packet)
{
    int len;
    ONLINE *online;

    online = online_find(packet->dst0, packet->dst1);
    len = sizeof(UP2P_PACKET) + packet->len;

    log_debug("send dev %lx %lx idx %lx cmd %lx", packet->dst0, packet->dst1, packet->idx, packet->cmd);

    // 检测目标设备是处于广域网还是局域网
    if(online)
    {
        if(online->online == ONLINE_LAN)
        {
            // 是局域网,直接使用局域网模式发送
            return _send((const char *)packet, len, online->ip, online->port);
        }
    }
    return _send((const char *)packet, len, server_ip, server_port);
}
/*
* description:发送数据包.
*/
static int send_packet_to(UP2P_PACKET *packet, u32 ip, u16 port)
{
    int len;

    len = sizeof(UP2P_PACKET) + packet->len;

    return _send((const char *)packet, len, ip, port);
}

/*
* description：解析接收到的命令
*   argv: 
*       packet:接收到数据包指针，
*       ip port: 数据包的源 ip 源 port.
*/
static int recv_cmd(UP2P_PACKET *packet, u32 ip, u16 port)
{
    switch(packet->cmd)
    {
    case CMD_CHECK_ONLINE_YES:
        online_update(packet->dst0, packet->dst1, packet->idx, ONLINE_WAN);
        break;
    case CMD_CHECK_ONLINE_NO:
        online_update(packet->dst0, packet->dst1, packet->idx, ONLINE_NO);
        break;
    case CMD_LAN_SCAN_ACK:
        {
            ONLINE *online;
            online_update(packet->src0, packet->src1, packet->idx, ONLINE_LAN);
            online = online_find(packet->src0, packet->src1);
            online->ip = ip;
            online->port = port;

            break;
        }
    default:
        {
            packetrecv_add(packet, sizeof(UP2P_PACKET) + packet->len);
            log_debug("recv cmd = %x", packet->cmd);
        }
        break;
    }
    return 0;
}

/*
* desciption: socket 接收.
*   argv : param 未使用.
* todo: 应该移到平台文件夹中。
*/
static int recv_proc(void *param)
{
    int size;
    struct sockaddr_in sin_recv;
    int addrlen = sizeof(SOCKADDR);
    UP2P_PACKET *packet;

    while(1)
    {
        size = recvfrom(socket_main, recvbuf, sizeof(recvbuf), 0, (SOCKADDR*)&sin_recv, &addrlen);

        packet = (UP2P_PACKET *)recvbuf;

        log_debug("recv from:%s len:%d magic:%x cmd:%x", inet_ntoa(sin_recv.sin_addr), size, packet->magic, packet->cmd);
        
        if(packet->magic != MAGIC)
            continue;
        
        recv_cmd(packet, sin_recv.sin_addr.s_addr, sin_recv.sin_port);
    }

    return 0;
}
/*
 * description: 初始化各大全局变量以及 socket.
 * argv:
 *      sport 本地绑定端口 
 *      sname 服务器域名或者IP 
 *      dport 服务器端口 
 *      cport 客户端即设备的端口
 *      mac0 mac1: 本机 id，一般为mac.
 * todo: socket 部分应该移到平台.
 */
int up2pa_init(u16 sport, const char *sname, u16 dport, u16 cport, u32 mac0, u32 mac1)
{
    struct hostent *host;
    struct sockaddr_in sin;
    int op0,res =0;

#ifdef _WIN32
    DWORD dwBytesReturned = 0;
    BOOL bNewBehavior = FALSE;
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2),&wsaData);//1.1版本
#else
    pthread_t thread;
#endif

    flag_run = 0;
    flag_break = 0;

    local_port = sport;
    server_port = dport;
    client_port = cport;
    local_mac0 = mac0;
    local_mac1 = mac1;
    packet_idx = 0;
    
    packet_idx_observer = 0;

    host = gethostbyname(sname);
    if(host)
    {
        struct in_addr *addr = (struct in_addr *)host->h_addr_list[0];
        server_ip = addr->s_addr;
    }
    else
    {
        server_ip = inet_addr(sname);
    }

    if(server_ip == 0xFFFFFFFF)
    {
        log_level(U_LOG_ERROR,"link server error");
        server_ip = 0;
    }

   if( (res = packetrecv_init()) < 0 )
        return res;
   
    if((res = online_init())< 0 )
        return res;

    // 创建socket
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(local_port);
    socket_main = socket(AF_INET, SOCK_DGRAM, 0);

    if(socket_main == -1){

        log_level(U_LOG_ERROR,">>> error %s",strerror(errno));
        return -1;
    }

    // 使能广播
    op0 = 1;
    setsockopt(socket_main, SOL_SOCKET, SO_BROADCAST, (char *)&op0, sizeof(op0));

#ifdef _WIN32
    WSAIoctl(socket_main, SIO_UDP_CONNRESET,
        &bNewBehavior, sizeof(bNewBehavior),
        NULL, 0, &dwBytesReturned,
        NULL, NULL);
#endif
    bind(socket_main, (SOCKADDR*)&sin, sizeof(SOCKADDR));
    
#ifdef WIN32
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recv_proc, NULL, 0, 0);
#else
    pthread_create(&thread, NULL, (void *)recv_proc, NULL);
#endif

    return 0;
}

/*
 * 卸载
 */
int up2pa_deinit()
{
    packetrecv_deinit();
    online_deinit();

    closesocket(socket_main);

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

/*
* description：获取广播的 ip 列表.
* argv:
*       list : 当前广播列表的表头.
*       maxlen： 
*/
#ifdef _WIN32
static int get_bcast_list(u32 *list, int maxlen)
{
    INTERFACE_INFO bInterfaceInfo[10];
    DWORD dlByteReturn = 0;
    int iNumOfInterface;
    int i;

    WSAIoctl(socket_main, SIO_GET_INTERFACE_LIST, NULL, 0, bInterfaceInfo, 1024, &dlByteReturn, NULL, NULL );
    iNumOfInterface = dlByteReturn / sizeof(INTERFACE_INFO);
    for(i = 0; i < iNumOfInterface; i++)
    {
        struct sockaddr_in pIPAddress;
        struct sockaddr_in pMaskAddress;

        if(i >= maxlen)
            break;

        pIPAddress = bInterfaceInfo[i].iiAddress.AddressIn;
        pMaskAddress = bInterfaceInfo[i].iiNetmask.AddressIn;
        list[i] = pIPAddress.sin_addr.s_addr | (~pMaskAddress.sin_addr.s_addr);
    }

    return i;
}
#elif defined(__ANDROID__)
static int get_bcast_list(u32 *list, int maxlen)
{
    int i, num;
    struct ifconf ifc;
    struct ifreq buf[10];
    
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    
    if(ioctl(socket_main, SIOCGIFCONF, (char *)&ifc) != 0)
    {
        log_level(U_LOG_ERROR,"__ANDROID__,get_bcast_list err");
        // log_level(U_LOG_ERROR,"socket_main:  %p",socket_main);
        log_level(U_LOG_ERROR,">>> SIOCGIFCONF error  %s",strerror(errno));
        return 0;
    }

    num = ifc.ifc_len / sizeof(struct ifreq);
    if(num > 10)
        num = 10;
    
    log_debug("num = %d", num);
    
    for(i = 0; i < num; i++)
    {
        u32 ip = 0, mask = 0;
        
        if(!(ioctl(socket_main, SIOCGIFADDR, (char *)&buf[i])))
        {
            ip = ((struct sockaddr_in*) (&buf[i].ifr_addr))->sin_addr.s_addr;
            
            log_debug("ip %x", ip);
        }
        
        if(!(ioctl(socket_main, SIOCGIFNETMASK, (char *)&buf[i])))
        {
            mask = ((struct sockaddr_in*) (&buf[i].ifr_netmask))->sin_addr.s_addr;
            
            log_debug("mask %x", mask);
        }
        
        list[i] = ip | (~mask);
    }
    
    return i;
}
#else
static int get_bcast_list(u32 *list, int maxlen)
{
    int num;
    struct ifaddrs *ifa, *oifa;
    
    if(getifaddrs(&ifa) < 0)
    {
        log_level(U_LOG_ERROR,"get_bcast_list err");
        return 0;
    }
    oifa = ifa;
    num = 0;
    while(ifa)
    {
        struct sockaddr_in* saddr = (struct sockaddr_in*)ifa->ifa_addr;
        struct sockaddr_in* smask = (struct sockaddr_in*)ifa->ifa_netmask;
        
        if(saddr && smask)
        {
            u32 ip = saddr->sin_addr.s_addr;
            u32 mask = smask->sin_addr.s_addr;
            
            if(ip != 0 && ip != 0x100007f)
            {
                list[num] = ip | (~mask);
                //debug_log("bcast %x\n", list[num]);
                num++;
            }
        }
        
        ifa = ifa->ifa_next;
    }
    
    
    return num;
}
#endif

/*
 * 清理在线状态
 */
int up2pa_clear_online(u32 dev0, u32 dev1)
{
    ONLINE *online = online_find(dev0, dev1);

    if(online)
    {
        online->dev0 = 0;
        online->dev1 = 0;
        online->online = 0;
    }

    return 0;
}

/*
 *decriptions: 发送 online 到服务器，发送 CMD_CHECK_ONLINE 到服务器,同时在局域网内 广播 CMD_LAN_SCAN。
 *  argv:
 *          dev0 dev1: 设备id
 *notice: 发送检查在线状态的数据包, 查询在线状态之前先调用
 */
int up2pa_send_online(u32 dev0, u32 dev1)
{
    UP2P_PACKET *packet = (UP2P_PACKET *)sendbuf;
    u32 iplist[4];
    int num;
    int i;

    // 发送给服务器
    packet->magic = MAGIC;
    packet->src0 = local_mac0;
    packet->src1 = local_mac1;
    packet->dst0 = dev0;
    packet->dst1 = dev1;
    packet->cmd = CMD_CHECK_ONLINE;
    packet->len = 0;

    // online包的idx使用了时间低16位
    packet->idx = (u16)time(NULL);
    
    //log_debug("send online to %x %x", server_ip, server_port);
    send_packet_to(packet, server_ip, server_port);

#if 1
    // todo disable lan scan.
    packet->cmd = CMD_LAN_SCAN;

    // 获取子网掩码列表,每个子网发一次广播
    num = get_bcast_list(iplist, 4);
    for(i = 0; i < num; i++)
    {
        // 关于端口号: 此处的端口是客户端的端口
        send_packet_to(packet, iplist[i], client_port);
    }
    #endif
    return 0;
}

/*
 * descrption: 检查在线状态
 * argv: dev 查询的设备 id
 * return: 0 不在线 1 广域网在线 2 局域网在线 -1 服务器无响应
 */
int up2pa_check_online(u32 dev0, u32 dev1)
{
    return online_check(dev0, dev1);
}

/*
 * description: 发送远程指令
 * argv: 
 *      cmd 指令编码 param 指令附加数据 len 附加数据长度
 * return: 没有附加数据 param 可以为NULL, len为0
 */
int up2pa_send_cmd(u32 dev0, u32 dev1, u32 cmd, const char *param, int len)
{
    UP2P_PACKET *packet = (UP2P_PACKET *)sendbuf;

    if(flag_break)
        return -2;

    packet->magic = MAGIC;
    packet->src0 = local_mac0;
    packet->src1 = local_mac1;
    packet->dst0 = dev0;
    packet->dst1 = dev1;
    packet->cmd = cmd;
    packet->len = len;

    packet_idx++;
    packet->idx = packet_idx;
    if(len > 0)
        memcpy(packet->payload, param, len);

    return send_packet(packet);
}

/*
 * description: 发送带索引的远程指令,往往是非加密数据.
 * argv: 
 *      idx 索引号 cmd 指令编码 param 指令附加数据 len 附加数据长度
 * noti: 没有附加数据 param 可以为NULL, len为0.
 * return: 已发的数据
 */
int up2pa_send_idx_cmd(u32 dev0, u32 dev1, u16 idx, u32 cmd, const char *param, int len)
{
    UP2P_PACKET *packet = (UP2P_PACKET *)sendbuf;

    if(flag_break)
        return -2;

    packet->magic = MAGIC;
    packet->src0 = local_mac0;
    packet->src1 = local_mac1;
    packet->dst0 = dev0;
    packet->dst1 = dev1;
    packet->cmd = cmd;
    packet->len = len;
    packet->idx = idx;
    if(len > 0)
        memcpy(packet->payload, param, len);

    return send_packet(packet);
}

/*
 * description: 发送加密远程指令
 * argv:
 *      cmd 指令编码 param 指令附加数据 len 附加数据长度
 * notice: 没有附加数据 param 可以为NULL, len为0
 * return: 已发的数据量.
 */
int up2pa_send_data_cmd(u32 dev0, u32 dev1, u32 key0, u32 key1, u32 token, u32 cmd, const char *param, int len)
{
    int ret;
    char inbuf[MAX_NET_PACKAGE_SIZE];
    char outbuf[MAX_NET_PACKAGE_SIZE];
    UP2P_DATA *data = (UP2P_DATA *)inbuf;

    data->cmd = cmd;
    data->token = token;
    data->len = len;
    
    if(param)
    {
        memcpy(data->payload, param, len);
        log_debug("raw data param [%x][%x]", param[0],param[1]);
        log_debug("raw data data payload [%x][%x]", data->payload[0], data->payload[1]);
    }
    
    unsigned char *p = (unsigned char *)data;
    //w_dump("befor aes raw data ",p,(sizeof(UP2P_DATA) + data->len));
    len = data_enc((char *)data, outbuf, sizeof(UP2P_DATA) + data->len, key0, key1);

    //w_dump("after aes data ",outbuf,len);
    ret = up2pa_send_cmd(dev0, dev1, CMD_DATA, outbuf, len);

    return ret;
}

/*
 * description: 读取返回指令
 * argv:
 *  data 接收缓存 maxlen 缓存区最大长度
 * return: 附加数据的长度 长度也可能为0 -1 无数据
 */
int up2pa_recv_cmd(CMD_TYPE cmd_type,u32 dev0, u32 dev1, u32 *cmd, char *data, int maxlen)
{
    int len;
    int rm_package = 1;

    UP2P_PACKET *packet = packetrecv_find(dev0, dev1);

    if(flag_break)
        return -2;

    if(packet == NULL)
        return -1;
    // 丢弃早包.
    rm_package = (cmd_type)?(packet->idx < packet_idx_observer):(packet->idx != packet_idx);
    if(packet->len > maxlen || rm_package )
    {
        packetrecv_remove(packet);
        return -1;
    }
    if(cmd_type)
        packet_idx_observer = packet->idx;
    

    *cmd = packet->cmd;
    len = packet->len;

    if(len > 0)
    {
        memcpy(data, packet->payload, packet->len);
    }

    packetrecv_remove(packet);

    return len;
}

/*
 * description: 读取带索引的返回指令
 * argv:
 *      idx 索引号 data 接收缓存 maxlen 缓存区最大长度
 * return: 附加数据的长度 长度也可能为0 -1 无数据
 * todo:  不是该 idx 直接删除. 是否存在误删.
 */
int up2pa_recv_idx_cmd(u32 dev0, u32 dev1, u16 idx, u32 *cmd, char *data, int maxlen)
{
    int len;

    UP2P_PACKET *packet = packetrecv_find(dev0, dev1);

    if(flag_break)
        return -2;

    if(packet == NULL)
        return -1;
    // todo 不是该 idx 直接删除. 是否存在误删.
    if(packet->len > maxlen || packet->idx != idx)
    {
        packetrecv_remove(packet);
        return -1;
    }

    *cmd = packet->cmd;
    len = packet->len;

    if(len > 0)
    {
        memcpy(data, packet->payload, packet->len);
    }

    packetrecv_remove(packet);

    return len;
}
/*
 * descriptin: 解码接收到的数据
 * argv：
 *      cmd_type 数据包的类型
 *      dev0 dev1 接收的设备
 *      key0 key1 aes秘钥.
 *      data 接收缓存 maxlen 缓存区最大长度
 * return: 附加数据的长度 长度也可能为0 -1 无数据
 *
 */
int up2pa_recv_data_cmd(CMD_TYPE cmd_type,u32 dev0, u32 dev1, u32 key0, u32 key1, u32 token, u32 *cmd, char *data, int maxlen)
{
    int len;

    char inbuf[512];
    char outbuf[512];
    u32 rcmd;
    UP2P_DATA *udata = (UP2P_DATA *)outbuf;

    // 读取一个数据包
    len = up2pa_recv_cmd(cmd_type,dev0, dev1, &rcmd, inbuf, sizeof(inbuf));
    if(len < 0)
        return len;
    if(rcmd != CMD_DATA_ACK)
        return -1;
    if(len > MAX_NET_PACKAGE_SIZE)
        return -1;

    // 解密数据包
    len = data_dec(inbuf, (char *)udata, len, key0, key1);
    if(udata->token != token)
    {
        log_level(U_LOG_WARN,"diff token recv %lx my %lx", udata->token, token);
        return -1;
    }
    
    if(udata->len > maxlen)
    {
        log_level(U_LOG_WARN,"receive package is too big");
        return -1;
    }

    // 输出数据
    *cmd = udata->cmd;
    if(data)
        memcpy(data, udata->payload, udata->len);
    
    return udata->len;
}
// 同上只是没有去重发数据。
static int common_wait_cmd_noRetry(u32 dev0, u32 dev1, u32 key0, u32 key1, u32 token, u32 cmd, char *data, int maxlen, int crypt)
{
    int len;
    int retry;
    u32 recvcmd;
    int time_start = (int)time(NULL);

    retry = 0;
    while(1)
    {
        if(flag_break)
            return -2;

        if(crypt)
        {
            len = up2pa_recv_data_cmd(CMD_TYPE_OBSERVER,dev0, dev1, key0, key1, token, &recvcmd, data, maxlen);
        }
        else
        {   
            len = up2pa_recv_cmd(CMD_TYPE_OBSERVER,dev0, dev1, &recvcmd, data, maxlen);
        }

        if(len >= 0)
        {
            if(recvcmd == cmd)
                break;
            debug_log("same idx but diff cmd recv %x my %x, don't retry\n", recvcmd, cmd);
            len = -1;
            break;
        }

        if(time(NULL) - time_start > MAX_WAIT_TIME)
            break;
        delay_ms(1);

    }
    return len;
}
/*
* description: 阻塞等待数据包，最多等待 MAX_WAIT_TIME，在等待时会重发请求.
* argv:
*       dev0 dev1: 等待的目标设备。
*       key0 key1: aes 秘钥。
*       token: 会话 token.
*       cmd: 所等待包的cmd.
*       data: 接收数据的buffer.
*       maxlen: buffer 长度.
*       crypt:  0 代表等待的数据包不加密, 否则加密.
* return: 接收数据的长度.
* detail:
*       1.不断的查询接收队列，另外的接收线程把数据包扔到接收队列。
*       2.等待包过程会每隔 500ms 重发上一个数据包。
* todo: 没有发送队列，容易发生拥挤。
*       必须send  /wait 成对调用，才能实现接收.
*/
static int common_wait_cmd(u32 dev0, u32 dev1, u32 key0, u32 key1, u32 token, u32 cmd, char *data, int maxlen, int crypt)
{
    int len;
    int retry;

    u32 recvcmd;
    int time_start = (int)time(NULL);

    retry = 0;
    while(1)
    {
        if(flag_break)
            return -2;
        if(crypt)
        {
            len = up2pa_recv_data_cmd(0,dev0, dev1, key0, key1, token, &recvcmd, data, maxlen);
        }
        else
        {   
            len = up2pa_recv_cmd(CMD_TYPE_NONE,dev0, dev1, &recvcmd, data, maxlen);
        }

        if(len >= 0)
        {
            if(recvcmd == cmd)
                break;
            log_debug("same idx but receive diff cmd %lx my %lx, don't retry\n", recvcmd, cmd);
            len = -1;
            break;
        }

        if(time(NULL) - time_start > MAX_WAIT_TIME){
            log_level(U_LOG_WARN,"receive %lx timeout",cmd);
            break;
         }
        // 大于500ms之后,每隔500ms重发最后一个数据包
        if(retry >= 50 && (retry % 50) == 0)
        {
            UP2P_PACKET *packet = (UP2P_PACKET *)sendbuf;
            send_packet(packet);
        }

        delay_ms(10);
        retry++;
    }
    return len;
}

/*
 * 阻塞并等待指定的指令
 * cmd 等待的指令 data 接收缓存 maxlen 缓存区最大长度
 * 返回值 附加数据的长度 长度也可能为0 -1 无数据
 */
int up2pa_wait_cmd(u32 dev0, u32 dev1, u32 cmd, char *data, int maxlen)
{
    return common_wait_cmd(dev0, dev1, 0, 0, 0, cmd, data, maxlen, 0);
}


/*
 * 阻塞并等待指定的加密指令
 * cmd 等待的指令 data 接收缓存 maxlen 缓存区最大长度
 * 返回值 附加数据的长度 长度也可能为0 -1 无数据
 */
int up2pa_wait_data_cmd(u32 dev0, u32 dev1, u32 key0, u32 key1, u32 token, u32 cmd, char *data, int maxlen)
{
    return common_wait_cmd(dev0, dev1, key0, key1, token, cmd, data, maxlen, 1);
}
int up2pa_wait_data_cmd_noRetry(u32 dev0, u32 dev1, u32 key0, u32 key1, u32 token, u32 cmd, char *data, int maxlen)
{
    return common_wait_cmd_noRetry(dev0, dev1, key0, key1, token, cmd, data, maxlen, 1);
}

/*
 * 设置服务器参数
 */
int up2pa_set_host(u32 sip, u16 sport)
{
    u8 *p = &server_ip;
    server_ip = sip;
    server_port = sport;
    
    log_level(U_LOG_INFO,"set server ip = %d.%d.%d.%d port = %ld",p[0],p[1],p[2],p[3],sport);

    return 0;
}

/*
 * 边界保护开始
 * 在多线程中通讯序列使用此函数作为开始
 */
void up2pa_border_begin()
{
    flag_break = 1;
    while(flag_run)
    {
        delay_ms(10);
    }
    flag_run = 1;
    flag_break = 0;
}

/*
 * 边界保护结束
 * 在多线程中通讯序列使用此函数作为结束
 */
void up2pa_border_end()
{
    flag_run = 0;
}



/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.cn.
    All right reserved.

    File:    up2ps.c

    No description

    TIME LIST:
    CREATE  skyli   2017-06-27 13:48:30

*******************************************************************************/
#include<pthread.h>
#include <arpa/inet.h>
#include "up2ps.h"
#include "ulink.h"

#define MAX_HOST 32

SOCKET socket_main;
static char recvbuf[1024];
static int server_port;
// todo 
static char recv_queue[MAX_NET_PACKAGE_SIZE];
static u32 recv_len  = 0;
DEVINFO *devinfolist = NULL;
int user_total;

#define MAX_UID 0x1000


static volatile u16 s_packet_idx;


typedef struct{
	u32 uid0;
	u32 uid1;
	u32 ip;
	u16 port;
} UID_ITEM;

UID_ITEM *uid_list;
static int uid_count;

static int user_init()
{
	int i;

	devinfolist = (DEVINFO *)malloc(sizeof(DEVINFO) * MAX_USER);
	memset(devinfolist, 0, sizeof(DEVINFO) * MAX_USER);
	user_total = 0;

#if 0
	for(i = 0; i < MAX_USER; i++)
	{
		devinfolist[i].dev0 = i;
		devinfolist[i].dev1 = i;
		devinfolist[i].ip = i;
		devinfolist[i].port = 0;
		devinfolist[i].time = 0;
	}
#endif
	return 0;
}

DEVINFO *user_find(u32 dev0, u32 dev1)
{
	int i;
	u32 now_time = time(NULL);

    // devinfolist is null.
    if(devinfolist == NULL)
        return NULL;
	// 寻找空闲块
	for(i = 0; i < MAX_USER; i++)
	{
		// 相同的dev
		if(devinfolist[i].dev0 == dev0 && devinfolist[i].dev1 == dev1)
		{
			return &devinfolist[i];
		}
	}

	return NULL;
}

int user_add(u32 dev0, u32 dev1, u32 ip, u16 port)
{
	int i;
	u32 now_time = time(NULL);

	// 寻找空闲块
	for(i = 0; i < MAX_USER; i++)
	{
		// 超时的或者空闲的,都可以使用
		if((now_time - devinfolist[i].time) > UP2P_ONLINE_TIME 
			|| devinfolist[i].time == 0
			|| (devinfolist[i].dev0 == dev0 && devinfolist[i].dev1 == dev1))
		{
			devinfolist[i].dev0 = dev0;
			devinfolist[i].dev1 = dev1;
			devinfolist[i].ip = ip;
			devinfolist[i].port = port;
			devinfolist[i].time = now_time;
			log_level(U_LOG_INFO,"update dev %x %x", dev0, dev1);
			break;
		}
	}
	return 0;
}

int user_deinit()
{
	free(devinfolist);

	return 0;
}
int uid_init(const char *fname)
{
	FILE *fp;
	char buf[256];
	int i;

	fp = fopen(fname, "rb");
	if(fp == NULL)
		goto _err;

	uid_list = malloc(MAX_UID * sizeof(UID_ITEM));
	if(uid_list == NULL)
		goto _err2;

	for(i = 0; i < MAX_UID; i++)
	{
		char ipstr[32];
		u32 port;
		char *ret = fgets(buf, sizeof(buf), fp);
		if(ret == NULL)
			break;
		sscanf(ret, "%08lx%08lx %s %ld", &uid_list[i].uid1, &uid_list[i].uid0, ipstr, &port);
		
		uid_list[i].ip = inet_addr(ipstr);
		uid_list[i].port = htons(port);
	}

	uid_count = i;

	fclose(fp);
	
	return 0;
_err2:
	fclose(fp);
_err:
	log_level(U_LOG_ERROR,"uid_init error");
	return -1;
}

// UDP发送数据包
int _send(const char *data, int len, u32 ip, u16 port)
{
	int addrlen = sizeof(SOCKADDR);
	int ret;
	struct sockaddr_in dstaddr;

	dstaddr.sin_family = AF_INET;
	dstaddr.sin_addr.s_addr = ip;
	dstaddr.sin_port = port;

	ret = sendto(socket_main, data, len, 0, (SOCKADDR*)&dstaddr, addrlen);

	return ret;
}
static int s_send_packet(UP2P_PACKET *packet)
{
    int len = sizeof(UP2P_PACKET) + packet->len;
    DEVINFO *info = user_find(packet->dst0,packet->dst1);
    if(!info){
        log_level(U_LOG_WARN,"DEV %x %x offline",packet->dst0,packet->dst1);
        return U_ERROR_LIINKDEV;
    }
    u8 *ip= (u8*)&(info->ip);
    log_level(U_LOG_WARN,"send packet to %d.%d.%d.%d : %d",ip[0],ip[1],ip[2],ip[3],htons(info->port));
    return _send((const char *)packet, len, info->ip, info->port);
}

/*
 * description: 发送远程指令
 * argv: 
 *      cmd 指令编码 param 指令附加数据 len 附加数据长度
 * return: 没有附加数据 param 可以为NULL, len为0
 */
int s_send_cmd(u32 dev0, u32 dev1, u32 cmd, const char *param, int len,char *p_package)
{
    UP2P_PACKET *packet = (UP2P_PACKET *)p_package;

    packet->magic = MAGIC;
    packet->src0 = 0;
    packet->src1 = 0;
    packet->dst0 = dev0;
    packet->dst1 = dev1;
    packet->cmd = cmd;
    packet->len = len;

    s_packet_idx++;
    packet->idx = s_packet_idx;
    if(len > 0)
        memcpy(packet->payload, param, len);

    
    return s_send_packet(packet);
}
/*
 * descriptin: 查询接收队列，并获取最新的数据  
  * argv:
  *  data 接收缓存 maxlen 缓存区最大长度
  * return: 附加数据的长度 长度也可能为0 -1 无数据
  * todo : there are no queue currently.
  */
int s_recv_data(u32 dev0, u32 dev1, u32 *cmd, char *data, int maxlen)
{
    
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
int s_get_recv_data(u32 dev0, u32 dev1, u32 key0, u32 key1, u32 token, u32 *cmd, char *data, int maxlen)
{
    int len;

    char inbuf[MAX_NET_PACKAGE_SIZE];
    char outbuf[MAX_NET_PACKAGE_SIZE];
    u32 rcmd;
    UP2P_DATA *udata = (UP2P_DATA *)outbuf;

#if 0
    // 读取一个数据包  todo
    len = up2pa_recv_data(dev0, dev1, &rcmd, inbuf, sizeof(inbuf));
    if(len < 0)
        return len;
#endif
    if(recv_len ==0 ) 
        return 0;
    
    memset(inbuf,0,MAX_NET_PACKAGE_SIZE);
    memcpy(inbuf,recv_queue,recv_len);
    len = recv_len;
    
//    if(rcmd != CMD_DATA_ACK)
//        return -1;
    
    if(len > MAX_NET_PACKAGE_SIZE)
        return -1;

    // 解密数据包
    len = data_dec(inbuf, (char*)udata, len, key0, key1);
    if(udata->cmd != CMD_GET_TOKEN_ACK  && udata->cmd != CMD_UPDATE_TOKEN_ACK && udata->token != token)
    {
        log_level(U_LOG_WARN," cmd = %lx diff token recv %lx my %lx", udata->cmd,udata->token,token);
        return -1;
    }
    
    if(udata->len > maxlen)
    {
        log_level(U_LOG_WARN,"receive package is too big  %d >  %d",udata->len,maxlen);
        return -1;
    }

    // 输出数据
    *cmd = udata->cmd;
    if(data)
        memcpy(data, udata->payload, udata->len);
    
    return udata->len;
}

/*
 * 阻塞并等待指定的加密指令
 * cmd 等待的指令 data 接收缓存 maxlen 缓存区最大长度
 * 返回值 附加数据的长度 长度也可能为0 -1 无数据
 */
int s_wait_data_cmd(u32 dev0, u32 dev1, u32 key0, u32 key1, u32 token, u32 cmd, char *data, int maxlen,char *rsbuf)
{
  int ret = 0;
  int len=0,retry=0;

  
  log_level(U_LOG_DEBUG," >>>    s_wait_data_cmd");
  int time_start = (int)time(NULL);
   while(1){
        u32 tmp_cmd = 0;
    // 查询接收队列 是否有缓存包
       ret = s_get_recv_data(dev0,dev1,key0,key1,token,&tmp_cmd,data,maxlen);
       if(cmd == tmp_cmd){
            break;
       }
       // 重发
        // 大于500ms之后,每隔500ms重发最后一个数据包
        if(retry >= 50 && (retry % 50) == 0)
        {
            UP2P_PACKET *packet = (UP2P_PACKET *)rsbuf;
            s_send_packet(packet);
        }

        delay_ms(10);
        retry++;
        
        // 是否超时
        if(time(NULL) - time_start > MAX_WAIT_TIME){
            log_level(U_LOG_WARN,"receive %lx timeout",cmd);
            break;
            }

   }
}

/*
 * description: 进行加密
 * argv:
 *      cmd 指令编码 param 指令附加数据 len 附加数据长度
 * notice: 没有附加数据 param 可以为NULL, len为0
 * return: 已发的数据量.
 */
int s_send_data_cmd(u32 dev0, u32 dev1, u32 key0, u32 key1, u32 token, u32 cmd, const char *param, int len,char *sbuf)
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
    }
    
    log_debug("send_data_cmd token = %x cmd = %x", token, cmd);
    unsigned char *p = (unsigned char *)data;
    len = data_enc((char *)data, outbuf, sizeof(UP2P_DATA) + data->len, key0, key1);

    ret = s_send_cmd(dev0, dev1, CMD_DATA, outbuf, len,sbuf);

    return ret;
}
int s_dev_open(u32 dev0,u32 dev1,u32 key0, u32 key1,u32 *p_token)
{
    int ret;
    char *sbuf[MAX_NET_PACKAGE_SIZE];

    memset(sbuf,0,MAX_NET_PACKAGE_SIZE);
    // is dev online.
    if( !user_find(dev0,dev1)){
        log_level(U_LOG_INFO,"dev %x %x offline",dev0,dev1);
        return U_ERROR_LIINKDEV;
    }
    // update session token
    ret = s_send_data_cmd(dev0,dev1,key0,key1,0,CMD_GET_TOKEN,NULL,0,&sbuf);
    if(ret  < 0)
        return ret;
    // GET respond
    ret = s_wait_data_cmd(dev0,dev1,key0,key1,0,CMD_GET_TOKEN_ACK,p_token,sizeof(u32),&sbuf);
    log_level(U_LOG_DEBUG," >>> TOKEN %p \n", *p_token);

    return ret;
}

/*
 * description:  向dev 发送一条控制指令
 * argv:
 *      cmd 指令编码; cmd_ack 等待回应的 ack.
 *      dev 设备id   ；
 *      key 设备 aes 的key
 *      data 控制的数据.
 * notice: 每次都刷新 dev 的token，一个控制指令 两个来回.
 * return: 指令是否控制成功.
 */
int s_cmd_send(u32 cmd,u32 cmd_ack,const char *dev,const char *key,void *sdata,int slen,char *rdata,int *rlen){
    int ret = 0;
    u32 dev0,dev1,key0,key1,token;
    // todo malloc
    char *sbuf[MAX_NET_PACKAGE_SIZE];

    memset(sbuf,0,MAX_NET_PACKAGE_SIZE);
    ret = ascii_to_2u32(dev,&dev0,&dev1);
    if( ret < 0) {
        log_level(U_LOG_WARN,"illegal dev %s", dev);
        return ret;
    }
    
    ret = ascii_to_2u32(key,&key0,&key1);
    if( ret < 0 ) {
        log_level(U_LOG_WARN,"illegal key %s", key);
        return ret;
    }
   
    ret = s_dev_open(dev0,dev1,key0,key1,&token);
    if(ret < 0){
        log_level(U_LOG_WARN,"server open dev %x %x fail");
        return ret;
    }
    ret = s_send_data_cmd(dev0,dev1,key0,key1,token,cmd,sdata,slen,sbuf);
    if(ret < 0)
        return ret;
    
    // wait respond
    ret = s_wait_data_cmd(dev0,dev1,key0,key1,token,cmd_ack,rdata,*rlen,sbuf);
    if(ret >=0)
        *rlen = ret;
        
    return ret;
    // wait cmd ack.
}

static int recv_cmd(UP2P_PACKET *packet, u32 ip, u16 port)
{
	DEVINFO *devinfo;
	UP2P_HOST *host;
	u32 tmp0, tmp1;

	// 将源端加入列表
	// todo 没有检查当前包的合法性.
	user_add(packet->src0, packet->src1, ip, port);

	//log_level(U_LOG_DEBUG,"cmd %lx", packet->cmd);

	switch(packet->cmd)
	{
	case CMD_ONLINE:
		// 回应ACK
		packet->cmd = CMD_ONLINE_ACK;
		packet->dst0 = packet->src0;
		packet->dst1 = packet->src1;
		packet->src0 = 0;
		packet->src1 = 0;
        
        log_level(U_LOG_DEBUG,"send to ip %ld  port %ld",ip,htons(port));
		_send((const char *)packet, sizeof(UP2P_PACKET), ip, port);
		break;

	case CMD_CHECK_ONLINE:
		// 查询在线
		devinfo = user_find(packet->dst0, packet->dst1);
		if(devinfo)
		{
			if(time(NULL) - devinfo->time < UP2P_ONLINE_TIME)
			{
				packet->cmd = CMD_CHECK_ONLINE_YES;
			}
			else
			{
				packet->cmd = CMD_CHECK_ONLINE_NO;
			}
		}
		else
		{
			packet->cmd = CMD_CHECK_ONLINE_NO;
		}
        // 回应 online
		_send((const char *)packet, sizeof(UP2P_PACKET), ip, port);
		break;

	case CMD_GET_HOST:
		// 获取主机信息
		host = (UP2P_HOST*)packet->payload;
        // 集群 分配server 
		host->ip = uid_list[0].ip;
		host->port = uid_list[0].port;
		tmp0 = packet->src0;
		tmp1 = packet->src1;
		packet->src0 = packet->dst0;
		packet->src1 = packet->dst1;
		packet->dst0 = tmp0;
		packet->dst1 = tmp1;
		packet->cmd = CMD_GET_HOST_ACK;
		packet->len = sizeof(UP2P_HOST);
		_send((const char *)packet, sizeof(UP2P_PACKET) + packet->len, ip, port);
		break;

	default:
		// 其他的指令,统一做转发处理
		if( packet->dst0 == 0 && packet->dst1 == 0){
            // these package was to server            
            // push package to queue
            //todo 
            if(packet->cmd != CMD_DATA_ACK){
                log_level(U_LOG_INFO, "recive an none ack packet");
                return 0;
          }
            memset(recv_queue,0,MAX_NET_PACKAGE_SIZE);
            memcpy(recv_queue,packet->payload,packet->len);
            recv_len = packet->len;
        }else{
                
    		devinfo = user_find(packet->dst0, packet->dst1);
    		if(devinfo)
    		{
    			log_level(U_LOG_INFO,"relay %x %x", packet->dst0, packet->dst1);
    			_send((const char *)packet, sizeof(UP2P_PACKET) + packet->len, devinfo->ip, devinfo->port);
    		}
    		else
    		{
    			log_level(U_LOG_INFO,"not found %x %x", packet->dst0, packet->dst1);
    		}
        }
		break;
	}
	return 0;
}

static int recv_proc(void *param)
{
	int size;
	struct sockaddr_in sin_recv;
	int addrlen = sizeof(SOCKADDR);
	UP2P_PACKET *packet;

    if(socket_main <= 0 || recvbuf == NULL){
        log_level(U_LOG_ERROR,"Illegal pare socket = %d bufff addr = %p ",recvbuf);
        return U_ERROR_INVALID;
    }
	while(1)
	{
		size = recvfrom(socket_main, recvbuf, sizeof(recvbuf), 0, (SOCKADDR*)&sin_recv, &addrlen);

		log_level(U_LOG_INFO,"recv from:%s:%ld len:%d ", inet_ntoa(sin_recv.sin_addr),htons(sin_recv.sin_port), size);

		packet = (UP2P_PACKET *)recvbuf;
		if(packet->magic != MAGIC){
            log_level(U_LOG_DEBUG,"magic no match !!");
            continue;
        }
        recv_cmd(packet, sin_recv.sin_addr.s_addr, sin_recv.sin_port);
	}
    
	return 0;
}

int up2ps_socket_init(void){
    struct sockaddr_in sin;
        
    // set log level 
   // log_level(U_LOG_INFO,"Creat  %s %s", __DATE__, __TIME__);
    
    server_port = UP2PS_PORT;
    // load ip list 
    if(uid_init("../config/up2ps.ini") == -1)
    {
        log_level(U_LOG_ERROR,"host list is missing");
        return U_ERROR_NULL;
    }

    user_init();

    // 创建socket
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(server_port);
    socket_main = socket(AF_INET,SOCK_DGRAM, 0);

    if(socket_main <= 0){
        log_level(U_LOG_ERROR,"open socket FAILT !!");
        return U_ERROR_NULL;
    }
    
    bind(socket_main, (SOCKADDR*)&sin, sizeof(SOCKADDR));
    log_level(U_LOG_INFO,"socket listen to %d",server_port);
    

}
int up2ps_relay_start(){

    up2ps_socket_init();
    ctrl_init();
    // 处理转发业务
    recv_proc(NULL);

    user_deinit();

    uid_deinit();

    return 0;
}
int up2ps_dev_online(const char *dev)
{
    int ret;
    u32 dev0,dev1;
    ret = ascii_to_2u32(dev, &dev0, &dev1);
    if(ret < 0 )
        return 0;
    if(user_find(dev0, dev1))
        return 1;
    else 
        return 0;
}
int uid_deinit()
{
	free(uid_list);
	return 0;
}

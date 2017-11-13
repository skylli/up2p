/*******************************************************************************
    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    up2pc.c

    No description

    TIME LIST:
    CREATE  skyli   2017-06-26 13:47:42

*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "up2pc.h"

#include "up2p_crypt.h"

#ifdef MAC_LINUX

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#endif
// 用户密码
static u32 up2pc_key0, up2pc_key1;
// 服务器参数
static u32 server_ip;
static u16 server_port;
// 本机参数
static u32 local_ip;
static u16 local_port;
static u32 up2pc_dev0;
static u32 up2pc_dev1;

// 在线时间戳
static u32 online_time;
static u32 global_token;
static u16 token_idx;

static u8 *p_status_buf = NULL;
static u8 status_buf_len = 0;
typedef struct{
    
    u32 dsrc0;
    u32 dsrc1;
    u32 dip;
    u16 dport;
    u8 index;
    u8 retransmit;

} Observer_ST;

static Observer_ST observer[MAX_OBSERS];

// 发送缓冲,保留最后一个包的数据  todo
static char send_buf[256];

static int _file_write(UDP_INFO *info, u32 dst0, u32 dst1, u16 idx, const char *data, int len);
static int _file_init(const char * fname,int flen,UDP_INFO *info, u32 dst0, u32 dst1, u16 idx);

/*
 * 发送数据
 */
static int up2pc_send(const char *data, int len, UDP_INFO *info)
{
    int ret;

    ret = up2pc_udp_send(data, len, info);
    if(ret != len)
        return -1;

    return 0;
}

/*
 * 回应数据
 * UDP_INFO的源和目标交换,发送出去
 */
static int up2pc_ack(const char *data, int len, UDP_INFO *info)
{
    UDP_INFO ack;

    ack.dip = info->sip;
    ack.dport = info->sport;
    ack.sip = info->dip;
    ack.sport = info->dport;

    up2pc_send(data, len, &ack);

    return 0;
}

// 发送命令包
static int up2pc_send_cmd(UDP_INFO *info, u32 dst0, u32 dst1, u32 cmd, u16 idx, const char *param, int len)
{
    UP2P_PACKET *packet = (UP2P_PACKET *)send_buf;

    packet->magic = MAGIC;
    packet->src0 = up2pc_dev0;
    packet->src1 = up2pc_dev1;
    packet->dst0 = dst0;
    packet->dst1 = dst1;
    packet->cmd = cmd;
    packet->idx = idx;
    packet->len = len;

    
    if(packet->len > 0 && packet->len<255)
    {
        memcpy(packet->payload, param, len);
    }
    else if( packet->len<0)
    {
        w_debug_level(U_LOG_WARN,"payload LEN <0, the packet will not be send");
        return -1;
    }    
    return up2pc_send((const char *)packet, sizeof(UP2P_PACKET) + len, info);
}

// 回复命令包
static int up2pc_ack_cmd(UDP_INFO *info, u32 dst0, u32 dst1, u32 cmd, u16 idx, const char *param, int len)
{
    UDP_INFO ack;

    ack.dip = info->sip;
    ack.dport = info->sport;
    ack.sip = info->dip;
    ack.sport = info->dport;

    return up2pc_send_cmd(&ack, dst0, dst1, cmd, idx, param, len);
}

// 回复加密数据包
static int up2pc_ack_data_cmd(UDP_INFO *info, u32 dst0, u32 dst1, u32 cmd, u16 idx, const char *param, int len)
{

    
    char *pkbuf = NULL;
    char *encbuf = NULL;

    UP2P_DATA *data = NULL;// (UP2P_DATA *)pkbuf;
    int enclen;
    int ret;
    u8 i;
    
    //+16 加密长对齐
    pkbuf = (char *)malloc(2*(sizeof(UP2P_DATA)+len +16));
    if(pkbuf == NULL)
        {
        int alloc_len = 2 * sizeof(UP2P_DATA)+len +16; 
        w_debug_level(U_LOG_ERROR,"malloc pkbuf failt;len = %d",alloc_len);
        return -1;
    }
	
    encbuf = (char *)&pkbuf[(sizeof(UP2P_DATA)+len)];

    data = (UP2P_DATA *)pkbuf;//pkbuf;
    data->cmd = cmd;
    data->token = global_token;
    data->len = len;
    
    for(i=0;i<len;i++)
        {
        data->payload[i] = param[i];
    }
	
    enclen = data_enc((char *)data, encbuf, (sizeof(UP2P_DATA) + len), up2pc_key0, up2pc_key1);

    ret = up2pc_ack_cmd(info, dst0, dst1, CMD_DATA_ACK, idx, encbuf, enclen);

    if( pkbuf != NULL ){
        free(pkbuf);
        pkbuf = NULL; 
    }

    return ret;
}

/*
* file transmit 
*/
#ifdef MAC_LINUX

int fp = 0;
static long foffset =0;

#endif    
static u32 g_index = 0;

/***
*description: 传输文件前的初始化或工作
***/
static int _file_init(const char * fname,int flen,UDP_INFO *info, u32 dst0, u32 dst1, u16 idx){
    g_index = 1;
    int ret = -1;
#ifdef MAC_LINUX // only linux 
    fp = open("demofile.bin", O_CREAT | O_WRONLY,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    w_debug_level(U_LOG_DEBUG,"creatnewbinfile\n");
    if(fp == -1 )
    {
        perror("demofile.bin:");
        return -1;
    }
#endif  //  MAC_LINUX
    return up2pc_ack_data_cmd(info, dst0,dst1, CMD_FILE_NAME_ACK,idx, NULL, 0);
}

static int _file_write(UDP_INFO *info,u32 dst0,u32 dst1, u16 idx,const char *data,int len)
{
    FILE_T respon;
    FILE_T *p_buf = (FILE_T*) data;

    memset(&respon,0,sizeof(FILE_T));

    if(!data || len < sizeof(FILE_T)){
        respon.type = FILE_CMD_CAN;                
        goto FILE_RESPON;
    }                

    w_debug_level(U_LOG_DEBUG," type %d len %d\n",p_buf->type,p_buf->len);
    w_debug_level(U_LOG_INFO,"file %s\n",p_buf->payload);
#if 1
    switch(p_buf->type){
        
        case FILE_CMD_TXT:
            w_debug(">>> receive index %d  my index %d\n",p_buf->index,g_index);
            if( p_buf->index != g_index){
                // retransmit 
                respon.type = FILE_CMD_RET; 
                respon.index = g_index; 
                //goto FILE_RESPON;
            }else{

#ifdef MAC_LINUX                    
                int ret = write(fp,p_buf->payload,p_buf->len);
                if(ret < 0 ){
                        if(fp != -1)
                            close(fp);
                        fp = -1;
                        respon.type = FILE_CMD_CAN;
                        
                        w_debug("22");
                        //goto FILE_RESPON;
                    }
                else {
                    g_index++;
                    respon.index = g_index;
                    respon.type = FILE_CMD_ACK;
                }
                //goto FILE_RESPON;
#else 
                g_index++;
                respon.index = g_index;
                respon.type = FILE_CMD_ACK;
#endif
               }
            break;
        case FILE_CMD_END:
               g_index = 0;
               respon.type = FILE_CMD_ACK;
                
               w_debug("file write end \n close file \n");
                // close file;
#ifdef MAC_LINUX                        
                if(fp != -1)
                    close(fp);
                fp = -1;
#endif
            break;
        default:
            break;
        }
#endif    
FILE_RESPON:            
    w_debug("respon.type %d",respon.type);
    return up2pc_ack_data_cmd(info, dst0, dst1, CMD_FILE_TXT_ACK,idx, &respon, sizeof(FILE_T));
}

// 处理命令
static int up2pc_recv_cmd(UP2P_PACKET *inpkt, int len, UDP_INFO *info)
{
	int ret = -1,dec_len;
	u8 *p = NULL;
	UP2P_PACKET *lastsend = (UP2P_PACKET *)send_buf;
	UP2P_PACKET *packet = NULL;
	UP2P_DATA *data = NULL;
    
	UP2P_TOKEN tmp;
	packet = (UP2P_PACKET *)malloc( sizeof(UP2P_PACKET) + len );
	if( packet == NULL )
	{
		w_debug_level(U_LOG_ERROR,"dec memory malloc failt!!\n");
		return ret;
		}
	data = (UP2P_DATA *)packet->payload;
	memcpy(packet, inpkt, sizeof(UP2P_PACKET));

	// 解密
	//w_dump("raw payload  ",inpkt->payload,inpkt->len);
  	dec_len = data_dec(inpkt->payload, packet->payload, packet->len,up2pc_key0, up2pc_key1);
    
	//w_dump("payload after decode ",packet->payload,dec_len);

    if(data->cmd == CMD_GET_TOKEN){
        w_debug_level(U_LOG_INFO," get token  %x \n", global_token);
		ret = up2pc_ack_data_cmd(info, packet->src0, packet->src1,\
			CMD_GET_TOKEN_ACK,packet->idx, (char *)&global_token, sizeof(UP2P_TOKEN));
        goto _RECV_CMDHANDLE_END;
    }
    
	// 除了刷新令牌的命令,都要验证令牌
	if(data->cmd == CMD_UPDATE_TOKEN)
	{
		// 刷新令牌
		// 对于重发的请求,使用最后一次的值
		if(packet->idx == token_idx)
		{
			tmp.token = global_token;
		}
		else
		{
			tmp.token = sys_get_random();
		}
		token_idx = packet->idx;
		// 刷新令牌的指令回复的token是0
		global_token = 0;
		ret = up2pc_ack_data_cmd(info, packet->src0, packet->src1,\
			CMD_UPDATE_TOKEN_ACK,packet->idx, (char *)&tmp, sizeof(UP2P_TOKEN));
        
		global_token = tmp.token;
		w_debug_level(U_LOG_WARN,"update token to %x \n", global_token);
		w_debug_level(U_LOG_WARN,"sessiong has been build");
		goto _RECV_CMDHANDLE_END;
	}
	else
	{
		if(data->token != global_token)
		{
			// 回应令牌错误
			
            w_debug_level(U_LOG_WARN,"token errorl recv %x my is %x \n",data->token,global_token);
			ret = up2pc_ack_data_cmd(info, packet->src0, packet->src1,\
										CMD_DATA_KEY_ERR, packet->idx, NULL, 0);
			//w_debug_level(U_LOG_WARN,"token was not match receive %lx my token is %lx", data->token, global_token);
			goto _RECV_CMDHANDLE_END;
		}
	}
    
	// 对于重发的指令, 直接回应
	if(lastsend->magic == MAGIC &&
		lastsend->idx == packet->idx &&
		lastsend->dst0 == packet->src0 &&
		lastsend->dst1 == packet->src1)
	{
		w_debug_level(U_LOG_INFO,"retransmit packet idx %d\n", packet->idx);
		ret = up2pc_send((const char *)lastsend, sizeof(UP2P_PACKET) + lastsend->len, info);		
		goto _RECV_CMDHANDLE_END;
	}

	lastsend->dst0 = packet->dst0;
	lastsend->dst1 = packet->dst1;

    w_debug_level(U_LOG_INFO,"receive cmd:%x \n",data->cmd);
	switch(data->cmd)
	{
        // 订阅状态
        case CMD_DEV_STATE_OBSERVER:

            w_debug_level(U_LOG_DEBUG,"CMD_DEV_STATE_OBSERVER device id is %x  %x\n",packet->src0,packet->src1);
            up2pc_observer_host_register(packet->src0, packet->src1,info->sip,info->sport);
            up2pc_ack_data_cmd(info, packet->src0, packet->src1, CMD_DEV_STATE_OBSERVER_ACK, packet->idx, NULL, 0);
        	break;

        case CMD_DEV_STATE_UNOBSERVER:

            w_debug_level(U_LOG_DEBUG,"CMD_DEV_STATE_UNOBSERVER device id is %x  %x\n",packet->src0,packet->src1);
            up2pc_observer_host_unregister(packet->src0, packet->src1,info->sip,info->sport);
            up2pc_ack_data_cmd(info, packet->src0, packet->src1, CMD_DEV_STATE_UNOBSERVER_ACK, packet->idx, NULL, 0);
        	break;

        case CMD_DEV_STATE_UPDATE_ACK:

            w_debug_level(U_LOG_DEBUG,"CMD_DEV_STATE_UPDATE_ACK device id is %x  %x\n",packet->src0,packet->src1);
            up2pc_update_devState_ack(packet->src0, packet->src1,info->sip,info->sport);
            break;

        case CMD_SEND_SERIAL:
            
            w_debug_level(U_LOG_INFO, " CMD_SEND_SERIALc \n");
            w_debug_level(U_LOG_INFO, " len %d \n",data->len);
        	// 串口数据
        	serial_write((const char*)data->payload, data->len);
        	// 回应
        	up2pc_ack_data_cmd(info, packet->src0, packet->src1, CMD_SEND_SERIAL_ACK, packet->idx, NULL, 0);
        	break;
        case CMD_READ_SERIAL:
        	
        	// 读取串口数据
        	// 读取串口数据
        	w_debug("CMD_READ_SERIAL");
        	ret = serial_read(data->payload, MAX_SERIAL_SIZE);
        	if(ret > 0)
        	{
        		// 回应
        		up2pc_ack_data_cmd(info, packet->src0, packet->src1,\
        						CMD_READ_SERIAL_ACK, packet->idx, data->payload, ret);
        	}
        	break;
        case CMD_UPATA_SERIAL_ACK:
        	break;
        	
        case CMD_SEND_KEY:
        	p = (u8*)data->payload;
        	memcpy(&up2pc_key0,p,sizeof(up2pc_key0));
        	memcpy(&up2pc_key1,(p+sizeof(up2pc_key0)),sizeof(up2pc_key1));
        	
        	w_debug_level(U_LOG_INFO,"setting aes key0 = %x key1 = %x",up2pc_key0,up2pc_key1);
        	// updata flash key. todo
        	//if(sys_set_key(up2pc_key0, up2pc_key1) !=0 )
        	//break;
        	up2pc_ack_data_cmd(info, packet->src0, packet->src1, CMD_SEND_KEY_OK, packet->idx, NULL, 0);
        	break;

        case CMD_CONFIG_HOST:
        	// 配置服务器信息
        	// 模块端注意: 即使DNS解析不成功也要保存数据并返回ACK
        	// 但是写flash保存失败,则不要回应ACK
        	{
        		UP2P_CONFIG_HOST *chost = (UP2P_CONFIG_HOST *)data->payload;
        		//更新服务器的host 和port 到flash. todo.
        		if(sys_config_host(chost->name, chost->port) == 0)
        		{
        		    // 获取 host
                    up2pc_update_host();
        			// 设置成功回应
        			up2pc_ack_data_cmd(info, packet->src0, packet->src1, CMD_CONFIG_HOST_ACK, packet->idx, NULL, 0);
        			break;
        		}
        	}
        	break;

        case CMD_TXRX_SERIAL:
        	// 串口数据
        	serial_write(data->payload, data->len);
        	// 读取串口数据
        	// todo serial byte should store in another menory.
        	ret = serial_read(data->payload, MAX_SERIAL_SIZE);
        	if(ret > 0)
        	{
        		// 回应
        		up2pc_ack_data_cmd(info, packet->src0, packet->src1, CMD_TXRX_SERIAL_ACK, packet->idx, data->payload, ret);
        	}
        	break;
        case CMD_GPIO_INIT:
        	// todo serial byte should store in another menory.
        	if(data->len < 2)
        		break;
        	{			
        		ret = io_mode_int(data->payload[0], data->payload[1]);
        		// 回应
        		ret = up2pc_ack_data_cmd(info, packet->src0, packet->src1, CMD_GPIO_INIT_ACK, packet->idx, NULL, 0);
        	}
        	break;	
        case CMD_GPIO_WRITE:
        	// todo serial byte should store in another menory.
        	if(data->len < 2)
        		break;
        	{			
        	    ret = io_mode_int(data->payload[0],1);
        		ret = io_write(data->payload[0], data->payload[1]);
        		// 回应
        		ret = up2pc_ack_data_cmd(info, packet->src0, packet->src1, CMD_GPIO_WRITE_ACK, packet->idx, NULL, 0);
        	}
        	break;

        case CMD_FILE_NAME:
            ret = _file_init(data->payload,data->len,info, packet->src0, packet->src1,packet->idx);
            break;

        case CMD_FILE_TXT:
            ret =  _file_write(info,packet->src0, packet->src1,packet->idx,data->payload,data->len);
            break;
        default:
            break;
	}
_RECV_CMDHANDLE_END:
	
	if(packet)
		free(packet);
	return ret;
}

/*
 * 接收数据处理函数
 */
int up2pc_recv(const char *data, int len, UDP_INFO *info)
{
	UP2P_PACKET *packet = (UP2P_PACKET *)data;
	UP2P_HOST *host;
	int ret = -1;
	
	// 验证数
	if(packet->magic != MAGIC)
	{
		w_debug_level(U_LOG_WARN,"MAGIC not match,recv %d,my is %d%d",packet->magic,MAGIC);
		return -1;
	}
	
	//验证dev
	w_debug("receive dev id %lx %lx  my dev id %lx %lx",packet->dst0,packet->dst1,up2pc_dev0,up2pc_dev1);
	if( packet->dst0 != up2pc_dev0 || packet->dst1 != up2pc_dev1)
	{
		w_debug_level(U_LOG_WARN,"receive packet identify not match,receive dev id [%lx][%lx],my dev id is [%lx][%lx]",\
					packet->dst0,packet->dst1,up2pc_dev0,up2pc_dev1);
		return -1;
	}
    
	switch(packet->cmd)
	{
		case CMD_ONLINE_ACK:
		// 服务器回应在线
		online_time = sys_get_time();
		w_debug_level(U_LOG_DEBUG,"receive cmd CMD_ONLINE_ACK");
		ret = 0;
		break;

		case CMD_PING:
		// 回应测试指令
		w_debug_level(U_LOG_DEBUG,"receive cmd CMD_PING");
		ret = up2pc_ack_cmd(info, packet->src0, packet->src1, CMD_PING_ACK, packet->idx, NULL, 0);
		break;

		case CMD_LAN_SCAN:
		// APP局域网扫描在线状态
		w_debug_level(U_LOG_DEBUG,"receive cmd CMD_LAN_SCAN");
		
		w_debug(">>>>> udp rec info sip %lx sport %lx dip %lx dport %lx",info->sip,info->sport,info->dip,info->dport);
		ret = up2pc_ack_cmd(info, packet->src0, packet->src1, CMD_LAN_SCAN_ACK, packet->idx, NULL, 0);
		break;

		case CMD_GET_HOST_ACK:
		// 获取服务器信息返回
		host = (UP2P_HOST*)packet->payload;
		local_ip = host->ip;
		local_port = host->port;
		w_debug_level(U_LOG_DEBUG,"receive cmd CMD_GET_HOST_ACK Update host %x %x\n", local_ip, local_port);
		break;

		case CMD_DATA:
			
		w_debug_level(U_LOG_DEBUG,"receive cmd CMD_DATA");
		// 加密的数据
		ret = up2pc_recv_cmd(packet, len, info);
		
		break;
	}

	return ret;
}
/*
 * 定时更新在线状态的回调函数
 * 至少30秒调用一次
 */
int up2pc_set_online()
{
    UDP_INFO info;

    info.dip = server_ip;
    info.dport = server_port;
    info.sip = local_ip;
    info.sip = local_port;

    return up2pc_send_cmd(&info, 0, 0, CMD_ONLINE, 0, NULL, 0);
}

/*
 * 更新服务器信息
 * 由应用层在长时间无法连接时调用(例如重新解析完DNS之后)
 * 之前必须设置过dev0 dev1
 */
int up2pc_update_host()
{
    UDP_INFO info;
    UP2P_DEVID devid;

    info.dip = server_ip;
    info.dport = server_port;
    info.sip = local_ip;
    info.sip = local_port;

    devid.dev0 = up2pc_dev0;
    devid.dev1 = up2pc_dev1;

    return up2pc_send_cmd(&info, 0, 0, CMD_GET_HOST, 0,(const char*)&devid, sizeof(UP2P_DEVID));
}

// 设置用户密钥
int up2pc_setkey(u32 key0, u32 key1)
{
    w_debug("update aes key %ld %ld",key0,key1);
    up2pc_key0 = key0;
    up2pc_key1 = key1;

    return 0;
}

/*
 * 查询在线状态
 * 返回0 在线
 * -1 不在线
 */
int up2pc_get_online()
{

    w_debug(".....sysgettime: %d;onlinetime= %d;",sys_get_time(),online_time);
    if(sys_get_time() - online_time > UP2P_ONLINE_TIME)
        return -1;

    return 0;
}

// 设置服务器ip和端口
int up2pc_set_server(u32 ip, u16 port)
{
	w_debug("seting server ip : %ld, port :%ld",ip,port);
	
    server_ip = ip;
    server_port = port;

    return 0;
}
// register ip and port of server 
int up2pc_observer_host_register(u32 sr0,u32 sr1,u32 ip, u16 port)
{
	w_debug("register observer host : %ld, port :%ld",ip,port);  
  int i=0;
  if(ip ==0 || port == 0)
    return 0;
  // find equel
  for(i=0;i<MAX_OBSERS;i++){
    if(  observer[i].dsrc0 == sr0 && observer[i].dsrc1 == sr1 &&  observer[i].dip == ip && observer[i].dport == port ){
      observer[i].index = 0;
      return 0;
    }
  }
  // find empty
  for(i=0;i<MAX_OBSERS;i++){
    if(  observer[i].dsrc0 == 0 && observer[i].dsrc1 == 0 && ( observer[i].dip == 0 || observer[i].dport == 0 )){
      
        observer[i].dsrc0 = sr0;
        observer[i].dsrc1 = sr1;
        observer[i].dip = ip;
        observer[i].dport = port;
        observer[i].index = 0;
        observer[i].retransmit = 0;
        return 0;
    }
  }
  return -1;
}
int up2pc_observer_host_unregister(u32 sr0,u32 sr1,u32 ip, u16 port)
{
	w_debug("register observer host : %ld, port :%ld",ip,port);  
  int i=0;
  if(ip ==0 || port == 0)
    return 0;
  // find equel
  for(i=0;i<MAX_OBSERS;i++){
    if(  observer[i].dsrc0 == sr0 && observer[i].dsrc1 == sr1 &&  observer[i].dip == ip && observer[i].dport == port ){
      observer[i].dsrc0 = 0;
      observer[i].dsrc1 = 0;
      observer[i].dip = 0;
      observer[i].dport = 0;
      observer[i].index = 0;
      observer[i].retransmit = 0;

      return 0;
    }
  }
  return -1;
}

// 设置本机ip和端口
int up2pc_set_local(u32 ip, u16 port)
{
	w_debug("seting local ip : %ld, port :%ld",ip,port);
    local_ip = ip;
    local_port = port;

    return 0;
}

// 设置本机设备名
int up2pc_set_name(u32 dev0, u32 dev1)
{
    w_debug("setting dev id %lx %lx",dev0,dev1);
    up2pc_dev0 = dev0;
    up2pc_dev1 = dev1;

    return 0;
}
// 更新设备状态
static int up2pc_update_devStatus_send(u8 *pdata,u32 len){
  UDP_INFO info;
  int i = 0;

  if(!pdata ||  len ==0)
    return U_ERROR_INVALID;
  
  info.sip = local_ip;
  info.sip = local_port;

  for(i=0;i<MAX_OBSERS;i++){
    if( observer[i].dip != 0 && observer[i].dport != 0 ){
        info.dip = observer[i].dip;
        info.dport = observer[i].dport;
        up2pc_ack_data_cmd(&info, observer[i].dsrc0, observer[i].dsrc1, CMD_DEV_STATE_UPDATE,observer[i].index++, pdata,len);
        observer[i].retransmit = MAX_RETRANSMITS;
    }
  }

  return 0;
}
// 重传 设备状态
int up2pc_update_devStatus_retransmit(u8 *pdata,u32 len){
  UDP_INFO info;
  int i = 0;

  if(!pdata ||  len ==0)
    return -1;
  info.sip = local_ip;
  info.sip = local_port;

  for(i=0;i<MAX_OBSERS;i++){
    if( observer[i].retransmit > 0 && observer[i].dip != 0 && observer[i].dport != 0 ){
        info.dip = observer[i].dip;
        info.dport = observer[i].dport;
        up2pc_ack_data_cmd(&info, observer[i].dsrc0, observer[i].dsrc1, CMD_DEV_STATE_UPDATE,observer[i].index++, pdata,len);
        observer[i].retransmit--;
    }
  }
  return 0;
}

int up2pc_update_devState_ack(u32 sr0,u32 sr1,u32 ip, u16 port){
  int i = 0;
  
  for(i=0;i<MAX_OBSERS;i++){
     if(  observer[i].dsrc0 == sr0 && observer[i].dsrc1 == sr1 &&  observer[i].dip == ip && observer[i].dport == port ){
       observer[i].retransmit = 0;
       return 0;
     }
   }
  return 0;
}
// 有新的状态变化
int up2pc_update_dev_status(u8 *pdata,u32 len){

  u8 *p = NULL;

  if( !pdata || !len)
    return U_ERROR_INVALID;
    
  p = (u8*)malloc(len+1);
  if(!p)
    return U_ERROR_NULL;

  memset(p,0,len+1);
  
  if(p_status_buf){
    free(p_status_buf);
    status_buf_len = 0;
    p_status_buf = NULL;
  }
  p_status_buf = p;
  status_buf_len = len;

  memcpy(p_status_buf,pdata,len);
  return up2pc_update_devStatus_send(p_status_buf,len);
}
int up2pc_retransmit(void){

  return up2pc_update_devStatus_retransmit(p_status_buf,(u32)status_buf_len);
}


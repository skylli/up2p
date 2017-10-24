/*******************************************************************************

    This file is part of the ulink.
    Copyright wilddog.com
    All right reserved.

    File:    ulink.c

    No description

    TIME LIST:
    CREATE  skyli   2014-08-27 13:16:17

*******************************************************************************/

#include "up2pa.h"
#include "ulink.h"
#include <sys/time.h>

struct ULINK{
    u32 dev0; // 目标设备的 dev
    u32 dev1;
    u32 key0; // 当前会aes 的秘钥
    u32 key1; // 
    u32 token; // 当前回话的 token
    u32 sip;   // 会话的源 ip.
    u16 sport;
};

static char server_name[64];

int StartSmartConnection(const char *, const char *, char);
int StopSmartConnection();

/*
 * description: 初始化库
 * argv:
 *  host 服务器域名或者IP地址
 *  appid 代表本机唯一的ID码 16字节HEX字符串
 * return: 0成功
 */
int ulink_init(const char *host, const char *appid,u16 sport)
{
    int ret;
    u32 mac0, mac1;
    u16 aport, cport,ssport; 
    ascii_to_2u32(appid, &mac0, &mac1);

    log_debug("libulink Build %s %s\n", __DATE__, __TIME__);
    log_level(U_LOG_INFO,"link to server : %s,setting my dev id to %s",host,appid);
    
    strcpy(server_name, host);
    aport = htons(UP2PA_PORT);
    ssport = htons(sport);
    cport = htons(UP2PC_PORT);
    ret = up2pa_init(aport, host, ssport, cport, mac0, mac1);

    return 0;
}

/*
 * 卸载库
 */
int ulink_deinit()
{
    up2pa_deinit();

    return 0;
}

static int gen_key(u32 *key0, u32 *key1)
{
    // 生成key

    struct timeval tv;
    gettimeofday(&tv, NULL);

    *key0 = (tv.tv_sec * tv.tv_usec) ^ tv.tv_usec;
    delay_ms(500);
    *key1 = (tv.tv_sec * tv.tv_usec) ^ tv.tv_usec;

    return 0;
}

/*
 * description : 配置设备 配置连接路由
 * arg：
 *      devid：16字节的设备ID字符串,注意一般是 mac 地址
 *      ssid：将要连接到的接入点SSID
 *      key：接入点的密钥
 *      type：加密类型
 * return： ULINK_ERR 当且仅当返回ULINK_ERR_NONE时表示成功
 * outkey 返回密钥字符串
 * detail:  todo 需要分层
 *          1.smartconfig
 *          2.CMD_GET_HOST 获取服务器的ip.
 *          3.CMD_CHECK_ONLINE 检测设备是否在线.
 *          4.CMD_UPDATE_TOKEN 获取当前回话的token.
 *          5.CMD_CONFIG_HOST 配置设备的host.
 *          6.CMD_SEND_KEY 更新设备 aes 加密的key.
 */
int ulink_config(const char *devid, const char *ssid, const char *key, int type, char *outkey)
{
    u32 token, dev0, dev1, key0, key1;
    int ret, i, status;
    UP2P_TOKEN stk;
    UP2P_HOST host;
    UP2P_CONFIG_HOST cfgh;

    // 配置wifi
#if TARGET_OS_IPHONE
    //InitSmartConnection();
#endif
#if defined(__ANDROID__) || defined(TARGET_OS_IPHONE)

#if 0
    ret = StartSmartConnection(ssid, key, (char)type);
    if(ret != 0)
    {
        log_level(U_LOG_ERROR,"StartSmartConnection error");
    }
#endif // todo 

#endif // end __ANDROID__

    // 配置key
    ret = ascii_to_2u32(devid, &dev0, &dev1);
    if(ret == -1)
    {
        ret = ULINK_ERR_DEVID_INVILD;
        goto _err;
    }
    
    log_level(U_LOG_INFO,"target dev id is %s",devid);

    // 根据时间生成，todo 集群模式下需要修改
    gen_key(&key0, &key1);


    status = ULINK_ERR_DEV_OFFLINE;
    
#if 0
    // 获取服务器信息
    up2pa_send_cmd(dev0, dev1, CMD_GET_HOST, NULL, 0);
    ret = up2pa_wait_cmd(dev0, dev1, CMD_GET_HOST_ACK, &host, sizeof(UP2P_HOST));
    if(ret >= 0)
    {
        // 有服务器信息则保存
        up2pa_set_host(host.ip, host.port);
    }
#endif
    
    // 初始化,设置客户端密钥
    for(i = 0; i < MAX_CONFIG_TIME; i++)
    {
        // 检查在线状态
        up2pa_send_online(dev0, dev1);
        delay_ms(500);

        ret = up2pa_check_online(dev0, dev1);
        delay_ms(500);
        log_level(U_LOG_DEBUG,"dev online state %d", ret);

        if(ret == -1)
        {
            status = ULINK_ERR_SERVER_OFFLINE;
            continue;
        }
        else
        if(ret == 0)
        {
            status = ULINK_ERR_DEV_OFFLINE;
            continue;
        }

        // 获取token
        // 令牌获取成功才能进行加密通讯
        up2pa_send_data_cmd(dev0, dev1, 0, 0, 0, CMD_UPDATE_TOKEN, NULL, 0);
        ret = up2pa_wait_data_cmd(dev0, dev1, 0, 0, 0, CMD_UPDATE_TOKEN_ACK,(char*)&stk, sizeof(stk));
        debug_log("dev token = %lx", ret, stk.token);
        token = stk.token;
        
        if(ret == -1)
        {
            status = ULINK_ERR_INIT_TOKEN;
            continue;
        }

        break;
    }
    if(i >= 20)
    {
        goto _err;
    }

#if defined(__ANDROID__) || defined(TARGET_OS_IPHONE)
#if 0
    ret = StopSmartConnection();
    if (ret != 0)
    {
        log_level(U_LOG_ERROR,"StopSmartConnection error.\n");
    }
#endif // if 0
#endif  //__ANDROID__

    // 联网之后，配置设备的host.一方面更新设备的服务器。
    // 更重要的是是的 app 连接的服务器和设备端的一致。
    status = ULINK_ERR_CONFIG_HOST;
    for(i = 0; i < 5; i++)
    {
        // 设置服务器地址
        strcpy(cfgh.name, server_name);
        cfgh.port = htons(UP2PS_PORT);
        log_level(U_LOG_INFO,"update dev server to %s",server_name);
        up2pa_send_data_cmd(dev0, dev1, 0, 0, token, CMD_CONFIG_HOST, &cfgh, sizeof(UP2P_CONFIG_HOST));

        // 等待ACK
        ret = up2pa_wait_data_cmd(dev0, dev1, 0, 0, token, CMD_CONFIG_HOST_ACK, NULL, 0);
        if(ret < 0)
            log_level(U_LOG_DEBUG, "update dev server failt !!");
            
        if(ret == 0)
        {
            break;
        }
    }
    if(i >= 5)
    {
        goto _err;
    }

    status = ULINK_ERR_INIT_KEY;
#if 0
    for(i = 0; i < 5; i++)
    {
        UP2P_SET_KEY key;
        u32 recv_cmd;

        key.key0 = key0;
        key.key1 = key1;
        
        // 发送key
        ret = up2pa_send_data_cmd(dev0, dev1, 0, 0, token, CMD_SEND_KEY, (const char *)&key, sizeof(UP2P_SET_KEY));
        
        if(ret <= 0)
            log_level(U_LOG_INFO, "unable setting dev aes key");
        else
            log_level(U_LOG_INFO, "setting dev aes key to %lx %lx",key0,key1);
        
        // 等待ACK
        ret = up2pa_wait_data_cmd(dev0, dev1, key0, key1, token, CMD_SEND_KEY_OK, NULL, 0);
        
        if(ret < 0)
            log_level(U_LOG_INFO,"setting dev aes key failt !!");
        else
            log_level(U_LOG_INFO,"setting dev aes key successfully!!");
        
        if(ret >= 0)
        {
            status = ULINK_ERR_NONE;
            break;
        }

        // todo whty double.
        // 发送key
        up2pa_send_data_cmd(dev0, dev1, key0, key1, token, CMD_SEND_KEY, (const char *)&key, sizeof(UP2P_SET_KEY));
        // 等待ACK
        ret = up2pa_wait_data_cmd(dev0, dev1, key0, key1, token, CMD_SEND_KEY_OK, NULL, 0);
        if(ret >= 0)
        {
            status = ULINK_ERR_NONE;
            break;
        }
    }
#endif
    sprintf(outkey, "%08X%08X", key0, key1);

_err:
#if defined(__ANDROID__) || defined(TARGET_OS_IPHONE)

#if 0
    ret = StopSmartConnection();
    if (ret != 0)
    {
        log_level(U_LOG_ERROR,"StopSmartConnection error.\n");
    }
#endif // 0

#endif //#if defined(__ANDROID__) || defined(TARGET_OS_IPHONE)
    return status;
}

/*
 * description: 连接到设备
 * argv:
 *      devid:目标设备ID 
 *      key: 密钥 (均为8字节HEX字符串)
 * return: 返回 ulink 指针.
 * detail: todo 有重复的流程，需要削减.
 *          1.刷新在线状态，并确定是设备和app 是局域网还是 wan。
 *          2.CMD_GET_HOST 获取服务器 ip 地址.
 *          3.CMD_UPDATE_TOKEN 更新会话 token.
 */
ULINK *ulink_open(const char *devid, const char *key)
{
    int i, ret, gethost;
    ULINK *ulink;
    UP2P_TOKEN stk;
    UP2P_HOST host;
    UP2P_DEVID devs;
    u32 cmd;

    ulink = malloc(sizeof(ULINK));
    if(ulink == NULL){
        
        log_level(U_LOG_ERROR,"malloc fail!!");
        return NULL;
    }

    ret = ascii_to_2u32(devid, &ulink->dev0, &ulink->dev1);
    if(ret == -1){
        log_level(U_LOG_ERROR,"dev transfer fail!!");
        goto _err;
        }
    ret = ascii_to_2u32(key, &ulink->key0, &ulink->key1);
    if(ret == -1){
        log_level(U_LOG_ERROR,"key transfer fail!!");
        goto _err;
    }
    ulink->sip = 0;
    ulink->sport = 0;

    gethost = 1;
    up2pa_clear_online(ulink->dev0, ulink->dev1);
    for(i = 0; i < ULINK_OPEN_TIMEOUT * 10; i++)
    {
        int online;

        // 发送检查请求
        up2pa_send_online(ulink->dev0, ulink->dev1);
        if(gethost)
        {
            up2pa_send_idx_cmd(0, 0, 0, CMD_GET_HOST, &devs, sizeof(UP2P_DEVID));
        }
        delay_ms(100);
        // 检测在线状态
        online = up2pa_check_online(ulink->dev0, ulink->dev1);
        if(online == 2)
        {
            // 局域网模式
            log_level(U_LOG_INFO,"dev and app int the same Lan\n");
            break;
        }
        else
           log_level(U_LOG_INFO,"dev and app in diff net,switch to wan mode");

#if 1  // todo remove 多服务器  集群必须去掉
        if(online == 1){ // 456258
          log_level(U_LOG_INFO,"up to wan mode !!\n");
          break;
        }
#endif

        if(gethost)
        {
            ret = up2pa_recv_idx_cmd(0, 0, 0, &cmd, &host, sizeof(UP2P_HOST));
            if(ret == sizeof(UP2P_HOST) && cmd == CMD_GET_HOST_ACK)
            {
                // 更新服务器参数
                ulink->sip = host.ip;
                ulink->sport = host.port;
                
                log_level(U_LOG_INFO,">> get server ip port %d \n",htons(ulink->sport) );
                // 设置服务器
                up2pa_set_host(ulink->sip, ulink->sport);
                gethost = 0;
                if(online == 1)
                {
                    log_level(U_LOG_INFO,"switch to wan mode\n");
                    break;
                }
            }
        }
    }

    if(i == ULINK_OPEN_TIMEOUT * 10)
    {
        log_level(U_LOG_WARN,"link dev timeout\n");
        goto _err;
    }

    // 每次一组通讯之前,要先刷新令牌,此处的作用是获取到令牌
    up2pa_send_data_cmd(ulink->dev0, ulink->dev1, ulink->key0, ulink->key1, 0, CMD_UPDATE_TOKEN, NULL, 0);
    ret = up2pa_wait_data_cmd(ulink->dev0, ulink->dev1, ulink->key0, ulink->key1, 0, CMD_UPDATE_TOKEN_ACK, &stk, sizeof(stk));
    log_level(U_LOG_INFO,"dev token = %lx", stk.token);
    if(ret == -1)
    {
        goto _err;
    }

    ulink->token = stk.token;

    return ulink;
_err:
    free(ulink);
    return NULL;
}

/*
 * description: 关闭连接,释放该链接的资源
 * argv:
 *      ulink: ulink指针
 */
int ulink_close(ULINK *ulink)
{
    if(ulink == NULL)
        return -1;

    free(ulink);

    return 0;
}

/*
 * 发送命令
 * ulink ulink指针
 * cmd 命令字
 * param 附加数据
 * len 附加数据长度
 * 返回已发送字节数 -1 失败
 */
int ulink_cmd_send(ULINK *ulink, u32 cmd, void *param, int len)
{
    int ret;

    if(ulink == NULL)
        return -1;

    // 设置服务器
    //up2pa_set_host(ulink->sip, ulink->sport);
    log_level(U_LOG_INFO, "send %lx  payload %s to dev",cmd,param);
    log_level(U_LOG_INFO, "Target dev id is %lx %lx ,session toke %lx ,session key %lx %lx",\
        ulink->dev0, ulink->dev1,ulink->token,ulink->key0, ulink->key1);
    ret = up2pa_send_data_cmd(ulink->dev0, ulink->dev1, ulink->key0, ulink->key1, ulink->token,
        cmd, param, len);

    return ret;
}

/*
 * description: 等待命令
 * argv: 
 *      ulink ulink指针
 *      cmd 命令字
 *      param 附加数据缓冲区
 *      maxlen 缓冲区长度
 * return: 返回收到的附加数据长度 -1 失败
 */
int ulink_cmd_wait(ULINK *ulink, u32 cmd, void *param, int maxlen)
{
    int ret;

    if(ulink == NULL)
        return -1;

    ret = up2pa_wait_data_cmd(ulink->dev0, ulink->dev1, ulink->key0, ulink->key1, ulink->token, 
        cmd, param, maxlen);

    return ret;
}
/*
 * description: 阻塞等待数据包，最多阻塞 MAX_WAIT_TIME.没有重发动作.
 * argv: 
 *      ulink ulink指针
 *      cmd 命令字
 *      param 附加数据缓冲区
 *      maxlen 缓冲区长度
 * return: 返回收到的附加数据长度 -1 失败
 */
int ulink_cmd_wait_noRetry(ULINK *ulink, u32 cmd, void *param, int maxlen)
{
    int ret;

    if(ulink == NULL)
        return -1;

    ret = up2pa_wait_data_cmd_noRetry(ulink->dev0, ulink->dev1, ulink->key0, ulink->key1, ulink->token, 
        cmd, param, maxlen);

    return ret;
}

/*
 *decriptions: 发送获取设备的在线状态请求,接收线程收到回包后，会自动更新设备状态到本地
 *  argv:
 *        ulink: 当前回话的结构指针.
 * return: -1 为失败，否则成功发送.
 *notice: 应用应该先调用 ulink_check_online 查询本地记录的设备状态，再调用该接口更新这边状态.
 */
int ulink_get_devState(ULINK *ulink)
{
    if(ulink == NULL)
        return -1;
    
    return up2pa_send_online(ulink->dev0, ulink->dev1);
}

/*
 * description：查询本地记录的设备状态.
 * return: 0 不在线 1 广域网在线 2 局域网在线 -1 服务器无响应
 */
int ulink_check_online(ULINK *ulink)
{
    if(ulink == NULL)
        return -1;

    return up2pa_check_online(ulink->dev0, ulink->dev1);
}

/*
 * description: 边界保护开始, 在多线程中通讯序列使用此函数作为开始。
 * todo: 通过标志位，非原子操作。
 */
void ulink_border_begin()
{
    up2pa_border_begin();
}

/*
 * 边界保护结束
 * 在多线程中通讯序列使用此函数作为结束
 */
void ulink_border_end()
{
    up2pa_border_end();
}

/*
 * 调试信息输出, 0为关闭, 大于0为输出
 */
int ulink_debug(int level)
{
    up2pa_debug_level = level;
    return 0;
}





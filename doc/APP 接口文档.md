#ulink库 接口文档
##简要
ulink 是基于 udp 实现对 wifi 模块通讯以及控制。使用wifi 模块的设备在此统一称为设备。
>1.建立会话：调用 *ulink_config* 配置设备的秘钥，并获取设备会话的token.接着调用 *ulink_open* 获取当前会话(*ulink_open* 会在后续设备共享中用到)，网络环境差或者设备根本不存在，这两个接口会最多会阻塞30s。*ulink*数据结构见数据说明。
>数据的发送和接收:成功获取 *ulink* 时，可以调用 *ulink_cmd_send* 发送指令，支持的指令在 *ulink.h*中。调用*ulink_cmd_wait* 阻塞等待对应包的回应，最多阻塞*10s*，因为 *ulink_cmd_wait* 在等待时重发上一个包，所以*ulink_cmd_wait*应该紧跟着*ulink_cmd_send*.

##数据说明：
ulink 为描述会话对象,局域网下 ulink 直接同设备通讯，app 和设备不在一个域内则同 server 通讯.

```c
struct ULINK{
    u32 dev0; // 目标设备的 dev
    u32 dev1;
    u32 key0; // 当前会aes 的秘钥
    u32 key1; // 
    u32 token; // 当前回话的 token
    u32 sip;   // 会话的源 ip.
    u16 sport;
};
```
##库接口说明：
```c
/*
 * description : 配置设备
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
int ulink_config(const char *devid, const char *ssid, const char *key, int type, char *outkey);

```

```c
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
ULINK *ulink_open(const char *devid, const char *key);
```

```cpp
/*
 * description: 关闭连接,释放该链接的资源
 * argv:
 *      ulink: ulink指针
 */
int ulink_close(ULINK *ulink);
```
```cpp
/*
 * description: 发送命令和数据
 *  argv:
 *      ulink ulink指针
 *      cmd 命令字
 *      param 附加数据
 *      len 附加数据长度
 *      返回已发送字节数，-1为失败
 */
int ulink_cmd_send(ULINK *ulink, u32 cmd, void *param, int len);
```
```cpp
/*
 * description: 阻塞等待数据包，最多阻塞 MAX_WAIT_TIME.
 * argv: 
 *      ulink ulink指针
 *      cmd 命令字
 *      param 附加数据缓冲区
 *      maxlen 缓冲区长度
 * return: 返回收到的附加数据长度 -1 失败
 */
int ulink_cmd_wait(ULINK *ulink, u32 cmd, void *param, int maxlen);
```

```cpp
/*
 * description: 阻塞等待数据包，最多阻塞 MAX_WAIT_TIME.没有重发动作.
 * argv: 
 *      ulink ulink指针
 *      cmd 命令字
 *      param 附加数据缓冲区
 *      maxlen 缓冲区长度
 * return: 返回收到的附加数据长度 -1 失败
 */
int ulink_cmd_wait_noRetry(ULINK *ulink, u32 cmd, void *param, int maxlen);

```



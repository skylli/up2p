#ulink�� �ӿ��ĵ�
##��Ҫ
ulink �ǻ��� udp ʵ�ֶ� wifi ģ��ͨѶ�Լ����ơ�ʹ��wifi ģ����豸�ڴ�ͳһ��Ϊ�豸��
>1.�����Ự������ *ulink_config* �����豸����Կ������ȡ�豸�Ự��token.���ŵ��� *ulink_open* ��ȡ��ǰ�Ự(*ulink_open* ���ں����豸�������õ�)�����绷��������豸���������ڣ��������ӿڻ���������30s��*ulink*���ݽṹ������˵����
>���ݵķ��ͺͽ���:�ɹ���ȡ *ulink* ʱ�����Ե��� *ulink_cmd_send* ����ָ�֧�ֵ�ָ���� *ulink.h*�С�����*ulink_cmd_wait* �����ȴ���Ӧ���Ļ�Ӧ���������*10s*����Ϊ *ulink_cmd_wait* �ڵȴ�ʱ�ط���һ����������*ulink_cmd_wait*Ӧ�ý�����*ulink_cmd_send*.

##����˵����
ulink Ϊ�����Ự����,�������� ulink ֱ��ͬ�豸ͨѶ��app ���豸����һ��������ͬ server ͨѶ.

```c
struct ULINK{
    u32 dev0; // Ŀ���豸�� dev
    u32 dev1;
    u32 key0; // ��ǰ��aes ����Կ
    u32 key1; // 
    u32 token; // ��ǰ�ػ��� token
    u32 sip;   // �Ự��Դ ip.
    u16 sport;
};
```
##��ӿ�˵����
```c
/*
 * description : �����豸
 * arg��
 *      devid��16�ֽڵ��豸ID�ַ���,ע��һ���� mac ��ַ
 *      ssid����Ҫ���ӵ��Ľ����SSID
 *      key����������Կ
 *      type����������
 * return�� ULINK_ERR ���ҽ�������ULINK_ERR_NONEʱ��ʾ�ɹ�
 * outkey ������Կ�ַ���
 * detail:  todo ��Ҫ�ֲ�
 *          1.smartconfig
 *          2.CMD_GET_HOST ��ȡ��������ip.
 *          3.CMD_CHECK_ONLINE ����豸�Ƿ�����.
 *          4.CMD_UPDATE_TOKEN ��ȡ��ǰ�ػ���token.
 *          5.CMD_CONFIG_HOST �����豸��host.
 *          6.CMD_SEND_KEY �����豸 aes ���ܵ�key.
 */
int ulink_config(const char *devid, const char *ssid, const char *key, int type, char *outkey);

```

```c
/*
 * description: ���ӵ��豸
 * argv:
 *      devid:Ŀ���豸ID 
 *      key: ��Կ (��Ϊ8�ֽ�HEX�ַ���)
 * return: ���� ulink ָ��.
 * detail: todo ���ظ������̣���Ҫ����.
 *          1.ˢ������״̬����ȷ�����豸��app �Ǿ��������� wan��
 *          2.CMD_GET_HOST ��ȡ������ ip ��ַ.
 *          3.CMD_UPDATE_TOKEN ���»Ự token.
 */
ULINK *ulink_open(const char *devid, const char *key);
```

```cpp
/*
 * description: �ر�����,�ͷŸ����ӵ���Դ
 * argv:
 *      ulink: ulinkָ��
 */
int ulink_close(ULINK *ulink);
```
```cpp
/*
 * description: �������������
 *  argv:
 *      ulink ulinkָ��
 *      cmd ������
 *      param ��������
 *      len �������ݳ���
 *      �����ѷ����ֽ�����-1Ϊʧ��
 */
int ulink_cmd_send(ULINK *ulink, u32 cmd, void *param, int len);
```
```cpp
/*
 * description: �����ȴ����ݰ���������� MAX_WAIT_TIME.
 * argv: 
 *      ulink ulinkָ��
 *      cmd ������
 *      param �������ݻ�����
 *      maxlen ����������
 * return: �����յ��ĸ������ݳ��� -1 ʧ��
 */
int ulink_cmd_wait(ULINK *ulink, u32 cmd, void *param, int maxlen);
```

```cpp
/*
 * description: �����ȴ����ݰ���������� MAX_WAIT_TIME.û���ط�����.
 * argv: 
 *      ulink ulinkָ��
 *      cmd ������
 *      param �������ݻ�����
 *      maxlen ����������
 * return: �����յ��ĸ������ݳ��� -1 ʧ��
 */
int ulink_cmd_wait_noRetry(ULINK *ulink, u32 cmd, void *param, int maxlen);

```



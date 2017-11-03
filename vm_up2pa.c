/*******************************************************************************

    This file is part of the project.
    Copyright wilddog.com
    All right reserved.

    File:    ulink_test.c

    No description

    TIME LIST:
    CREATE  skyli   2014-09-01 20:08:23

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "up2pa.h"
#include "ulink.h"
#include "util.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef __ANDROID__
#include <android/log.h>
#define LOG    "skyjni" // 这个是自定义的LOG的标识
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG,__VA_ARGS__) // 定义LOGI类型
#else
#define LOGI debug_log
#endif
static char recvbuf[MAX_NET_PACKAGE_SIZE];
static char sendbuf[MAX_NET_PACKAGE_SIZE];

extern int up2pa_send_online(u32 dev0, u32 dev1);
extern int up2pa_check_online(u32 dev0, u32 dev1);

static int _file_int(ULINK *ulink,const char *file_path){
    int ret  = 0,fd = -1;
    ulink_cmd_send(ulink, CMD_FILE_NAME, NULL, 0);
    ret = ulink_cmd_wait(ulink, CMD_FILE_NAME_ACK, recvbuf, MAX_SERIAL_SIZE);
    if(ret >= 0)
        fd = open(file_path, O_RDONLY);
    if(fd ==-1){
        perror("can't open /tmp/ctrl.c");
        log_level(U_LOG_ERROR, ">>>>>>> reading file failt !! ");
        return -1;
    }
    return fd;
}
static int _file_transmit(ULINK *ulink,int fd){

    u8 rbuf[MAX_NET_PACKAGE_SIZE];
    umemset(rbuf,0,MAX_NET_PACKAGE_SIZE);
    FILE_T *psend = (FILE_T*)rbuf;
    off_t filepositon = 0;
    int ret = 0;
    psend->type = FILE_CMD_TXT;
    psend->index = 1;

    psend->len = read(fd,psend->payload,MAX_NET_PACKAGE_SIZE-100);
    filepositon += psend->len;

    log_level(U_LOG_INFO, "reading file len %d",psend->len);
    log_level(U_LOG_INFO,"reading text : %s",psend->payload);
    
    printf("\n transmiting file ../vm_up2pa.c \t");
    while(1){
        int sendlen = sizeof(FILE_T) + psend->len;
        ulink_cmd_send(ulink, CMD_FILE_TXT, psend, sendlen);
        umemset(recvbuf,0,MAX_SERIAL_SIZE);
        ret = ulink_cmd_wait(ulink, CMD_FILE_TXT_ACK, recvbuf, MAX_SERIAL_SIZE);
        if(ret <  sizeof(FILE_T)) {
            w_debug(" ack error %d",ret);
        }
        FILE_T *pack = (FILE_T*)recvbuf;
        if(pack->type == FILE_CMD_CAN){
            log_level(U_LOG_WARN, " >>> transmit was cansole .");
            return -1;
        }else if(pack->type == FILE_CMD_ACK){
            umemset(psend->payload ,0,(MAX_NET_PACKAGE_SIZE-4));
            psend->type = FILE_CMD_TXT;
            psend->index++;

            lseek(fd,filepositon,SEEK_SET);
            psend->len = read(fd,psend->payload,MAX_NET_PACKAGE_SIZE-100);
            
            filepositon += psend->len;
            printf("+");
            //log_level(U_LOG_INFO,"index : %d", psend->index);
            //log_level(U_LOG_INFO,"file : %s",psend->payload);
            
            if(psend->len == 0 ){
                log_level(U_LOG_WARN,"\t\n read end !!");
                psend->type = FILE_CMD_END;
                ulink_cmd_send(ulink, CMD_FILE_TXT, psend, sizeof(FILE_T));
                umemset(recvbuf,0,MAX_SERIAL_SIZE);
                ret = ulink_cmd_wait(ulink, CMD_FILE_TXT_ACK, recvbuf, MAX_SERIAL_SIZE);
                return 0;
            }
            else if(psend->len < 0){
                    log_level(U_LOG_WARN, ">> read file error ");
                    return  -1;
            }
        }
    }
    return -1;
}
/*
*   1. 初始化 sdk
*   2. 建立session
*   3. 传输命令
*
*/
void ulink_test(const char* host,const char *dev)
{
    int ret;
    ULINK *ulink;
    char outkey[32],strcmd[512];
    // 设置 sdk log 输出等级
    logLevel_set(U_LOG_WARN);
    // 初始化 sdk 
    ulink_init(host, "0102030405060708",9529);    
    // 建立 session .
    ulink = ulink_open(dev, "0000000000000000");

    if(ulink == NULL)
    {
        log_level(U_LOG_ERROR,"ulink_open faile");
        delay_ms(1000);
        goto _err;
    }

    ulink_border_begin();

    // 在线检查
    ulink_get_devState(ulink);
    delay_ms(500);
    
    ret = ulink_check_online(ulink);
    log_level(U_LOG_INFO,"dev state %d", ret);
    delay_ms(500);

    //仅仅传输文件
    int fd  = _file_int(ulink,"../vm_up2pa.c");
    if(fd != -1)
        _file_transmit(ulink, fd);
    // 传输键盘输入的字符串.
    while(1)
    {
       
       printf("\n input string :\t");
       scanf("%s", strcmd);
       ret = ulink_cmd_send(ulink, CMD_SEND_SERIAL, strcmd, strlen(strcmd));
       ret = ulink_cmd_wait(ulink, CMD_SEND_SERIAL_ACK, recvbuf, MAX_SERIAL_SIZE);
       umemset(strcmd,strcmd, strlen(strcmd));
       delay_ms(1000);
    }
        ulink_border_end();
        ulink_close(ulink);
_err:
    ulink_deinit();
}

int main(int argc, const char *argv[])
{
    char key[32];
    ULINK *ulink;
    int ret;

    if(argc != 3)
    {
        printf("input server host and dev id\n");
        return -1;
    }
    
    ulink_test(argv[1],argv[2]);

}



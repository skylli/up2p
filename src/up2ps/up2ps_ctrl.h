/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.cn.
    All right reserved.

    File:    up2ps_ctrl.h

    No description

    TIME LIST:
    CREATE  skyli  2017-05-06 10:32:08

*******************************************************************************/
#ifndef _up2ps_ctrl_h_
#define _up2ps_ctrl_h_

#ifdef		__cplusplus
extern "C" {
#endif

#define CTRL_MAGIC 0x78653985
#define MAX_USERS 256
#define SERVER_PORT 62105

typedef enum{
	CTRL_CMD_LIST,
	CTRL_CMD_MAX
} CTRL_CMD;

typedef struct{
	u32 magic;
	char username[32];
	char password[32];
	u32 cmd;
	u32 len;
} CTRL_LOGIN;

typedef struct{
	u32 dev0;
	u32 dev1;
	u32 key0;
	u32 key1;
	u32 ip;
	u32 time;
} CLIENT_INFO;

typedef struct{
	u32 magic;
	u32 count;
} CLIENT_INFO_HEAD;

#ifdef __cplusplus
}
#endif

#endif

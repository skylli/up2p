/*******************************************************************************

    This file is part of the debug.c.
    Copyright wilddog.com
    All right reserved.

    File:    debug.c

    No description

    TIME LIST:
    CREATE  skyli   2017-05-06 13:47:55

*******************************************************************************/
#ifndef _util_h_

#define _util_h_
#include "up2p.h"


#define __LOGFILENAME__ (strrchr(__FILE__, '/')? strrchr(__FILE__, '/') + 1 : __FILE__) 

extern int g_log_level;


void logLevel_set(int level);
void log_detail_set(int detail);

int debug_log(const char *fmt, ...);


int ascii_to_2u32(const char *devid, u32 *dev0, u32 *dev1);
int up2p_log_close(void);

int up2p_log(const char *tag,const char *file,const char* func,int line,const char *fmt, ...);
#define log_level(level, format,...) do{\
       if(level >= g_log_level){\
            up2p_log(s_debug[level],__LOGFILENAME__, __FUNCTION__, __LINE__,format,##__VA_ARGS__); \
        }\
}while(0)

#define log_debug(format,...) log_level(U_LOG_DEBUG,format,##__VA_ARGS__)

#define str_equal(ps1,ps2) (( strlen(ps1) == strlen(ps2) && strcmp(ps1,ps2) == 0)?1:0)

#endif

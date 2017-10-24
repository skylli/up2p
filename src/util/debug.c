/*******************************************************************************

    This file is part of the debug.c.
    Copyright wilddog.com
    All right reserved.

    File:    debug.c

    No description

    TIME LIST:
    CREATE  skyli   2017-05-06 13:47:55

*******************************************************************************/
#include "up2p.h"

FILE *debug_log_fp = NULL;
int g_log_level = U_LOG_INFO;
u8 g_log_detail =0;


void logLevel_set(int level)
{
    g_log_level = level;
}
void log_detail_set(int detail)
{
    g_log_detail = detail;
}
char *time_str()
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
	char buffer[0x1000];
	char *time_s = time_str();
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf(buffer,fmt,argptr);
	va_end(argptr);

	if(debug_log_fp == NULL)
		debug_log_fp = fopen("up2ps.log", "w");

	if(debug_log_fp){
		fputs(time_s, debug_log_fp);
		fputs(buffer, debug_log_fp);
		fflush(debug_log_fp);
	}

	up2p_printf("%s", time_s);
	up2p_printf("%s", buffer);

	return 0;
}

int up2p_log(const char *tag,const char *file,const char* func,int line,const char *fmt, ...)
{
    
	char buffer[0x1000];
	char *time_s = time_str();
    char head[512];

    if(g_log_detail)
        sprintf(head,"%s %s FUNC =%s LINE =%d: ",tag,file,func,line);
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf(buffer,fmt,argptr);
	va_end(argptr);

	if(debug_log_fp == NULL)
		debug_log_fp = fopen("up2ps.log", "w");
  if(debug_log_fp){
    fputs(time_s, debug_log_fp);
    if(g_log_detail)
          fputs(head,debug_log_fp);
    if(g_log_detail){
        fputs(buffer, debug_log_fp);
        fputs("\n",debug_log_fp);
      }
    fflush(debug_log_fp);
  }
	up2p_printf("%s", time_s);
    
    if(g_log_detail)
        up2p_printf("%s",head);
    
	up2p_printf("%s", buffer);
    up2p_printf("\n");

	return 0;
}
int up2p_log_close(void)
{
    if(debug_log_fp)
        fclose(debug_log_fp);
    debug_log_fp = NULL;
    return 0;
}

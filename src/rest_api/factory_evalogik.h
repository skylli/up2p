/*******************************************************************************
    This file is part of the up2p.
    Copyright 
    All right reserved.

    File: factory_evalogik.h

 描述： 1、工厂功能函数实现文件
 注意：
        需要手动在 rest_api.c rest_api.h 注册工厂的名称和工厂的功能函数。
     1. 在 rest_api.c 头部 添加   #include "factory_evalogik.h"  声明工厂函数
     2. 在 rest_api.c  厂家数组 _u_fac_nameTable 中添加厂家 FAC_NAME_EVALOGIK。
     3. 在 rest_api.c  厂家功能解析数组 _u_fac_funcTable 中添加该工厂的解析函数  _factory_evalogik_payload_alloc.
     
  TIME LIST:
  CREATE  skyli   2017-11-23 17:30

*******************************************************************************/
#ifndef _factory_evalogik_h_
#define _factory_evalogik_h_
#include "up2p.h"
#include "rest_api.h"

#define FAC_NAME_EVALOGIK "evalogik"

// 产品的功能定义，该字符应该同数据库中的一致，否则解析不了，功能不可用。
#define _EVALOGIK_FUNCTION_TURNON "turnOn"
#define _EVALOGIK_FUNCTION_TURNOFF "turnOff"
#define _EVALOGIK_FUNCTION_COLOR_CHANGE  "colorChange"
#define _EVALOGIK_FUNCTION_LIGHTTIME  "lightTime"
#define _EVALOGIK_FUNCTION_FLASH  "flash"



/***** 提供给上层的入口，解析并输出控制设备的 payload
***/

int _factory_evalogik_payload_alloc(Product_CtlInfo *p_ctl_info,DEV_CmdInfo *p_cmdinfo);

#endif //  end _factory_evalogik_h_

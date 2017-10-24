/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    up2ps_skill.h

    No description

    TIME LIST:
    CREATE  skyli   2014-08-13 10:57:52

*******************************************************************************/
#ifndef _up2ps_skill_h_
#define _up2ps_skill_h_

#define AMAZON_PATH	 "/amazon_interface"
#define SKILL_PATH_DISCOVER	 "/skill_discover"
#define SKILL_PATH_CTRL	 "/skill_control"

#define SKILL_CMD_INDEX "cmdindex"

typedef enum SKILL_CMD_T{
    SKILL_CMD_NON, 
    SKILL_CMD_ON,
    SKILL_CMD_OFF,
    SKILL_CMD_SETPERCEN,
    SKILL_CMD_INCPERCEN,
    SKILL_CMD_DECPERCEN,
    SKILL_CMD_SETCOLOR,

    SKILL_CMD_TREE_ON    = 0x10,   //控制开
    SKILL_CMD_TREE_OFF   = 0x11, //控制关
    SKILL_CMD_TREE_TOP_ON   = 0x12, // 控制树顶开关开
    SKILL_CMD_TREE_TOP_OFF  = 0x13, // 控制树顶开关关
    SKILL_CMD_TREE_COLOR_CHANGE  = 0x14, // 控制颜色转换
    SKILL_CMD_TREE_TWINKLE = 0x15, // 控制闪烁
    SKILL_CMD_TREE_TIMER = 0x16, //控制定时


    SKILL_CMD_MAX
}Skill_CMD_T;
static const char *skill_cmd[]={
    "none",
    "TurnOnRequest",
    "TurnOffRequest",
    "SetPercentageRequest",
    "IncrementPercentageRequest",
    "DecrementPercentageRequest",
    "SetColorRequest",
    "cmdmax"
};
static const char *skill_tree_cmd[] = {
  "TurnOn",
  "TurnOFF",
  "TurnOnTreeTop",
  "TurnOffTreeTop",
  "Twinklen",
  "ColorChange",
  "TimeSetting"
};

int skill_handle_distcoverDev_request (const struct _u_request * request, struct _u_response * response, void * user_data);
int skill_handle_controlDev_request (const struct _u_request * request, struct _u_response * response, void * user_data);

// function test 
void test_distcover_device(void);
void test_ctrl_device(void);


#endif //  end skill_h

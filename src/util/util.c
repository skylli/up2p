/*******************************************************************************

    This file is part of the debug.c.
    Copyright wilddog.com
    All right reserved.

    File:    util.c

    No description

    TIME LIST:
    CREATE  skyli   2017-05-06 13:47:55

*******************************************************************************/
#include "up2p.h"


int ascii_to_2u32(const char *devid, u32 *dev0, u32 *dev1)
{
    char tmp0[32];
    char tmp1[32];
    int i;

    if(strlen(devid) != 16)
        return -1;

    memset(tmp0, 0, sizeof(tmp0));
    memset(tmp1, 0, sizeof(tmp1));

    strncpy(tmp0, devid, 8);
    strncpy(tmp1, devid + 8, 8);

    sscanf(tmp0, "%08x", dev0);
    sscanf(tmp1, "%08x", dev1);

    return 0;
}


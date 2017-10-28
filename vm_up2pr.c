 /*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.cn.
    All right reserved.

    File:    up2ps.c

    No description

    TIME LIST:
    CREATE  skyli   2017-06-27 13:48:30

*******************************************************************************/
#include<pthread.h>
#include <arpa/inet.h>
#include <jansson.h>
#include <ulfius.h>
#include "up2p.h"
#include "util.h"
#include "config.h"
#include "rest_api.h"
#include "vlink.h"

int main(void){
    //logLevel_set(U_LOG_ALL);
   //restApi_unittest_rest_api();
   ///unittest_observer_queue();
   //unittest_protocol_test();
   unittest_link();
}


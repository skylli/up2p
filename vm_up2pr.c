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

#if 1
    //logLevel_set(U_LOG_ALL);
    unittest_rest_api();
   ///unittest_observer_queue();
   //unittest_protocol_test();
   //unittest_link();
    return 0;
#else

    struct _u_instance instance;
    
    logLevel_set(0);
      // Initialize instance with the port number
  if (ulfius_init_instance(&instance, HTTPS_REST_PORT, NULL, NULL) != U_OK) {
    log_level(U_LOG_ERROR, "Error ulfius_init_instance, abort\n");
    return (-1);
  }
    // oauth2  config 
    struct _glewlwyd_resource_config g_config;
    
    g_config.method = G_METHOD_URL;
    g_config.oauth_scope = "scope1";
    g_config.jwt_decode_key = "secret";
    g_config.jwt_alg = JWT_ALG_HS512;
    g_config.realm = "example";

    restApi_init(REST_MAC_CONCURRENT);
    
    // Endpoint list declaration
    //ulfius_add_endpoint_by_val(&instance, "POST", SKILL_PATH_DISCOVER, NULL, 0, &skill_handle_distcoverDev_request, NULL);
    ulfius_add_endpoint_by_val(&instance, "POST", "/rest/control", NULL, 0,&restApi_handle_dev_type_value_request, NULL);
    log_level(U_LOG_DEBUG,"bind url path end.");

    // Start the framework
    if (ulfius_start_framework(&instance) == U_OK) {
        log_level(U_LOG_DEBUG,"Start framework on port %d\n", instance.port);
     // Wait for the user to press <enter> on the console to quit the application
    while(1){
        sleep(1);
        }
    } else {
        log_level(U_LOG_DEBUG, "Error starting framework\n");
    }
    
    rest_ctl.recv_pthread_runing = 0;
    sleep(3);
    
    log_level(U_LOG_DEBUG, "ending ....\n");
    restApi_clean();
    
    log_level(U_LOG_INFO, "test end\n");
    // cmd
    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);
    
#endif
}


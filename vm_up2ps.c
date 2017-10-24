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

#include "config.h"
#include "up2ps.h"
#include "up2ps_skill.h"
#include "up2p_oauth2.h"

#include "apc_color_conversions.h"

#if 1

/**
 * main function
 */
int main(void) {

    struct _u_instance instance;
#if 0

    json_t *j_query = json_pack("{sss[ssss]}",
                    "table",
                    "GLEWLWYD_TABLE_RESOURCE",
                    "columns",
                      "gr_id",
                      "gr_name AS name",
                      "gr_description AS description",
                      "gr_uri AS uri");

    char *p = json_dumps(j_query,JSON_COMPACT);
      if(p){
          log_level(U_LOG_INFO,"dev is : %s\n",p);
          free(p);
          p = NULL;
      }

    json_decref(j_query);
   return 0;
#endif

#if 0
 // test_distcover_device();
 // test_ctrl_device();
    char text[] = "{\"payload\":{\"hue\":240.14514,\"saturation\":1.00,\"brightness\":3.142354646}}";
    json_error_t error;
    json_t *root = NULL;
    HsvColor hsv;
    root = json_loads(text, 0, &error);
    if(!root){
        fprintf(stderr, "error : root\n");
        fprintf(stderr, "error : on line %d: %s\n", error.line, error.text);
        exit(1);
    }
    _resquest_getHsb(root,&hsv);
    json_decref(root);
    up2p_log_close();
  return 0;
#endif

    
   pthread_t relay_thread;
   
   // up2ps server init 
   // log setting 
   
   logLevel_set(U_LOG_ALL);
   log_detail_set(1);
  // set up relay server .
   pthread_create(&relay_thread,NULL,up2ps_relay_start,NULL);
  

  // Initialize instance with the port number
  if (ulfius_init_instance(&instance, HTTPSERVER_PORT, NULL, NULL) != U_OK) {
    fprintf(stderr, "Error ulfius_init_instance, abort\n");
    return(1);
  }
  // oauth2  config 
    struct _glewlwyd_resource_config g_config;
    g_config.method = G_METHOD_URL;
    g_config.oauth_scope = "scope1";
    g_config.jwt_decode_key = "secret";
    g_config.jwt_alg = JWT_ALG_HS512;
    g_config.realm = "example";

  // Endpoint list declaration
  ulfius_add_endpoint_by_val(&instance, "POST", SKILL_PATH_DISCOVER, NULL, 0, &skill_handle_distcoverDev_request, NULL);
  ulfius_add_endpoint_by_val(&instance, "POST", SKILL_PATH_CTRL, NULL, 0, &skill_handle_controlDev_request, NULL);
  // First add an endpoint with the authentication callback callback_check_glewlwyd_access_token
  ulfius_add_endpoint_by_val(&instance, "GET", "/api", "/resurce/:id", 0,&callback_check_glewlwyd_access_token, (void*)&g_config);
  //Second example, use callback_check_glewlwyd_access_token as a default authentication callback
  //ulfius_set_default_auth_function(instance, &callback_check_glewlwyd_access_token, (void*)g_config, NULL);

  // Start the framework
  if (ulfius_start_framework(&instance) == U_OK) {
    log_level(U_LOG_INFO,"Start framework on port %d\n", instance.port);
     // Wait for the user to press <enter> on the console to quit the application
    while(1){
        sleep(500);
    }
  } else {
        fprintf(stderr, "Error starting framework\n");
  }
   log_level(U_LOG_INFO,"End framework\n");

// cmd
  ulfius_stop_framework(&instance);
  ulfius_clean_instance(&instance);
  
  pthread_join(relay_thread,NULL);

  return 0;
}
#endif

#if 0
int main(int argc, const char *argv[])
{
	struct sockaddr_in sin;
    pthread_t relay_thread;
    UP2P_IO io;
    char rbuf[MAX_NET_PACKAGE_SIZE],sbuf[MAX_NET_PACKAGE_SIZE];
    
    int ret = 0,slen,rlen = 0;
    // set log level 
    logLevel_set(U_LOG_ALL);
    log_detail_set(1);
   // set up relay server .
    pthread_create(&relay_thread,NULL,up2ps_relay_start,NULL);

    while(1){
        // init
        memset(rbuf,0,MAX_NET_PACKAGE_SIZE);
        memset(sbuf,0,MAX_NET_PACKAGE_SIZE);
        rlen = MAX_NET_PACKAGE_SIZE;

        // get input
        log_level(U_LOG_DEBUG,"/n>>> input uart data : ");
        if (gets(sbuf) != NULL)
        {
            slen = strlen(sbuf);
            sbuf[slen] = '\n';
            slen +=1;
            log_level(U_LOG_INFO,"str = %s", sbuf);
            ret = s_cmd_send(CMD_TXRX_SERIAL,CMD_TXRX_SERIAL_ACK,"0000000C4326605A","0000000000000000",sbuf,slen,rbuf,&rlen);
            if(rlen)
                log_level(U_LOG_INFO,"Uart receive %s/n",rbuf);
        }
    }

    pthread_join(relay_thread,NULL);
	return 0;
}
#endif

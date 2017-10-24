/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.ulink.demo;//
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import com.ulink.UlinkNative;
import com.ulink.UlinkGPIO;

import android.net.wifi.ScanResult;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;


/*
 * Simple Java UI to trigger jni function. It is exactly same as Java code
 * in hello-jni.
 */
public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        TextView tv = new TextView(this);
        //WifiManager mWifiManager = (WifiManager) getSystemService (Context.WIFI_SERVICE); 
        //WifiInfo info = mWifiManager.getConnectionInfo();
        //String mac=info.getMacAddress();
        //String appId="0000"+mac.replace(":", "");
       // MLog.d("appid:"+ "1122334455667788");
        short sport  = 9529;
        UlinkNative.ulinkInit("192.168.2.229", "1122334455667788",sport);
        int link = UlinkNative.ulinkOpen("0000000C4326605A","0000000000000000");
        if( link == 0){
            tv.setText( "open device failt");
            
        }else{
            byte recvbuf[] = new byte[32];
		    UlinkGPIO ulinkgpio = new UlinkGPIO(); 
		    ulinkgpio.pin=1;
		    ulinkgpio.mode=0;
		    UlinkGPIO reculinkgpio = new UlinkGPIO();

            UlinkNative.ulinkBorderBegin();
            int ret = UlinkNative.ulinkDevOnlineRequestSend(link);
            // 发送 gpio 控制指令CMD_GPIO_INIT io 初始化
            UlinkNative.ulinkSendGPIOCmd(link,UlinkNative.CMD_GPIO_INIT,ulinkgpio);
            ret=UlinkNative.ulinkWaitCmd(link, UlinkNative.CMD_GPIO_INIT_ACK, recvbuf, 32);
            //  串口指令 CMD_SEND_SERIAL  写串口
            UlinkNative.ulinkSendCmd(link,UlinkNative.CMD_SEND_SERIAL,"test string",11);
            ret = UlinkNative.ulinkWaitCmd(link,UlinkNative.CMD_SEND_SERIAL_ACK,recvbuf,32);
            
            // UlinkNative.ulinkSendGPIOCmd(link,UlinkNative.CMD_GPIO_READ,ulinkgpio);
            // ret=UlinkNative.ulinkWaitGPIOCmd(link, UlinkNative.CMD_GPIO_READ_ACK, reculinkgpio);
            UlinkNative.ulinkBorderEnd();
        }
        setContentView(tv);
    }
  
}

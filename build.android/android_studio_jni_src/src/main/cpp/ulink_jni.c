/*******************************************************************************

    This file is part of the project.
    Copyright wilddog.com
    All right reserved.

    File:    ulink_jni.c

    No description

    TIME LIST:
    CREATE  skyli   2017-09-09 09:26:57

*******************************************************************************/
#include <jni.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <android/log.h>

#include "up2pa.h"
#include "ulink.h"
#include "util.h"
#define TEST_HOST "192.168.2.223"
#define TEST_APPID "1122334455667788"

#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, "hell-libs::", __VA_ARGS__))

jint  JNICALL Java_com_ulink_UlinkNative_ulinkInit(JNIEnv *pjEnv, jclass clazz,
		jstring host, jstring app_id,jshort sport) {
	char* devid = (*pjEnv)->GetStringUTFChars(pjEnv, host, NULL);
	char* appid = (*pjEnv)->GetStringUTFChars(pjEnv, app_id, NULL);
	unsigned int port=  (unsigned int)sport;
	debug_log("hell-libs:: in Java_com_ulink_UlinkNative_ulinkInit");
	debug_log("hell-libs:: %s, app id %s",host, app_id);
	return ulink_init(devid, appid,port);
 }

jint JNICALL Java_com_ulink_UlinkNative_ulinkOpen(JNIEnv *pjEnv, jclass clazz,
		jstring dev_id, jstring u_key) {
	char* devid = (*pjEnv)->GetStringUTFChars(pjEnv, dev_id, NULL);
	char* ukey = (*pjEnv)->GetStringUTFChars(pjEnv, u_key, NULL);
	struct ULINK * ulink = (struct ULINK *) ulink_open(devid, ukey);
	return (int) ulink;
}

jint JNICALL Java_com_ulink_UlinkNative_ulinkClose(JNIEnv *pjEnv, jclass clazz,
		jint link) {
	return ulink_close((int *) link);
}

jint Java_com_ulink_UlinkNative_ulinkConfig(JNIEnv *pjEnv, jclass clazz,
		jstring dev_id, jstring ss_id, jstring pass_wd, jint type,
		jstring keystr) {
	char* devid = (*pjEnv)->GetStringUTFChars(pjEnv, dev_id, NULL);
	char* ssid = (*pjEnv)->GetStringUTFChars(pjEnv, ss_id, NULL);
	char* passwd = (*pjEnv)->GetStringUTFChars(pjEnv, pass_wd, NULL);
	char str_key_buf[32] = {0};
    memset(str_key_buf, 0, 32);

	int ret = ulink_config(devid, ssid, passwd, type, str_key_buf);

	jclass strClass = (*pjEnv)->FindClass(pjEnv, "java/lang/String");
	jmethodID ctorID = (*pjEnv)->GetMethodID(pjEnv, strClass, "<init>",
			"([BLjava/lang/String;)V");
	jbyteArray bytes_key = (*pjEnv)->NewByteArray(pjEnv, 16);
	(*pjEnv)->SetByteArrayRegion(pjEnv, bytes_key, 0, 16, (jbyte*) str_key_buf);
	jstring encoding = (*pjEnv)->NewStringUTF(pjEnv, "utf-8");
	jstring outstr = (jstring) (*pjEnv)->NewObject(pjEnv, strClass, ctorID, bytes_key,
			encoding);
	jmethodID ctorID2 = (*pjEnv)->GetMethodID(pjEnv, strClass, "<init>",
			"(Ljava/lang/String;)V");
	(*pjEnv)->CallVoidMethod(pjEnv, keystr, ctorID2, outstr);

	return ret;
}

jint Java_com_ulink_UlinkNative_ulinkDevOnlineRequestSend(JNIEnv *pjEnv, jclass clazz,
		jint link) {
	return ulink_get_devState((ULINK *) link);
}


jint JNICALL Java_com_ulink_UlinkNative_ulinkCheckOnline(JNIEnv *pjEnv,
		jclass clazz, jint link) {
	return ulink_check_online((ULINK *) link);
}

jint JNICALL Java_com_ulink_UlinkNative_ulinkSendCmd(JNIEnv *pjEnv,
		jclass clazz, jint link, jint cmd, jstring param, jint len) {

	char* send_buf = (*pjEnv)->GetStringUTFChars(pjEnv, param, NULL);

	return ulink_cmd_send((ULINK *) link, cmd, send_buf, len);
}


jint JNICALL Java_com_ulink_UlinkNative_ulinkSendGPIOCmd(JNIEnv *pjEnv,
		jclass clazz, jint link, jint cmd, jobject customer) {
	jclass cls_objClass = (*pjEnv)->GetObjectClass(pjEnv, customer);
	jfieldID pinId = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "pin", "I");
	jfieldID modeId = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "mode", "I");

	jfieldID valueId = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "value", "I");
	jint pin = (*pjEnv)->GetIntField(pjEnv, customer, pinId);
	jint mode = (*pjEnv)->GetIntField(pjEnv, customer, modeId);
	jint value = (*pjEnv)->GetIntField(pjEnv, customer, valueId);
	UP2P_GPIO pgpio_cust;
	pgpio_cust.pin = pin;
	pgpio_cust.mode = mode;
	pgpio_cust.value = value;
	return ulink_cmd_send((ULINK *) link, cmd, (char*) &pgpio_cust,
			sizeof(UP2P_GPIO));
			
}
jint JNICALL Java_com_ulink_UlinkNative_ulinkWaitGPIOCmd(JNIEnv *pjEnv,
		jclass clazz, jint link, jint cmd, jobject customer) {
	int ret = 0;
	jclass cls_objClass = (*pjEnv)->GetObjectClass(pjEnv, customer);
	jfieldID pinId = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "pin", "I");
	jfieldID modeId = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "mode", "I");
	jfieldID valueId = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "value", "I");
	UP2P_GPIO rec_buf;
	ret = ulink_cmd_wait((ULINK *) link, cmd, (char *) &rec_buf,
			sizeof(UP2P_GPIO));

	(*pjEnv)->SetIntField(pjEnv, customer, pinId, rec_buf.pin);
	(*pjEnv)->SetIntField(pjEnv, customer, modeId, rec_buf.mode);
	(*pjEnv)->SetIntField(pjEnv, customer, valueId, rec_buf.value);

	return ret;
}

#if 0
jint JNICALL Java_com_ulink_UlinkNative_ulinkSendPWMCmd(JNIEnv *pjEnv,
		jclass clazz, jint link, jint cmd, jobject customer) {
	jclass cls_objClass = (*pjEnv)->GetObjectClass(pjEnv, customer);
	jfieldID pinId = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "pin", "I");
	jfieldID timer_cntId = (*pjEnv)->GetFieldID(pjEnv, cls_objClass,
			"timer_cnt", "I");
	jfieldID dutyId = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "duty", "I");
	jfieldID freqId = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "freq", "I");

	jint pin = (jint) (*pjEnv)->GetIntField(pjEnv, customer, pinId);
	jint timer_cnt = (jint) (*pjEnv)->GetIntField(pjEnv, customer, timer_cntId);
	jint duty = (jint) (*pjEnv)->GetIntField(pjEnv, customer, dutyId);
	jint freq = (jint) (*pjEnv)->GetIntField(pjEnv, customer, freqId);

	pwm_cust_ ppwm_cust;
	ppwm_cust.pin = pin;
	ppwm_cust.timer_cnt = timer_cnt;
	ppwm_cust.duty = duty;
	ppwm_cust.freq = freq;
	return ulink_cmd_send((ULINK *) link, cmd, (char*) &ppwm_cust,
			sizeof(pwm_cust_));
}
jint JNICALL Java_com_ulink_UlinkNative_ulinkSendI2CCmd(JNIEnv *pjEnv,
		jclass clazz, jint link, jint cmd, jobject customer) {
	jclass cls_objClass = (*pjEnv)->GetObjectClass(pjEnv, customer);
	jfieldID clk_pin_id = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "clk_pin",
			"I");
	jfieldID sda_pin_id = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "sda_pin",
			"I");
	jfieldID speed_id = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "speed", "I");
	jfieldID address_id = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "address",
			"I");
	jfieldID w_len_id = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "w_len", "I");
	jfieldID r_len_id = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "r_len", "I");
	jfieldID date_id = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "date", "[B");

	jint clkPin = (jint) (*pjEnv)->GetIntField(pjEnv, customer, clk_pin_id);
	jint sdaPin = (jint) (*pjEnv)->GetIntField(pjEnv, customer, sda_pin_id);
	jint speed = (jint) (*pjEnv)->GetIntField(pjEnv, customer, speed_id);
	jint address = (jint) (*pjEnv)->GetIntField(pjEnv, customer, address_id);
	jint wLen = (jint) (*pjEnv)->GetIntField(pjEnv, customer, w_len_id);
	jint rLen = (jint) (*pjEnv)->GetIntField(pjEnv, customer, r_len_id);
	jbyteArray value = (jbyteArray) (*pjEnv)->GetObjectField(pjEnv, customer,
			date_id);
	jbyte *dat = (*pjEnv)->GetByteArrayElements(pjEnv, value, 0);
	I2C_cust_ iic_cust;
	iic_cust.clk_pin = clkPin;
	iic_cust.sda_pin = sdaPin;
	iic_cust.speed = speed;
	iic_cust.address = address;
	iic_cust.w_len = wLen;
	iic_cust.r_len = rLen;
	int i = 0;
	 if(CMD_I2C_WRITE==cmd)
	 {
		    for (i = 0; i < wLen; i++) {
			 iic_cust.date[i] = dat[i];
			 }
	 }else if(CMD_I2C_READ==cmd)
	 {
		 for (i = 0; i < rLen; i++) {
			iic_cust.date[i] = dat[i];
		}
	 }
	return ulink_cmd_send((ULINK *) link, cmd, (char*) &iic_cust,
			sizeof(I2C_cust_)+wLen);
}

jint JNICALL Java_com_ulink_UlinkNative_ulinkUpdateWifiUdpSend(JNIEnv *pjEnv,
		jclass clazz, jint link, jbyteArray param, jint len) {

	jbyte *update_buf = (*pjEnv)->GetByteArrayElements(pjEnv, param, 0);
	return ulink_update_vm10x((ULINK *) link, update_buf, len);
}


jint JNICALL Java_com_ulink_UlinkNative_ulinkWaitPWMCmd(JNIEnv *pjEnv,
		jclass clazz, jint link, jint cmd, jobject customer) {
	int ret = 0;
	jclass cls_objClass = (*pjEnv)->GetObjectClass(pjEnv, customer);
	jfieldID pinId = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "pin", "I");
	jfieldID timer_cntId = (*pjEnv)->GetFieldID(pjEnv, cls_objClass,
			"timer_cnt", "I");
	jfieldID dutyId = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "duty", "I");
	jfieldID freqId = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "freq", "I");

	pwm_cust_ rec_buf;
	ret = ulink_cmd_wait((ULINK *) link, cmd, (char *) &rec_buf,
			sizeof(pwm_cust_));
	(*pjEnv)->SetIntField(pjEnv, customer, pinId, rec_buf.pin);
	(*pjEnv)->SetIntField(pjEnv, customer, timer_cntId, rec_buf.timer_cnt);
	(*pjEnv)->SetIntField(pjEnv, customer, dutyId, rec_buf.duty);
	(*pjEnv)->SetIntField(pjEnv, customer, freqId, rec_buf.freq);
	return ret;
}

jint JNICALL Java_com_ulink_UlinkNative_ulinkWaitI2CCmd(JNIEnv *pjEnv,
		jclass clazz, jint link, jint cmd, jobject customer) {
	int ret = 0;
	jclass cls_objClass = (*pjEnv)->GetObjectClass(pjEnv, customer);
	jfieldID clk_pin = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "clk_pin",
			"I");
	jfieldID sda_pin = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "sda_pin",
			"I");
	jfieldID speed_ = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "speed", "I");
	jfieldID address_ = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "address",
			"I");
	jfieldID w_len = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "w_len", "I");
	jfieldID r_len = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "r_len", "I");
	jfieldID date = (*pjEnv)->GetFieldID(pjEnv, cls_objClass, "date", "I");

	I2C_cust_ rec_buf;
	ret = ulink_cmd_wait((ULINK *) link, cmd, (char *) &rec_buf,
			sizeof(I2C_cust_));
	(*pjEnv)->SetIntField(pjEnv, customer, clk_pin, rec_buf.clk_pin);
	(*pjEnv)->SetIntField(pjEnv, customer, sda_pin, rec_buf.sda_pin);
	(*pjEnv)->SetIntField(pjEnv, customer, speed_, rec_buf.speed);
	(*pjEnv)->SetIntField(pjEnv, customer, address_, rec_buf.address);
	(*pjEnv)->SetIntField(pjEnv, customer, w_len, rec_buf.w_len);
	(*pjEnv)->SetIntField(pjEnv, customer, r_len, rec_buf.r_len);
	(*pjEnv)->SetObjectField(pjEnv, customer, date, rec_buf.date);
	return ret;
}

#endif

jint JNICALL Java_com_ulink_UlinkNative_ulinkWaitCmd(JNIEnv *pjEnv,
		jclass clazz, jint link, jint cmd, jbyteArray param, int maxlen) {
	char* pBuf;
	int len;
	pBuf = (*pjEnv)->GetPrimitiveArrayCritical(pjEnv, param, 0);
	len = ulink_cmd_wait((ULINK *) link, cmd, pBuf, maxlen);
	(*pjEnv)->ReleasePrimitiveArrayCritical(pjEnv, param, pBuf, 0);
	return len;
}

void JNICALL Java_com_ulink_UlinkNative_ulinkBorderBegin(JNIEnv *pjEnv,
		jclass clazz) {
	ulink_border_begin();
}

void JNICALL Java_com_ulink_UlinkNative_ulinkBorderEnd(JNIEnv *pjEnv,
		jclass clazz) {
	ulink_border_end();
}

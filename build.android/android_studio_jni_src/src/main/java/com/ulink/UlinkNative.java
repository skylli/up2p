package com.ulink;

public class UlinkNative {
		static {
	        System.loadLibrary("ulink_jni");
	    }

	// 建立 udp socket 并初始化
	public static  native int ulinkInit(String host, String appId,short sport);


	// 广播和到服务器查询 devId 设备是否在线，并且获取会话 token
	public static  native int ulinkOpen(String devId,String key);
	// 设置设备的连接热点 ssid, 并刷新设备会话的 aes key
	public static  native int ulinkConfig(String devId, String ssid,String passwd,int type,String keystr);
	//  发送广播包或者 udp 包到服务器查询设备的状态，native 会自动接收，并把设备状态保存，供 ulinkCheckOnline 使用
	public static  native int ulinkDevOnlineRequestSend(int link);
	//  查询设备是否在线，仅仅查询内部缓存的设备状态
	public static  native int ulinkCheckOnline(int link);
	// gpio 读写命令
	public static  native int ulinkSendGPIOCmd(int link, int cmd, UlinkGPIO gpio);
//	public static  native int ulinkSendPWMCmd(int link, int cmd, UlinkPWM pwm);
//	public static  native int ulinkSendI2CCmd(int link, int cmd, UlinkIIC iic);
	// 可以发送所有的控制指令
	public static  native int ulinkSendCmd(int link, int cmd, String param, int len);


	public static  native int ulinkWaitGPIOCmd(int link, int cmd, UlinkGPIO gpio);
//	public static native int ulinkWaitPWMCmd(int link, int cmd, UlinkPWM pwm);
//	public static native int ulinkWaitI2CCmd(int link, int cmd, UlinkIIC iic);
	// 阻塞等待 cmd ，用于等待设备或者服务器回应的指令
	public static native int ulinkWaitCmd(int link, int cmd, byte[] param, int maxlen);
	   
//	public static native int ulinkUpdateWifiUdpSend(int link,byte[] param, int len);

	public static native void ulinkBorderBegin();
	public static native void ulinkBorderEnd();

	// 支持的控制指令
	public static int	CMD_SEND_SERIAL			= 0x3000;	// 发送串口数据
	public static int	CMD_SEND_SERIAL_ACK		= 0x3001;	// 发送串口数据回应
	public static int	CMD_READ_SERIAL			= 0x3010;	// 读取串口数据
	public static int	CMD_READ_SERIAL_ACK		= 0x3011;	// 读取串口数据回应
	public static int	CMD_TXRX_SERIAL			= 0x3020;	// 以收发一体的形式发送串口数据
	public static int	CMD_TXRX_SERIAL_ACK		= 0x3021;	// 返收发一体的形式回应串口数据

	public static int 	CMD_WIFISDKVER_QURY		= 0x3030;	// 询问wifi sdk 的版本信息
	public static int 	CMD_WIFISDKVER_QURY_ACK	= 0x3031;	// 询问wifi sdk 的版本信息回应
	public static int	CMD_WIFIUPDATE_TRI		= 0x3040;	// 触发更新wifiSDK命令
	public static int	CMD_WIFIUPDATE_TRI_ACK	= 0x3041;	// 回应更新
	public static int	CMD_WIFIUPDATE_DATA		= 0x3050;	// 传输wifi更新数据
	public static int	CMD_WIFIUPDATE_DATA_ACK	= 0x3051;	// 传输wifi更新数据回应

	public static int	CMD_GPIO_INIT			= 0x3060;	// 配置gpio
	public static int	CMD_GPIO_INIT_ACK		= 0x3061;	// 配置gpio 回应
	public static int	CMD_GPIO_READ			= 0x3070;	// 读gpio
	public static int	CMD_GPIO_READ_ACK		= 0x3071;	// 读gpio 回应
	public static int	CMD_GPIO_WRITE			= 0x3080;	// 写gpio
	public static int	CMD_GPIO_WRITE_ACK		= 0x3081;	// 写gpio 回应
	public static int	CMD_PWM_INIT			= 0x3090;	// 配置pwm
	public static int	CMD_PWM_INIT_ACK		= 0x3091;	// 配置pwm 回应
	public static int	CMD_PWM_READ			= 0x30A0;	// 读pwm参数

	public static int	CMD_PWM_READ_ACK		= 0x30A1;	// 读pwm参数回应
	public static int	CMD_PWM_WRITE			= 0x30B0;	// 写pwm 参数
	public static int	CMD_PWM_WRITE_ACK		= 0x30B1;	// 写pwm 参数回应
	public static int	CMD_I2C_INIT			= 0x30C0;	// 配置I2C
	public static int	CMD_I2C_INIT_ACK		= 0x30C1;	// I2C配置回应
	public static int	CMD_I2C_READ			= 0x30D0;	// 读取i2c数据
	public static int	CMD_I2C_READ_ACK		= 0x30D1;	// 读取回应
	public static int	CMD_I2C_WRITE			= 0x30E0;	// 写i2c数据
	public static int	CMD_I2C_WRITE_ACK		= 0x30E1;	// 写i2c数据回应
	public static int	CMD_I2C_TXRX			= 0x30F0;	// 写i2c等待数据
	public static int	CMD_I2C_TXRX_ACK		= 0x30F1;	// 写读i2c数据回应

	public static int  LOW_GPIO_LEV=0;      ///GPIO脚拉低
	public static int  HIGHT_GPIO_LEV=1;    ///GPIO脚拉高

	public static int eGPIO_Output=0; //  输出模式
	 public static int eGPIO_Input=1;  //  输入模式

}


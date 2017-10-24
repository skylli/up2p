package com.ulink;

public class UlinkIIC {
	public int clk_pin;	// i2c scl pin 脚号
	public int sda_pin;	// i2c sda pin 脚号
	public int speed;	// i2c 速度，1,10,40,单位10k
	public int address;	// i2c 从设备地址
	public int w_len;   // 写数据长度
	public int r_len;   // 读数据长度
	public byte date[]; // 读/写数据内容
}

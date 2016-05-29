/**
 *
 * 由于Interrupt里面没有开放关中断和开中断的接口，所以采用外部中断的方式
 * 通过配置中断优先级去防止TaskManager打断update函数
 */





#ifndef _AM2302_H_
#define _AM2302_H_

#include "stm32f10x.h"
#include "GPIO.h"
#include "InputCapture_EXIT.h"
#include "Senser.h"

class AM2302:public Senser
{

private:
	//缓存和校验
	u8 sbuf,check;
	//原始数据
	u8 OriginalData[5];
	//保存解读后数据
	u8 DHT22_data[4];
	//中断控制
	InputCapture_EXIT *mICP;
	//引脚定义
	GPIO DATApin;

	void DHT22_PortIN(void);
	void DHT22_PortOUT(void);

	unsigned char StartDHT22(void);
	/**
	 *接收一组数据存放到sbuf里面
	 */
	u8 get_DHT22data_8(void);
	void get_DHT22data_Byte(void);
	void InitOriginalData(void);
	
public:
	
	AM2302(GPIO DataPin,InputCapture_EXIT *icp);
	u8 getData();

virtual bool Updata();

//返回温度数据
virtual	float GetFloatData();
virtual unsigned char Data_Hight_8(); 
virtual unsigned char Data_Low_8();

//返回湿度数据
float GetFloatData_H();
unsigned char Data_Hight_8_H(); 
unsigned char Data_Low_8_H();




};

//#define DHT22_GPIOx GPIOA
//#define DHT22_GPIO_Pin_x GPIO_Pin_4

#endif

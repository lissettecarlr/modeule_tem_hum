#ifndef __TRANSMISSION_H
#define __TRANSMISSION_H

#include "stm32f10x.h"
#include "USART.h"
#include "esp8266.h"

/*define**************************************************/
#define HumidityNumber 		0x01
#define TemperatureNumber 0x02
#define CO2Number 				0x03
#define DustNumber 				0x04
#define HCHONumber				0x05

#define ModuleNuber       0x12345678

/*END*****************************************************/



class Transmission{
	
	private:
		u8 ModuleToModule[20];
		u8 ModuleToUser[20];
	public:
		Transmission();
	
	//温湿度模块数据封装
		u8 *humidityModuleToModule(u8 data1,u8 data2,u8 adc); //to client
		u8 *temperatureModuleToModule(u8 data1,u8 data2,u8 adc);
		u8 *humidityModuleToUser(u8 data1,u8 data2,u8 adc);  //to server
		u8 *temperetureModuleToUser(u8 data1,u8 data2,u8 adc);
	  
	//CO2模块数据封装
		u8 *CO2_ModuleToUser(u8 data1,u8 data2,u8 adc);
		u8 *CO2_ModuleToModule(u8 data1,u8 data2,u8 adc);
	
	//有害气体模块数据封装
		u8 *HCHO_ModuleToUser(u8 data1,u8 data2,u8 adc);
		u8 *HCHO_ModuleToModule(u8 data1,u8 data2,u8 adc);
	
	//粉尘模块数据封装
		u8 *Dust_ModuleToUser(u8 data1,u8 data2,u8 adc);
		u8 *Dust_ModuleToModule(u8 data1,u8 data2,u8 adc);
	

		u8 CommandParsing(u8 command[8]);//命令协议校验解包
	
		u8 GetStateOrder(USART &ListeningCOM);//监听某个端口 返回命令字节

		void SendClient(u8 data1,u8 data2,u8 data3,u8 data4,u8 Voltage,esp8266 &esp); //发送给客服端
		void SendServer(u8 data1,u8 data2,u8 data3,u8 data4,u8 Voltage,esp8266 &esp); //发送给服务器
		void SendAlive(esp8266 &esp,bool mode); //true: send server   false send client
	
};

extern Transmission CMCT_Tool;

#endif

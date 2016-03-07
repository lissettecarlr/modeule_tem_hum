#ifndef __TRANSMISSION_H
#define __TRANSMISSION_H

#include "stm32f10x.h"
#include "USART.h"

/*define**************************************************/
#define HumidityNumber 0x01
#define TemperatureNumber 0x02

/*END*****************************************************/



class Transmission{
	
	private:
		u8 ModuleToModule[20];
		u8 ModuleToUser[20];
	public:
		Transmission();
		u8 *humidityModuleToModule(u8 data1,u8 data2,u8 adc);
		u8 *temperatureModuleToModule(u8 data1,u8 data2,u8 adc);
	
		u8 *humidityModuleToUser(u8 data1,u8 data2,u8 adc);
		u8 *temperetureModuleToUser(u8 data1,u8 data2,u8 adc);
	
		u8 CommandParsing(u8 command[8]);//命令协议校验解包
	
		u8 GetStateOrder(USART &ListeningCOM);//监听某个端口 返回命令字节
	
};

extern Transmission CMCT_Tool;

#endif

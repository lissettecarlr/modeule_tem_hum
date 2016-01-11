#ifndef __TRANSMISSION_H
#define __TRANSMISSION_H

#include "stm32f10x.h"

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
	
		u8 CommandParsing(u8 command[8]);
	
};

extern Transmission packaging;

#endif

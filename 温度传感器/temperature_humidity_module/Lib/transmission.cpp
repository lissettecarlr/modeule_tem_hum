#include "transmission.h"

Transmission packaging;

Transmission::Transmission()
{
	for(int i=0;i<20;i++)
	{
		ModuleToUser[i]=0;
		ModuleToModule[i]=0;
	}
}

//湿度数据封装
u8 *Transmission::humidityModuleToModule(u8 data1,u8 data2,u8 adc)
{
	//包头
	ModuleToModule[0]=0xff;
	ModuleToModule[1]=0xcc;
	
	//设备号
	ModuleToModule[2]=0x00;
	ModuleToModule[3]=0x00;
	ModuleToModule[4]=0x00;
	ModuleToModule[5]=0x01;
	//类型
	ModuleToModule[6]=0x01;
	//数据
	ModuleToModule[7]=data1;
	ModuleToModule[8]=data2;
	//电量
	ModuleToModule[9]=adc;
	
	for(int i=0;i<19;i++)
	{
		ModuleToModule[19]+=ModuleToModule[i];
	}
	return ModuleToModule;
}

//温度数据封装
u8 *Transmission::temperatureModuleToModule(u8 data1,u8 data2,u8 adc)
{
		//包头
	ModuleToModule[0]=0xff;
	ModuleToModule[1]=0xcc;
	
	//设备号
	ModuleToModule[2]=0x00;
	ModuleToModule[3]=0x00;
	ModuleToModule[4]=0x00;
	ModuleToModule[5]=0x01;
	//类型
	ModuleToModule[6]=0x02;
	//数据
	ModuleToModule[7]=data1;
	ModuleToModule[8]=data2;
	//电量
	ModuleToModule[9]=adc;
	
	for(int i=0;i<19;i++)
	{
		ModuleToModule[19]+=ModuleToModule[i];
	}
	return ModuleToModule;
}


u8 *Transmission::humidityModuleToUser(u8 data1,u8 data2,u8 adc)
{
	//一帧数据=2字节包头+4字节设备号+1字节数据类别+2字节数据+1字节电压 +9字节保留+1字节校验

	//头
	ModuleToUser[0]=0xff;
	ModuleToUser[1]=0xaa;
	//设备号
	ModuleToUser[2]=0x00;
	ModuleToUser[3]=0x00;
	ModuleToUser[4]=0x00;
	ModuleToUser[5]=0x01;
	//数据类型
	ModuleToUser[6]=0x01;
	//数据
	ModuleToUser[7]=data1;
	ModuleToUser[8]=data2;
	//电压值
	ModuleToUser[9]=adc;
	
	for(int i=0;i<19;i++)
	{
		ModuleToUser[19]+=ModuleToUser[i];
	}
	return ModuleToUser;
}

u8 *Transmission::temperetureModuleToUser(u8 data1,u8 data2,u8 adc)
{
	//头
	ModuleToUser[0]=0xff;
	ModuleToUser[1]=0xaa;
	//设备号
	ModuleToUser[2]=0x00;
	ModuleToUser[3]=0x00;
	ModuleToUser[4]=0x00;
	ModuleToUser[5]=0x01;
	//数据类型
	ModuleToUser[6]=0x02;
	//数据
	ModuleToUser[7]=data1;
	ModuleToUser[8]=data2;
	//电压值
	ModuleToUser[9]=adc;
	
	for(int i=0;i<19;i++)
	{
		ModuleToUser[19]+=ModuleToUser[i];
	}
	return ModuleToUser;
}

//得到上位机命令
u8 Transmission::CommandParsing(u8 command[8])
{
	if(command[0]==0xff&&command[1]==0xdd)
	{
		u32 equipment;
		equipment=(u32)(command[2]<<24)+(u32)(command[3]<<16)+(u32)(command[4]<<8)+(u32)command[5];
		if(equipment==2) //看是否是该设备
		{
			u32 sum=0;
			for(u8 i=0;i<7;i++)
			  {
				sum+=command[i];
			  }
			  if(command[7]==(u8)sum) //和校验
			  {
					return command[6];
			  }
		}
	}
	return 0;
}

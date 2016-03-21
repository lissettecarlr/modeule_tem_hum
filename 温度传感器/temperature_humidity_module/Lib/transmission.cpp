#include "transmission.h"

Transmission CMCT_Tool;

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
	ModuleToModule[6]=HumidityNumber;
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
	ModuleToModule[6]=TemperatureNumber;
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
	ModuleToUser[6]=HumidityNumber;
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
	ModuleToUser[6]=TemperatureNumber;
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

u8 *Transmission::CO2_ModuleToUser(u8 data1,u8 data2,u8 adc)
{
	//头
	ModuleToUser[0]=0xff;
	ModuleToUser[1]=0xaa;
	//设备号
	ModuleToUser[2]=0x00;
	ModuleToUser[3]=0x00;
	ModuleToUser[4]=0x00;
	ModuleToUser[5]=0x02;
	//数据类型
	ModuleToUser[6]=3;
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

u8 *Transmission::CO2_ModuleToModule(u8 data1,u8 data2,u8 adc)
{
		//包头
	ModuleToModule[0]=0xff;
	ModuleToModule[1]=0xcc;
	
	//设备号
	ModuleToModule[2]=0x00;
	ModuleToModule[3]=0x00;
	ModuleToModule[4]=0x00;
	ModuleToModule[5]=0x02;
	//类型
	ModuleToModule[6]=3;
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



//得到上位机命令
u8 Transmission::CommandParsing(u8 command[8])
{
	if(command[0]==0xff&&command[1]==0xdd)
	{
		u32 equipment;
		equipment=(u32)(command[2]<<24)+(u32)(command[3]<<16)+(u32)(command[4]<<8)+(u32)command[5];
		if(equipment==1) //看是否是该设备
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

//监听一个端口，返回一个命令
u8 Transmission::GetStateOrder(USART &ListeningCOM)
{
		u8 comand[8]={0};
		u8 ch=0;
		u8 num = ListeningCOM.ReceiveBufferSize();
		if(num>7)   //一帧命令包含8个字节
		{
				ListeningCOM.GetReceivedData(&ch,1);
			if(ch == 0xFF)
			{
				ListeningCOM.GetReceivedData(&ch,1);
				if(ch == 0xDD)
				{
					while(ListeningCOM.ReceiveBufferSize()<6);//等待数据
					comand[0]=0xff;
					comand[1]=0xDD;
					ListeningCOM.GetReceivedData(comand+2,6);
					ListeningCOM.ClearReceiveBuffer();
					return CommandParsing(comand);  //解包
				}
				else return 0;
			}
			else
				return 0;
		}
		else 
			return 0;
}

//发送数据给上位机
void Transmission::SendOnce(u8 data1,u8 data2,u8 data3,u8 data4,u8 Voltage,USART &usart, esp8266 &esp)
{
				esp.Send(20,humidityModuleToUser(data1,data2,Voltage));
				tskmgr.DelayMs(1000);//如果不延时，下一条将发送不出去
				tskmgr.DelayMs(1000);
				esp.Send(20,temperetureModuleToUser(data3,data4,Voltage));
						
				usart.SendData((humidityModuleToUser(data1,data2,Voltage)),20);			
				usart.SendData((temperetureModuleToUser(data3,data4,Voltage)),20);
}




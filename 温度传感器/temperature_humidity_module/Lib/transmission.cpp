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

//发送给服务器的帧格式封装
u8 *Transmission::ToServerPack(u8 DataType,u8 DataH,u8 DataL,u8 adc)
{
	//头
	ModuleToUser[0]=0xff;
	ModuleToUser[1]=0xaa;
	//设备号
	ModuleToUser[2]=ModuleNuber>>24;
	ModuleToUser[3]=(ModuleNuber>>16) &0xff;
	ModuleToUser[4]=(ModuleNuber>>8) 	&0xff;
	ModuleToUser[5]=(ModuleNuber)			&0xff;
	//数据类型
	ModuleToUser[6]=DataType;
	//数据
	ModuleToUser[7]=DataH;
	ModuleToUser[8]=DataL;
	//电压值
	ModuleToUser[9]=adc;
	
	for(int i=0;i<19;i++)
	{
		ModuleToUser[19]+=ModuleToUser[i];
	}
	return ModuleToUser;
	
}
//发送给客服端的帧格式封装
u8 *Transmission::ToClientPack(u8 DataType,u8 DataH,u8 DataL,u8 adc)
{
	//包头
	ModuleToModule[0]=0xff;
	ModuleToModule[1]=0xcc;
	//设备号
	ModuleToModule[2]= ModuleNuber>>24;
	ModuleToModule[3]=(ModuleNuber>>16) &0xff;
	ModuleToModule[4]=(ModuleNuber>>8) 	&0xff;
	ModuleToModule[5]=(ModuleNuber)			&0xff;
	//类型
	ModuleToModule[6]=DataType;
	//数据
	ModuleToModule[7]=DataH;
	ModuleToModule[8]=DataL;
	//电量
	ModuleToModule[9]=adc;
	for(int i=0;i<19;i++)
	{
		ModuleToModule[19]+=ModuleToModule[i];
	}
	return ModuleToModule;
}


//湿度数据封装
u8 *Transmission::humidityModuleToModule(u8 data1,u8 data2,u8 adc)
{
	//包头
	ModuleToModule[0]=0xff;
	ModuleToModule[1]=0xcc;
	
	//设备号
	ModuleToModule[2]=ModuleNuber>>24;
	ModuleToModule[3]=(ModuleNuber>>16) &0xff;
	ModuleToModule[4]=(ModuleNuber>>8) 	&0xff;
	ModuleToModule[5]=(ModuleNuber)			&0xff;
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
	ModuleToModule[2]=ModuleNuber>>24;
	ModuleToModule[3]=(ModuleNuber>>16) &0xff;
	ModuleToModule[4]=(ModuleNuber>>8) 	&0xff;
	ModuleToModule[5]=(ModuleNuber)			&0xff;
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
	ModuleToUser[2]=ModuleNuber>>24;
	ModuleToUser[3]=(ModuleNuber>>16) &0xff;
	ModuleToUser[4]=(ModuleNuber>>8) 	&0xff;
	ModuleToUser[5]=(ModuleNuber)			&0xff;
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
	ModuleToUser[2]=ModuleNuber>>24;
	ModuleToUser[3]=(ModuleNuber>>16) &0xff;
	ModuleToUser[4]=(ModuleNuber>>8) 	&0xff;
	ModuleToUser[5]=(ModuleNuber)			&0xff;
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
	ModuleToUser[2]=ModuleNuber>>24;
	ModuleToUser[3]=(ModuleNuber>>16) &0xff;
	ModuleToUser[4]=(ModuleNuber>>8) 	&0xff;
	ModuleToUser[5]=(ModuleNuber)			&0xff;
	//数据类型
	ModuleToUser[6]=CO2Number;
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
	ModuleToModule[2]=ModuleNuber>>24;
	ModuleToModule[3]=(ModuleNuber>>16) &0xff;
	ModuleToModule[4]=(ModuleNuber>>8) 	&0xff;
	ModuleToModule[5]=(ModuleNuber)			&0xff;
	//类型
	ModuleToModule[6]=CO2Number;
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
void Transmission::SendServer(u8 data1,u8 data2,u8 data3,u8 data4,u8 Voltage,esp8266 &esp)
{
				esp.Send(20,ToServerPack(HumidityNumber,data1,data2,Voltage));
				tskmgr.DelayS(2);//如果不延时，下一条将发送不出去
				esp.Send(20,ToServerPack(TemperatureNumber,data3,data4,Voltage));
}

void Transmission::SendClient(u8 data1,u8 data2,u8 data3,u8 data4,u8 Voltage,esp8266 &esp)
{
				esp.Send(0,20,ToClientPack(HumidityNumber,data1,data2,Voltage));
				tskmgr.DelayS(2);//如果不延时，下一条将发送不出去
				esp.Send(0,20,ToClientPack(TemperatureNumber,data3,data4,Voltage));
}

void Transmission::SendAlive(esp8266 &esp,u8 DataType,bool mode)
{
	if(mode)
	{
		esp.Send(20,ToServerPack(DataType,0xff,0xff,0xff));
	}
	else
	{
		esp.Send(0,20,ToClientPack(DataType,0xff,0xff,0xff));
	}
}

bool Transmission::GetWifiNameAndPassword(char *name,char *password,USART &ListeningCOM)
{
	u8 ch=0;
	u8 i =0;
	ListeningCOM.GetReceivedData(&ch,1);
		if(ch == 0xFF)
		{
			tskmgr.DelayMs(100);
			ListeningCOM.GetReceivedData(&ch,1);
			if(ch == 0x03)
			{
				ListeningCOM.GetReceivedData(&ch,1);
				while(ch!=0xff){
					*(name+i)=ch;
					i++;
					ListeningCOM.GetReceivedData(&ch,1);
				}
					*(name+i)='\0';
					ListeningCOM.GetReceivedData(&ch,1);
					i=0;
				while(ch!=0xff){
					*(password+i)=ch;
					i++;
					ListeningCOM.GetReceivedData(&ch,1);
				}
					*(password+i)='\0';	
				return 1;
			}
			else return 0;
		}
		else
			return 0;
}
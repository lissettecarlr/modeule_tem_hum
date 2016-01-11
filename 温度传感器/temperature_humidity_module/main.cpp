#include "ADC.h"
#include "Timer.h"
#include "USART.h"
#include "Interrupt.h"
#include "TaskManager.h"
#include "AM2302.h"
#include "transmission.h"


/*ESP8266 指令*******************************************************************************************************/
u8 setmode[13]={0x41,0x54,0x2B,0x43,0x57,0x4D,0x4F,0x44,0x45,0x3D,0x32,0x0D,0x0A};//设置模式为2
//名字fzj  密码1234567890
u8 namepassword[33]={0x41,0x54,0x2B,0x43,0x57,0x53,0x41,0x50,0x3D,0x22,0x46,0x5A,0x4A,0x22,
					0x2C,0x22,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x22,0x2C,0x35,0x2C,0x33,0x0D,0x0A};
u8 Command1[13]={0x41,0x54,0x2B,0x43,0x49,0x50,0x4D,0x55,0x58,0x3D,0x31,0x0D,0x0A}; //设置多路连接
//设置IP地址为192.168.1.1
u8 setIP[24]={0x41,0x54,0x2B,0x43,0x49,0x50,0x41,0x50,0x3D,0x22,0x31,0x39,0x32,0x2E,0x31,0x36,0x38,0x2E,0x31,0x2E,0x31,0x22,0x0D,0x0A};
	
//开启服务器模式 端口 8080
u8 Command2[21]={0x41,0x54,0x2B,0x43,0x49,0x50,0x53,0x45,0x52,0x56,0x45,0x52,0x3D,0x31,0x2C,0x38,0x30,0x38,0x30,0x0D,0x0A};
//发送指令
u8 Command3[17]={0x41,0x54,0x2B,0x43,0x49,0x50,0x53,0x45,0x4E,0x44,0x3D,0x30,0x2C,0x32,0x30,0x0D,0x0A};
//取消回传
u8 CLOSS[6]={0x41,0x54,0x45,0x30,0x0D,0x0A};
/*END***************************************************************************************************************/

#define CONTINUOUS 3
#define ONCE 2
#define STANDBY 1
#define ALIVE 0xFF
#define BEEP 0xBB




USART com(1,115200,false);   //USART1
USART WIFI(3,115200,true);   //USART3

ADC voltage(8);
AM2302 am2302;


int main()
{
//	static float newTime = 0, oldTime = 0, interval = 0;
		
	//WIFI配置
	
//	//设置模式
//	WIFI.SendData(setmode,13); 
//	tskmgr.DelayMs(1000);
//	WIFI.SendData(namepassword,33); 
//	tskmgr.DelayMs(1000);
//	//设置IP地址
//	WIFI.SendData(setIP,24); 
//	tskmgr.DelayMs(1000);
//	
//	//复位
//	WIFI<<"AT+RST\t\n";
//	tskmgr.DelayMs(3000);
	
	WIFI.SendData(CLOSS,6);  //关闭回显
	tskmgr.DelayMs(1000);
	WIFI.SendData(Command1,13);  //开启多路连接
	tskmgr.DelayMs(1000);
	WIFI.SendData(Command2,21); //开启服务器模式
	tskmgr.DelayMs(1000);
	
	
	
	u8 state_now=1; //用于选择模式
	u8 comand[8];
	static u8 COMANDS;
	u8 state_WIFI=0;
	
	while(1)
	{
		if(com.ReceiveBufferSize()>7)   //一帧命令包含8个字节 
		{
			com.GetReceivedData(comand,8);
			com.ClearReceiveBuffer();
			COMANDS=packaging.CommandParsing(comand); 
			
			if(COMANDS==2) //表示更新一次
			{
				state_now=ONCE;  
			}
			else if(COMANDS==3)//连续更新
			{
				state_now=CONTINUOUS;
			}
			else if(COMANDS==1)
			{
				state_now=STANDBY; //不更新
			}
			else if(COMANDS==0xff)
			{
			  state_now=ALIVE; //确认存在
			}
			else if(COMANDS==0xbb)//蜂鸣器
			{
				state_now=BEEP;
			}
			else
			{
			}			  			
		}
		//WIFI  连接
		if(state_WIFI==1){ //如果开启了WIFI模式（仅仅从模块）
		if(WIFI.ReceiveBufferSize()>40)
		{
			u8 ch=0;
			u8 temp[4];
			u32 ModuleNO;
			//test	
			while(WIFI.ReceiveBufferSize()>8)
			{
				WIFI.GetReceivedData(&ch,1);
				if(ch==0xff)//判断包头
				{
					WIFI.GetReceivedData(&ch,1);
					if(ch==0xdd)//判断包头
					{
					    WIFI.GetReceivedData(temp,4);//取出设备号
						ModuleNO=temp[0]+temp[1]+temp[2]+temp[3];
						if(ModuleNO==1)//如果模块号正确
						{
							WIFI.GetReceivedData(&COMANDS,1);//取出命令
							
							if(COMANDS==2) //表示更新一次
							{
							state_now=ONCE;  
							}
							else if(COMANDS==3)//连续更新
							{
							state_now=CONTINUOUS;
							}
							else if(COMANDS==1)
							{
							state_now=STANDBY; //不更新
							}
							else if(COMANDS==0xff)
							{
							state_now=ALIVE; //确认存在
							}
							else if(COMANDS==0xbb)//蜂鸣器
							{
							state_now=BEEP;
							}
							else
							{
							}			 
						}
					}
				}
			}
		}
	  }
		
		
		//命令判断
		switch(state_now)
		{
			case ONCE:{
				
				am2302.Updata();
				
				com.SendData((packaging.temperatureModuleToModule(am2302.DHT22_data[0],am2302.DHT22_data[1],voltage[8]*100)),20);		
				tskmgr.DelayMs(100);
				com.SendData((packaging.humidityModuleToModule(am2302.DHT22_data[2],am2302.DHT22_data[3],voltage[8]*100)),20);	
				
				if(state_WIFI==1){ //如果开启了WIFI模式（仅仅从模块）
				//WIFI发送
				WIFI.SendData(Command3,17);
				tskmgr.DelayMs(100);
				WIFI.ClearSendBuffer();	
				WIFI.SendData((packaging.temperatureModuleToModule(am2302.DHT22_data[0],am2302.DHT22_data[1],voltage[8]*100)),20);		
				tskmgr.DelayMs(500);
				
				WIFI.SendData(Command3,17);
				tskmgr.DelayMs(100);
				WIFI.ClearSendBuffer();	
				com.SendData((packaging.humidityModuleToModule(am2302.DHT22_data[2],am2302.DHT22_data[3],voltage[8]*100)),20);	
				tskmgr.DelayMs(500);
				}
				
			state_now=STANDBY;
			}break;
			
			case CONTINUOUS:{
			
				if(am2302.Updata())
				{
				com.SendData((packaging.temperatureModuleToModule(am2302.DHT22_data[0],am2302.DHT22_data[1],voltage[8]*100)),20);
				tskmgr.DelayMs(100);
				com.SendData((packaging.temperatureModuleToModule(am2302.DHT22_data[2],am2302.DHT22_data[3],voltage[8]*100)),20);

				//WIFI
				if(state_WIFI==1){ //如果开启了WIFI模式（仅仅从模块）
					WIFI.SendData(Command3,17);
					tskmgr.DelayMs(100);
					WIFI.ClearSendBuffer();	
					com.SendData((packaging.temperatureModuleToModule(am2302.DHT22_data[0],am2302.DHT22_data[1],voltage[8]*100)),20);
					tskmgr.DelayMs(500);
					
					WIFI.SendData(Command3,17);
					tskmgr.DelayMs(100);
					WIFI.ClearSendBuffer();	
					com.SendData((packaging.temperatureModuleToModule(am2302.DHT22_data[2],am2302.DHT22_data[3],voltage[8]*100)),20);
					tskmgr.DelayMs(500);
					
				}													
				tskmgr.DelayMs(1000);

				}
							
			}break;
			
			case STANDBY:{
				
			}break;
			
			case ALIVE:{  //确认存在

					com.SendData((packaging.temperatureModuleToModule(0XFF,0XFF,0xFF)),20); //存在信号
				
				if(state_WIFI==1){ //如果开启了WIFI模式（仅仅从模块）
					WIFI.SendData(Command3,17);
					tskmgr.DelayMs(100);
					WIFI.ClearSendBuffer();	
					WIFI.SendData((packaging.temperatureModuleToModule(0XFF,0XFF,0xFF)),20);
					tskmgr.DelayMs(500);
				}
				
					state_now=STANDBY;
			}break;
			
			
		}
		


	}
}
	

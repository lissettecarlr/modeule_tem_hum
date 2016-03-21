/*--------------------------------------------------------------------------------------*/
/*
stdlib  ADC GPIO  USART Memory Interrupt  TaskManager    
*/
/*--------------------------------------------------------------------------------------*/
/*
AM2302  ESP8266
*/
/*--------------------------------------------------------------------------------------*/
/*
 Hint transmission  WIFI_Memory
*/
/*--------------------------------------------------------------------------------------*/

#include "stdlib.h"
#include "ADC.h"
#include "GPIO.h"
//#include "Timer.h"
#include "USART.h"
#include "Memory.h"
#include "Interrupt.h"
#include "TaskManager.h"
#include "AM2302.h"
#include "transmission.h"
#include "ESP8266.h"
#include "Hint.h"
#include "WIFI_Memory.h"


/*DEFINE********************************************************/
#define DELAY 					0x01
#define START 					0x02
#define GETWIFI					0x03
#define REGISTER 				0xaa
#define BEEP 						0xBB
#define RESET 					0XCC
#define MODEL 					0xEE
#define ALIVE 					0xff


#define MODULE_IP 			"192.168.1.1"   
#define MODULE_COM			9000
#define SERVER_IP				"120.27.119.115"
#define SERVER_COM 			1111
/*END********************************************************/

u8 ConnectNetwork_client(char *WifiName,char* WifiPassword,char *IP,int COM);
u8 ConnectNetwork_server(int port,int time);
bool GetWifiNameAndPassword(char *name,char *password,USART &ListeningCOM);


USART com(1,115200,true);   //USART1
USART WIFI(3,115200,true);   //USART3

GPIO Beep(GPIOA,11,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
GPIO Led(GPIOB,0,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);

Memory InfoStore(0x08000000+100*MEMORY_PAGE_SIZE,true);//ip存储，长度+IP地址的存档格式，先读取出字符串长度
WifiMemory wifimemory(InfoStore);

AM2302 am2302;
esp8266 wifi(WIFI);

hint Hint(Led,Beep);

int main(){
	
	u8 order=0;
	double record_updataSensor=0;
	double record_alive=0;
	double record_getwifi=0;
	bool network=false ; //网络连接标识位 true ： client模式  false： server模式
	char *ip = (char*)calloc(15, sizeof(char*) ); 
	char *WifiName = (char*)calloc(20, sizeof(char*) ); 
	char *WifiPassword = (char*)calloc(20, sizeof(char*) ); 

/*开机WIFI模式选择*****************************************************************/
	if(wifimemory.getWifiSum()!=0)//判断表中是否已经保存wifi信息
	{
		while( wifimemory.Load(WifiName,WifiPassword) )
		 {
			  if(ConnectNetwork_client(WifiName,WifiPassword,SERVER_IP,1111)) //每次连接历时20秒
				{network=true;   break;}
		 }
		 if(network ==false) 
			  ConnectNetwork_server(MODULE_COM,0);
	}
  else
	{
		 ConnectNetwork_server(MODULE_COM,0);
	}
	
	Led.SetLevel(1);//将指示灯熄灭（模式切换结束）
/*END*********************************************************************************/	
	

	while(1)
	{		
		order=CMCT_Tool.GetStateOrder(WIFI);
		switch(order)
		{
			case DELAY:{}break;
			case START:{//启动
				while(1){
					while( am2302.NewDataFlag==false){am2302.Updata();tskmgr.DelayS(3);} //一直等待有一组新数据
						CMCT_Tool.SendOnce(am2302.DHT22_data[0],am2302.DHT22_data[1],am2302.DHT22_data[2],am2302.DHT22_data[3],0,wifi);
						am2302.NewDataFlag=false ; //更改新数据标识位			
						if(CMCT_Tool.GetStateOrder(WIFI)==DELAY)
							break;
				}
			}break;
			case RESET:{//复位
					wifimemory.ClearAllData(); //清空所有保存信息
					*((u32 *)0xE000ED0C) = 0x05fa0004;
			}break;
			case GETWIFI:{//得到wifi账号密码
					while(1)
					{
						if(GetWifiNameAndPassword(WifiName,WifiPassword,WIFI) )
						{
								//保存WIFI账号密码
								break;
						}
						if(tskmgr.ClockTool(record_getwifi,10)) //超时10秒退出
						   break;
					}
			}break;
			case MODEL:{//模式切换
					ConnectNetwork_server(MODULE_COM,0);
			}break;
			case ALIVE:{//存活确认
				CMCT_Tool.SendOnce(0xff,0xff,0xff,0xff,0xff,wifi); //存活确认，数据位全为0xff
			}break;			
			
			default:		
			{  
					if(tskmgr.ClockTool(record_updataSensor,3)) //每三秒执行一次更新
						 am2302.Updata();//当没有命令时更新传感器
					if(tskmgr.ClockTool(record_alive,10)) //每10秒发送一次存活确认
						 CMCT_Tool.SendOnce(0xff,0xff,0xff,0xff,0xff,wifi);
			}
		}		
	}
}


bool GetWifiNameAndPassword(char *name,char *password,USART &ListeningCOM)
{
		u8 ch=0;
		u8 i =0;
		u8 num = ListeningCOM.ReceiveBufferSize();
		if(num>7)   //一帧命令包含8个字节
		{
				ListeningCOM.GetReceivedData(&ch,1);
			if(ch == 0xFF)
			{
				ListeningCOM.GetReceivedData(&ch,1);
				if(ch == 0x03)
				{
					while(ch!=0xff){
						ListeningCOM.GetReceivedData(&ch,1);
						*(name+i)=ch;
						i++;
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
		else 
			return 0;
}

u8 ConnectNetwork_client(char *WifiName,char* WifiPassword,char *IP,int COM) //预计耗时26秒
{
	//网络连接
	if(!wifi.kick())
		return 0;
	tskmgr.DelayMs(1000);
	wifi.setEcho(1);//回显
	tskmgr.DelayMs(1000);
	wifi.setOprToStation();//设置为模式1
	tskmgr.DelayMs(500);
	wifi.restart();
	tskmgr.DelayMs(1000);
	tskmgr.DelayMs(1000);
	tskmgr.DelayMs(1000);
	if(!wifi.joinAP(WifiName,WifiPassword))
		return 0;//WIFI连接 如果连接失败，返回0
	if( !wifi.ConnectServer("TCP",IP,COM) ) return false;  //服务器连接
	return 1;
}

u8 ConnectNetwork_server(int port,int time) //预计耗时7秒
{
	if(!wifi.kick())
		return 0;
	tskmgr.DelayMs(1000);
	wifi.setOprToSoftAP();
	tskmgr.DelayMs(500);
	wifi.restart();
	tskmgr.DelayMs(1000);
	tskmgr.DelayMs(1000);
	tskmgr.DelayMs(1000);
	wifi.enableOrDisableMUX(1); //开启多路访问
	tskmgr.DelayMs(1000);
	wifi.OpenServer(port);
	tskmgr.DelayMs(1000);
	wifi.SetTimeout(time);
	tskmgr.DelayMs(1000);
	WIFI.ClearReceiveBuffer();
	return 1;
}





//		wifimemory.ClearAllData();
//		char *str1 = "FFF";//3
//		char *str2 = "f19940202";//9
//		wifimemory.Save(str1,str2);
//	char *ip1 = "120.27.119.115";
//	char *ip2 = "192.168.191.1";
//	ConnectNetwork_client("FFF","f19940202",ip1,1111);
//	ConnectNetwork_server(8888,0);
//	
//	InfoStore.Write(0,0,ip1); //保存IP
//	InfoStore.Write(0,20,&a,1);
//	
//	InfoStore.Read(0,0,ip);//读取IP
//	InfoStore.Read(0,20,&b,1);//读取IP



/*
测试命令数据

发送一次数据：ff dd 00 00 00 01 02 DF
存活确认：ff dd 00 00 00 01 ff DC
*/

//test procedure


//	char *str1 = "FFF";//3
//	char *str2 = "f19940202";//9
//	char *str3 = "fzj";//3
//	char *str4 = "hahahahahaha";//9

//u8 ch;
//while(1)
//{
//		u8 num = com.ReceiveBufferSize();
//		if(num>=1)
//		{
//				com.GetReceivedData(&ch,1);
//				com.ClearReceiveBuffer();
//			switch (ch)
//			{
//				case 1:{wifimemory.ClearAllData();com<<"clear!!\n";}break;
//				case 2:{wifimemory.Save(str1,str2);com<<"Save1\n";}break;
//				case 3:{wifimemory.Save(str3,str4);com<<"Save2\n";}break;
//				case 4:{
//									if(wifimemory.Load(strA,strB))
//									{
//										com<<strA<<"\n";
//										com<<strB<<"\n";
//									}
//									else
//											com<<"No data\n";
//							}break;
//			}
//		}
//	
//}


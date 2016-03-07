#include "ADC.h"
//#include "Timer.h"
#include "USART.h"
#include "Interrupt.h"
#include "TaskManager.h"
#include "AM2302.h"
#include "transmission.h"
#include "GPIO.h"
#include "ESP8266.h"

/*DEFINE********************************************************/
#define DELAY 					0x01
#define SEND_ONCE 			0x02
#define REGISTER 				0xaa
#define DELETE 					0xdd
#define BEEP 						0xBB
#define RESET 					0XCC
#define ALIVE 					0xff

/*END********************************************************/

u8 ConnectNetwork(char *WifiName,char* WifiPassword,char *IP,int COM);

USART com(1,115200,true);   //USART1
USART WIFI(3,115200,true);   //USART3
GPIO Beep(GPIOA,11,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
AM2302 am2302;
esp8266 wifi(WIFI);


int main(){
	
	u8 order=0;
	
	char *ip1 = "120.27.119.115";
	char *ip2 = "192.168.191.1";
	
	com<<ConnectNetwork("FFF","f19940202",ip2,8080);
	
	while(1)
	{
		tskmgr.DelayMs(300); //延时是必要的！
////			com<<"hum:"<<am2302.DHT22_data[0]+am2302.DHT22_data[1]*0.1<<"\t";
////			com<<"tem:"<<am2302.DHT22_data[2]+am2302.DHT22_data[3]*0.1<<"\n";

		order=CMCT_Tool.GetStateOrder(com);
		switch(order)
		{
			case DELAY:{}break;
			case SEND_ONCE:{
				while( !am2302.Updata() || am2302.NewDataFlag==false); //一直等待有一组新数据
				wifi.Send(20,CMCT_Tool.humidityModuleToUser(am2302.DHT22_data[0],am2302.DHT22_data[1],0));
				tskmgr.DelayMs(1000);
				tskmgr.DelayMs(1000);
				wifi.Send(20,CMCT_Tool.temperatureModuleToModule(am2302.DHT22_data[2],am2302.DHT22_data[3],0));
				
				com.SendData((CMCT_Tool.humidityModuleToModule(am2302.DHT22_data[0],am2302.DHT22_data[1],0)),20);			
				com.SendData((CMCT_Tool.temperatureModuleToModule(am2302.DHT22_data[2],am2302.DHT22_data[3],0)),20);
				
				am2302.NewDataFlag=false ; //更改新数据标识位
			}break;
			
			case REGISTER:{}break;
			case DELETE:{}break;
			case BEEP:{}break;
			case RESET:{}break;
			case ALIVE:{}break;			
			default:am2302.Updata();//当没有命令时更新传感器
		}
	}
}


u8 ConnectNetwork(char *WifiName,char* WifiPassword,char *IP,int COM)
{
	//网络连接	
	tskmgr.DelayMs(1000);
	wifi.setEcho(1);//回显
	tskmgr.DelayMs(1000);
	wifi.setOprToStation();//设置为模式1
	tskmgr.DelayMs(1000);
	if(!wifi.joinAP(WifiName,WifiPassword)){} //WIFI连接 如果连接失败，返回0
//			return 0;
//	tskmgr.DelayMs(1000);
//	tskmgr.DelayMs(1000);
//	tskmgr.DelayMs(1000);
//	tskmgr.DelayMs(1000);
	wifi.ConnectServer("TCP",IP,8080); //服务器连接
	tskmgr.DelayMs(1000);
	return 1;
}


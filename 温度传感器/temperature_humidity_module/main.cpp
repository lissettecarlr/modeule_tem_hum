#include "ADC.h"
//#include "Timer.h"
#include "USART.h"
#include "Interrupt.h"
#include "TaskManager.h"
#include "AM2302.h"
#include "transmission.h"
#include "GPIO.h"
#include "ESP8266.h"
#include "Hint.h"
#include "Memory.h"
#include "stdlib.h"
#include "WIFI_Memory.h"


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
GPIO Led(GPIOB,0,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);

Memory InfoStore(0x08000000+100*MEMORY_PAGE_SIZE,true);//ip存储，长度+IP地址的存档格式，先读取出字符串长度


AM2302 am2302;
esp8266 wifi(WIFI);

hint Hint(Led,Beep);


int main(){
	
	u8 order=0;
		
//	char *ip = (char*)calloc(15, sizeof(char*) ); 

//	char *ip1 = "120.27.119.115";
//	char *ip2 = "192.168.191.1";
//	
//	uint16_t a =0x01;
//	uint16_t b;
//	
//	InfoStore.Write(0,0,ip1); //保存IP
//	InfoStore.Write(0,20,&a,1);
//	
//	InfoStore.Read(0,0,ip);//读取IP
//	InfoStore.Read(0,20,&b,1);//读取IP
	
//	com<<ip;
	
		char *ip1 = "FFF";//3
		char *ip2 = "f19940202";//9
//		InfoStore.Clear(0);
		WifiMemory wifimemory(InfoStore);
//		wifimemory.ClearAllData();
		
		wifimemory.Save(ip1,ip2);
		
	
	  Led.SetLevel(1);//将指示灯熄灭
//		InfoStore.Clear(0);
	
//	if(ConnectNetwork("FFF","f19940202",ip2,8080))
//	{
//			com<<"succeed\n";
//			Hint.ledFlicker_2s();
//	}
//	else
//	{
//			com<<"false\n";
//			Led.SetLevel(0);
//	}
//	
//	
	
	
	while(1)
	{
		tskmgr.DelayMs(300); //延时是必要的！
		order=CMCT_Tool.GetStateOrder(com);
		switch(order)
		{
			case DELAY:{}break;
			
			case SEND_ONCE:{
				while( am2302.NewDataFlag==false){am2302.Updata();} //一直等待有一组新数据
				CMCT_Tool.SendOnce(am2302.DHT22_data[0],am2302.DHT22_data[1],am2302.DHT22_data[2],am2302.DHT22_data[3],0,com,wifi);
				am2302.NewDataFlag=false ; //更改新数据标识位			
			}break;
			
			case REGISTER:{}break;
			case DELETE:{}break;
			case BEEP:{}break;
			case RESET:{}break;
			case ALIVE:{
				CMCT_Tool.SendOnce(0xff,0xff,0xff,0xff,0xff,com,wifi); //存活确认，数据位全为0xff
			}break;			
			default:am2302.Updata();//当没有命令时更新传感器
		}
	}
}




u8 ConnectNetwork(char *WifiName,char* WifiPassword,char *IP,int COM)
{
	//网络连接
	if(!wifi.kick())
		return 0;
	tskmgr.DelayMs(1000);
	wifi.setEcho(1);//回显
	tskmgr.DelayMs(1000);
	wifi.setOprToStation();//设置为模式1
	tskmgr.DelayMs(1000);
	if(!wifi.joinAP(WifiName,WifiPassword)) //WIFI连接 如果连接失败，返回0
			return 0;
//	tskmgr.DelayMs(1000);
//	tskmgr.DelayMs(1000);
//	tskmgr.DelayMs(1000);
//	tskmgr.DelayMs(1000);
	wifi.ConnectServer("TCP",IP,8080); //服务器连接
	tskmgr.DelayMs(1000);
	return 1;
}



/*
网络连接方案
		首先进行flash读取，如果发现没有保存连接wifi则自动切换到server模式，如果轮询保存的wifi都未连接上则也切换到server模式
		（未考虑连上的WIFI没有网络的问题，这将会导致数据无法上传到服务器）
		server模式需要有一下功能：
				1.能够接受到客服端的注册命令
				2.接受到客服端发送来的WIFI的账号密码。并将其保存到flash中。
				3.复位重启
				4.清空flash
*/




/*
测试命令数据

发送一次数据：ff dd 00 00 00 01 02 DF
存活确认：ff dd 00 00 00 01 ff DC

*/


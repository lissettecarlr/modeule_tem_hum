
#include "stdlib.h"
#include "ADC.h"
#include "GPIO.h"
//#include "Timer.h"
#include "USART.h"
#include "Memory.h"
#include "Interrupt.h"
#include "TaskManager.h"
#include "InputCapture_EXIT.h"
#include "am2302.h"
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


#define MODULE_IP 			"10.10.10.4"   
#define MODULE_COM			9000
#define SERVER_IP				"120.27.119.115"                          //"120.27.119.115"
#define TEST_SERVER_IP  "106.91.29.33"
#define SERVER_COM 			1111
/*END********************************************************/


USART com(1,115200,true);   //USART1  用于状态显示
USART WIFI(3,115200,true);   //USART3
USART equipment(2,9600,true);

GPIO Beep(GPIOA,11,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
GPIO Led(GPIOB,0,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);

Memory InfoStore(0x08000000+100*MEMORY_PAGE_SIZE,true);//ip存储，长度+IP地址的存档格式，先读取出字符串长度 0x08019000
WifiMemory wifimemory(InfoStore);


//驱动温湿度传感器
GPIO datapin(GPIOA,4);
InputCapture_EXIT ch1(GPIOA,4);
AM2302 am2302(datapin,&ch1);

esp8266 wifi(WIFI);

hint Hint(Led,Beep);

int main(){
	
	double record_updataSensor=0;

	while(1)
	{		

 
		if(tskmgr.ClockTool(record_updataSensor,3)) //每三秒执行一次更新
		{ 
			am2302.Updata();//当没有命令时更新传感器
			com<<am2302.Data_Hight_8()<<"\t"<<am2302.Data_Low_8()<<"\n";
		}
	}
}

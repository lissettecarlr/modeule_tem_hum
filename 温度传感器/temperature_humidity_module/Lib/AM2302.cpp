#include "AM2302.h"

AM2302::AM2302()
{
	UpdataInterval=0.1;
}

//设置管脚为输入模式
void AM2302::DHT22_PortIN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	//Configure pin as input 
	GPIO_InitStructure.GPIO_Pin = DHT22_GPIO_Pin_x ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//浮动输入
	GPIO_Init(DHT22_GPIOx,&GPIO_InitStructure);
}

//设置管脚为输出模式
void AM2302::DHT22_PortOUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	//Configure pin as input 
	GPIO_InitStructure.GPIO_Pin = DHT22_GPIO_Pin_x ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
	GPIO_Init(DHT22_GPIOx,&GPIO_InitStructure);
}

//开始读取数据
unsigned char AM2302::StartDHT22(void)
{
//   static float newTime = 0, oldTime = 0,interval = 0;
//        
//	//控制输出频率
//	newTime = tskmgr.Time();    //get current time
//	interval = newTime - oldTime;
//	if(interval<UpdataInterval) return 0;
//	oldTime = newTime;   //update old time
	
   DHT22_PortOUT();//管脚输出模式
   GPIO_ResetBits(DHT22_GPIOx , DHT22_GPIO_Pin_x); 
   tskmgr.DelayMs(18); //触发开始,总线拉低要大于18ms
  
   GPIO_SetBits(DHT22_GPIOx , DHT22_GPIO_Pin_x);  //给控制线一个20us的高电平	//释放总线
   tskmgr.DelayUs(25); //wait 20-40uS 等待DHT22的低电平响应信号
   
       
   DHT22_PortIN();//改为输入

   if(GPIO_ReadInputDataBit(DHT22_GPIOx,DHT22_GPIO_Pin_x)==0)   //DHT22响应  先拉低80us 再拉高80us	  此处未进行死循环处理
   {
	   
	   	   while(GPIO_ReadInputDataBit(DHT22_GPIOx,DHT22_GPIO_Pin_x)==0);
		     while(GPIO_ReadInputDataBit(DHT22_GPIOx,DHT22_GPIO_Pin_x)==1);
		     return 1;
	      
   }
	 return 0;
}

/****************************************************************
接收一组数据存放到sbuf里面
****************************************************************/
u8 AM2302::get_DHT22data_8(void)
{
	u8 i;
	for(i = 0;i<8;i++)
	{
		sbuf <<= 1;
		tskmgr.DelayUs(20);
		while(GPIO_ReadInputDataBit(DHT22_GPIOx,DHT22_GPIO_Pin_x)==0);//等到高电平的出现，高电平的时间表示的是数据位的0和1
		tskmgr.DelayUs(26);//数据为0的信号时间为26-28us，1则为70us

		if(GPIO_ReadInputDataBit(DHT22_GPIOx,DHT22_GPIO_Pin_x)==1)//如果电平仍然为高则表示数据为1
		{
			 sbuf |= 0x01;
			 tskmgr.DelayUs(40);//   20+26+40=86  >70
		}
		else //如果变低
			sbuf &= 0xfe;
	}
	return sbuf;
}
/***********************************************************************
功能：读取一组温湿度  湿度高+湿度低+温度高+温度低
       进行了和校验，正确返回1 错误返回0
       数据结果存入外联数组DHT22_data[4]
***********************************************************************/
u8 AM2302::Updata(void)
{
	u8 sum;
	u16 humidity;
	u16 temperature;
	
	u8 humidity_integer;//保存湿度整数部分
	u8 humidity_decimals;//保存湿度小数部分
	u8 temperature_integer;//保存温度整数部分
    u8 temperature_decimals;//保存温度小数部分
	
//	/**************************限制更新频率********************************/
//	static float newTime = 0, oldTime = 0, interval = 0;
//	newTime = tskmgr.Time();    //get current time
//	interval = newTime - oldTime;
//	if(interval<0.1) return 0;
//	oldTime = newTime;   //update old time
//	/*********************************END*********************************/
	
	if(StartDHT22())	//判断是否已经响应
	{
				
		OriginalData[0]=get_DHT22data_8();
	    OriginalData[1]=get_DHT22data_8();
		OriginalData[2]=get_DHT22data_8();	
	    OriginalData[3]=get_DHT22data_8();
		check = get_DHT22data_8();
		sum = (OriginalData[0]+OriginalData[1]+OriginalData[2]+OriginalData[3]);
	}
	if(check == sum)
	{
	  /*在这儿 把传回来的值解算出来
		传回来的一组数据，先将高八位和第八位组合算出10进制数，其中高两位是整数，末位是小数
	  */
		
		//将湿度数据解码
		humidity=(((u16)OriginalData[0])<<8)|((u16)OriginalData[1]);		
		humidity_decimals=humidity%10;		
		humidity_integer=humidity/10;
		
		//将温度数据解码
		temperature=(((u16)OriginalData[2])<<8)|((u16)OriginalData[3]);		
		temperature_integer=temperature/10;
		temperature_decimals=temperature%10;
		
		//重新存入
		DHT22_data[0]=humidity_integer;
		DHT22_data[1]=humidity_decimals;			
		DHT22_data[2]=temperature_integer;
		DHT22_data[3]=temperature_decimals;
					
	   return(1);
	}
	else	
	return 0;
}

float AM2302::GetTemperature()
{
	
	return (DHT22_data[2]+((float)DHT22_data[3])/10.0);
}


float AM2302::GetHumidity()
{
	return (DHT22_data[0]+((float)DHT22_data[1])/10.0);
}


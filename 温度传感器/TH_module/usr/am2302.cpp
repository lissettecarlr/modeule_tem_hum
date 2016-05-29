#include "am2302.h"
#include "TaskManager.h"



#define USE_CAPTURE_PPM_EXIT



AM2302::AM2302(GPIO DataPin,InputCapture_EXIT *icp):DATApin (DataPin)				
{
	mICP=icp;
	mICP->Close();

	sbuf = 0;
	check = 0;

}


//设置管脚为上啦输入模式
void AM2302::DHT22_PortIN(void)
{
	DATApin.ChangeMode(GPIO_Mode_IN_FLOATING);
}

//设置管脚为下拉输出模式
void AM2302::DHT22_PortOUT(void)
{
	DATApin.ChangeMode(GPIO_Mode_Out_PP);
}

//根据搜集到的数据处理
void AM2302::get_DHT22data_Byte()
{

	for(u8 j = 0 ;j < 5;j++ )
	{
		for(u8 i = 0 ;i < 8;i++ )
		{
//			sbuf <<= 1;
			sbuf <<= 1;
			{

				//当有数据更新的时候就去判断获得的数据是1还是0
				if(mICP->mdataTime[j*8+i]<=30 && mICP->mdataTime[j*8+i]>= 20)
				{
					
					//说明获得是是0
					sbuf &= 0xfe;
				}
				else
					sbuf |= 0x01;
			}

		}
		OriginalData[j] = sbuf;
		sbuf = 0;
	}
//	return sbuf;
	
}

u8 AM2302::getData()
{
	u8 sum;

	DHT22_PortOUT();//管脚输出模式
	DATApin.SetLevel(0);
	tskmgr.DelayMs(5); //触发开始
	DATApin.SetLevel(1); //给控制线一个20us的高电平	//释放总线
	tskmgr.DelayUs(25); //触发开始
	DHT22_PortIN();
	
	if(DATApin.GetLevel() == 0)//DHT22响应  先拉低80us 再拉高80us	  此处未进行死循环处理
	{
				 
				 while(DATApin.GetLevel() == 0);		
				 while(DATApin.GetLevel() == 1)
				 {
						if(DATApin.GetLevel() == 0)
							break;
				 }
				 
	      
   }
	 mICP->InitdataTime();
	 mICP->Start();


	 tskmgr.DelayMs(500); //触发期间
	 tskmgr.DelayUs(1); //触发开始
	 get_DHT22data_Byte();//改变OrginaData的算法
	 tskmgr.DelayUs(1); //触发开始

//	 //接下来是第二次获取数据，主要是因为文档写的是第二次获取的数据才是正确的；但是根据调试，每次能获取到80个数据，也就是执行了这一次之后一共获取的是160个bit
//	 mICP->InitdataTime();
//	 mICP->Start();
//	 get_DHT22data_Byte();//改变OrginaData的算法
//	 sum = (OriginalData[0]+OriginalData[1]+OriginalData[2]+OriginalData[3]);
//	 tskmgr.DelayMs(5); //触发开始
//	 tskmgr.DelayUs(1); //触发开始
	 
	 	 sum = (OriginalData[0]+OriginalData[1]+OriginalData[2]+OriginalData[3]);
		return sum;

}





/***********************************************************************
功能：读取一组温湿度  湿度高+湿度低+温度高+温度低
       进行了和校验，正确返回1 错误返回0
       数据结果存入外联数组DHT22_data[4]
***********************************************************************/
bool AM2302::Updata(void)
{
	u8 sum;
	u16 humidity;
	u16 temperature;
	u8 humidity_integer;//保存湿度整数部分
	u8 humidity_decimals;//保存湿度小数部分
	u8 temperature_integer;//保存温度整数部分
	u8 temperature_decimals;//保存温度小数部分
	
	sum = getData();				//获取data的相关代码
	sum = getData();				//第二次获取data的相关代码
	check = OriginalData[4];
	
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
//		mCh1.Close();//关闭外部中断
	   return(1);
	}
	else
	{
//		mCh1.Close();//关闭外部中断
		return 0;
	}
}



float  AM2302::GetFloatData()
{
	return (DHT22_data[2]+((float)DHT22_data[3])/10.0);
}
 unsigned char  AM2302::Data_Hight_8()
{
		return DHT22_data[2];
}
 unsigned char  AM2302::Data_Low_8()
{
		return DHT22_data[3];
}



float  AM2302::GetFloatData_H()
{
	return (DHT22_data[0]+((float)DHT22_data[1])/10.0);
}
unsigned char  AM2302::Data_Hight_8_H()
{
	return DHT22_data[0];
}
unsigned char  AM2302::Data_Low_8_H()
{
	return DHT22_data[1];
}




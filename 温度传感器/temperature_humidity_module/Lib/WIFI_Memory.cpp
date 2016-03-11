#include "WIFI_Memory.h"

WifiMemory::WifiMemory(Memory &InfoSpace):infoSpace(InfoSpace)
{
	u8 nowPage=0;
	if(getNowPageInfo(nowPage)) //判断第一页是否存在
	{
		Sum=wifiNumber;
		while(pageNumber==1)
		{
			nowPage++;
			getNowPageInfo(nowPage);
			Sum+=wifiNumber;
		}
	}
	else{
		wifiNumber=0;
		occupancy=0;
		pageNumber=0;
		Sum=0;
		Pointer=0;
	}
}


bool WifiMemory::Save(char* name,char* password)
{
	u8 nowPage=0;
	u16 Lenth1=0,Lenth2=0,LenthSum=0;
	if(Sum==0)//从未保存过
	{
		occupancy=6;//输出存储的开始位置
		pageNumber=0;
	}
	
		while( (*name)!= '\0')
		{Lenth1++;name++;}
		
		name=name-Lenth1;
		
		while( (*password)!= '\0')
		{Lenth2++;password++;}
		
		password=password-Lenth2;
		

		
		if(Lenth1%2==0)//如果为偶数则仅仅加上两个长度位
			Lenth1+=2;
		else//如果是是奇数
			Lenth1+=3;
		
				
		if(Lenth2%2==0)//如果为偶数则仅仅加上两个长度位
			Lenth2+=2;
		else//如果是是奇数
			Lenth2+=3;
		
		LenthSum=Lenth1+Lenth2;	
	
	while(occupancy+LenthSum>=MEMORY_PAGE_SIZE ) //如果该页已满  
	{
		nowPage++;//翻页
		pageNumber=1;
		infoSpace.Write(nowPage,2,&pageNumber,1);
		getNowPageInfo(nowPage);
	}
	//这里有两种情况 一种是新页面，一种是未写满的页面
	
	wifiNumber++;
	occupancy+=LenthSum;
	//重写该页信息
	infoSpace.Write(nowPage,0,&wifiNumber,1);
	infoSpace.Write(nowPage,2,&occupancy,1);
	infoSpace.Write(nowPage,4,&pageNumber,1);
	
	occupancy-=LenthSum;
	
	infoSpace.Write(nowPage,occupancy,name);
	infoSpace.Write(nowPage,occupancy+Lenth1,password);
	
	
	return true;
}
	

bool WifiMemory::Load(char* name,char* password)
{
	return true;
}

//得到该页信息
bool WifiMemory::getNowPageInfo(u16 page)
{
	 infoSpace.Read(page,0,&wifiNumber,1);
		if(wifiNumber== 0xffff) //如果为空
		{
			wifiNumber=0;
			occupancy=7;
			pageNumber=0;
			return false;
		}
		else{
			infoSpace.Read(page,2,&occupancy,1);
			infoSpace.Read(page,4,&pageNumber,1);
			Pointer=0x07;//将读取指针放到数据开始位置
				return true;
		}
}

u16 WifiMemory::getWifiSum()
{
	 return Sum;
}

bool WifiMemory::ClearAllData()
{
	
	 return true;
}


#include "TaskManager.h"

#define DHT22_GPIOx GPIOA              
#define DHT22_GPIO_Pin_x GPIO_Pin_4


class AM2302{
	private:
		
		u8 sbuf,check;
		void DHT22_PortIN(void);
		void DHT22_PortOUT(void);
		unsigned char StartDHT22(void);
		u8 get_DHT22data_8(void);
		float UpdataInterval;
		u8 OriginalData[4];
	public:
		u8 DHT22_data[4];
		AM2302();
		u8 Updata(void);
		float GetTemperature();
		float GetHumidity();
};



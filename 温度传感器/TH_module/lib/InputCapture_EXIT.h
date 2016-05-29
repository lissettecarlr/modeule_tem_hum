#ifndef _INPUT_CAPTURE_EXIT_H_
#define _INPUT_CAPTURE_EXIT_H_

#include "stm32f10x.h"

#define Max 100
#define USE_USE_CAPTURE_AM2303_EXIT

class InputCapture_EXIT
{
protected:
	GPIO_TypeDef *mExitPort;  //GPIO Port of exit pin
	u16 mExitPin;             //GPIO pin for exit
	double mRiseTime;          //rising edge time
	double mFallTime;          //falling edge time
	double mInterval;          //time between rising edge and falling edge
	double mPPM[8];

	u8 mNumbermdataTime;

public:
	InputCapture_EXIT(GPIO_TypeDef *exitPort, u8 exitPin, u8 itGroup=3,u8 prePriority=5,u8 subPriority=0);
	void IRQ();
	float Value();

//自己添加的
	void Close();
	void Start();
	void InitdataTime();
#ifdef USE_USE_CAPTURE_AM2303_EXIT
	double mdataTime[Max];
#endif



#ifdef USE_CAPTURE_PPM_EXIT
	float ValuePPM(u8 num); //用于存储一组PPM
#endif
};



#endif

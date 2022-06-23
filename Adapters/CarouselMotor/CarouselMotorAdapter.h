//==============================================================================
#ifndef __CAROUSEL_MOTOR_ADAPTER_H
#define __CAROUSEL_MOTOR_ADAPTER_H
//==============================================================================
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
#include "MotorDriver/MotorDriver.h"
//==============================================================================
//#ifdef CAROUSEL_MOTOR_ADAPTER_USER_CONFIG
#include "CarouselMotorAdapterConfig.h" // override default macroses
//#endif
//------------------------------------------------------------------------------
#ifndef CarouselMotorResult
#define CarouselMotorResult int8_t
#endif
//------------------------------------------------------------------------------
#ifndef CarouselMotorResultAccept
#define CarouselMotorResultAccept 0
#endif
//------------------------------------------------------------------------------
#ifndef CarouselMotorResultError
#define CarouselMotorResultError -1
#endif
//------------------------------------------------------------------------------
#ifndef CarouselMotorResultBusy
#define CarouselMotorResultBusy -2
#endif
//------------------------------------------------------------------------------
#ifndef CarouselMotorResultNullPointer
#define CarouselMotorResultNullPointer -3
#endif
//------------------------------------------------------------------------------
#ifndef CarouselMotorResultInvalidParameter
#define CarouselMotorResultInvalidParameter -4
#endif
//==============================================================================
typedef struct
{
	char* Description;
	void* Parent;
	
	MotorDriverT Driver;
	
} CarouselMotorAdapterT;
//==============================================================================
CarouselMotorResult CarouselMotorAdapterInit(CarouselMotorAdapterT* adapter, void* parent);
//==============================================================================
#ifdef __cplusplus
}
#endif
//==============================================================================
#endif /* __L298_ADAPTER_H */

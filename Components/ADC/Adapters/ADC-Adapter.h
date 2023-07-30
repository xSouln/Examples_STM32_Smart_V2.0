//==============================================================================
//header:

#ifndef _ADC_ADAPTER_H_
#define _ADC_ADAPTER_H_
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif 
//==============================================================================
//includes:

#include "Peripherals/xADC/xADC.h"
#include "Common/xCircleBuffer.h"
#include "Registers/registers.h"
#include "adc.h"
//==============================================================================
//types:

typedef struct
{
	ADC_HandleTypeDef* Handle;
	REG_TIM_T* Timer;

	uint8_t ChannelsCount;

} ADC_AdapterT;
//------------------------------------------------------------------------------
typedef struct
{
	ADC_HandleTypeDef* Handle;
	REG_TIM_T* Timer;

	xADC_PointsT* PointsBuffer;

	uint16_t* PointsMemmory;
	uint32_t SizeOfPointsMemmory;

	uint8_t ChannelsCount;

} ADC_AdapterInitT;
//==============================================================================
//functions:

xResult ADC_AdapterInit(xADC_T* object, xADC_AdapterInitT* init);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif //_ADC_ADAPTER_H_

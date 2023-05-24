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

#include "Common/xADC/xADC.h"
#include "Common/xCircleBuffer.h"
#include "Registers/registers.h"
#include "adc.h"
//==============================================================================
//types:

typedef struct
{
	xADC_AdapterBaseT Base;

	ADC_HandleTypeDef* Handle;
	REG_TIM_T* Timer;

	uint8_t ChannelsCount;

} ADC_AdapterT;
//------------------------------------------------------------------------------
typedef struct
{
	ADC_AdapterT* Adapter;

	ADC_HandleTypeDef* Handle;
	REG_TIM_T* Timer;

	xADC_PointsT* PointsBuffer;

	uint16_t* PointsMemmory;
	uint32_t SizeOfPointsMemmory;

	uint8_t ChannelsCount;

} ADC_AdapterInitializationT;
//==============================================================================
//functions:

xResult ADC_AdapterInit(xADC_T* object, ADC_AdapterInitializationT* initialization);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif //_ADC_ADAPTER_H_
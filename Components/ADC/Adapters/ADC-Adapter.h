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
#include "Common/xRxReceiver.h"
#include "Common/xDataBuffer.h"
#include "Registers/registers.h"
//==============================================================================
//types:

typedef struct
{
	xADC_AdapterBaseT Base;

} ADC_AdapterT;
//==============================================================================
//functions:

xResult ADC_AdapterInit(xADC_T* object, ADC_AdapterT* adapter);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif //_ADC_ADAPTER_H_

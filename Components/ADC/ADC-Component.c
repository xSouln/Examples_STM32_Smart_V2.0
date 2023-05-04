//==============================================================================
//includes:

#include "Components.h"
#include "Adapters/ADC-Adapter.h"
//==============================================================================
//defines:


//==============================================================================
//variables:


xADC_T mADC;
//==============================================================================
//import:


//==============================================================================
//functions:

static void EventListener(xPortT* port, xPortSysEventSelector selector, void* arg)
{
	switch((int)selector)
	{
		default: break;
	}
}
//------------------------------------------------------------------------------

static xResult RequestListener(xPortT* port, xPortSysRequestSelector selector, void* arg)
{
	switch ((uint8_t)selector)
	{
		default: return xResultRequestIsNotFound;
	}

	return xResultAccept;
}
//------------------------------------------------------------------------------

void ADC_ComponentIRQ()
{

}
//------------------------------------------------------------------------------
//component functions:
/**
 * @brief main handler
 */
void ADC_ComponentHandler()
{

}
//------------------------------------------------------------------------------
/**
 * @brief time synchronization of time-dependent processes
 */
void ADC_ComponentTimeSynchronization()
{

}

//==============================================================================
//initialization:

static ADC_AdapterT Private_ADC_Adapter =
{

};
//------------------------------------------------------------------------------

static xADC_SysInterfaceT Private_ADC_SysInterface =
{
	.RequestListener = (xADC_SysRequestListenerT)RequestListener,
	.EventListener = (xADC_SysEventListenerT)EventListener
};
//==============================================================================
//component initialization:

xResult ADC_ComponentInit(void* parent)
{
	ADC_AdapterInit(&mADC, &Private_ADC_Adapter);
	xADC_Init(&mADC, parent, &Private_ADC_SysInterface);
  
	return xResultAccept;
}
//==============================================================================


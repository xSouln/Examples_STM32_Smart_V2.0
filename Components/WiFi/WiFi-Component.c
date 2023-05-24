//==============================================================================
//includes:

#include "Components.h"
#include "WiFi-Component.h"
#include "WiFi/Adapters/WiFi-Adapter.h"
//==============================================================================
//defines:


//==============================================================================
//variables:


//==============================================================================
//import:


//==============================================================================
//functions:

static void EventListener(xWiFi_T* wifi, xWiFi_SysEventSelector selector, void* arg)
{
	switch((int)selector)
	{
		default: break;
	}
}
//------------------------------------------------------------------------------

static xResult RequestListener(xWiFi_T* wifi, xWiFi_SysRequestSelector selector, void* arg)
{
	switch ((uint8_t)selector)
	{
		default: return xResultRequestIsNotFound;
	}

	return xResultAccept;
}

//------------------------------------------------------------------------------
//component functions:
/**
 * @brief main handler
 */
void WiFi_ComponentHandler()
{
	xWiFi_Handler(&mWiFi);
}
//==============================================================================
//initialization:


static xWiFi_SysInterfaceT Private_WiFi_SysInterface =
{
	.RequestListener = (xWiFi_SysRequestListenerT)RequestListener,
	.EventListener = (xWiFi_SysEventListenerT)EventListener
};

static WiFi_AdapterT WiFi_Adapter;

xWiFi_T mWiFi;
//==============================================================================
//component initialization:

xResult WiFi_ComponentInit(void* parent)
{
	WiFi_AdapterInitializationT adapter_init;
	WiFi_AdapterInit(&mWiFi, &WiFi_Adapter, &adapter_init);

	xWiFi_InitializationT init =
	{
		.Parent = parent,
		.Interface = &Private_WiFi_SysInterface
	};
	xWiFi_Init(&mWiFi, &init);

	return xResultAccept;
}
//==============================================================================


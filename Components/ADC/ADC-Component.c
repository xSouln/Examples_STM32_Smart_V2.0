//==============================================================================
//includes:

#include "Components.h"
#include "Adapters/ADC-Adapter.h"
#include "Common/xADC/Communications/xADC-RxTransactions.h"
//==============================================================================
//defines:


//==============================================================================
//variables:

struct
{
	xADC_TransactionEventNewPointsT Header;
	uint16_t Buffer[64 * 3];

} PointsPacket;
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
//component functions:
/**
 * @brief main handler
 */
void ADC_ComponentHandler()
{
	xADC_Handler(&mADC);

	if (mADC.NotifiedChannels)
	{
		int number_of_points = xADC_GetNumberOfNewPoints(&mADC);

		if (!mADC.IsEnable)
		{
			return;
		}

		if (number_of_points >= 64)
		{
			number_of_points = xCircleBufferReadObject(&mADC.Points->Buffer, PointsPacket.Buffer, 64, 0, 0);

			if (number_of_points == 64)
			{
				PointsPacket.Header.Channels = 0x7;
				PointsPacket.Header.PointsCount = 64;
				PointsPacket.Header.PointSize = SIZE_ELEMENT(PointsPacket.Buffer);

				xRxTransactionTransmitEvent(&UsartPort,
						mADC.Base.Description->ObjectId,
						TRANSACTION_EVENT_NEW_POTINTS,
						&PointsPacket, sizeof(PointsPacket));
			}
		}
	}
}

//==============================================================================
//initialization:

static ADC_AdapterT Private_ADC_Adapter;
//------------------------------------------------------------------------------

static xADC_SysInterfaceT Private_ADC_SysInterface =
{
	.RequestListener = (xADC_SysRequestListenerT)RequestListener,
	.EventListener = (xADC_SysEventListenerT)EventListener
};
//------------------------------------------------------------------------------
static uint16_t PrivatePointsMemory[sizeof(uint16_t) * 3 * 256];

static xADC_PointsT PrivatePoints;
//------------------------------------------------------------------------------
xADC_T mADC;
//==============================================================================
//component initialization:

xResult ADC_ComponentInit(void* parent)
{
	ADC_AdapterInitializationT adapter_init =
	{
		.Adapter = &Private_ADC_Adapter,

		.PointsBuffer = &PrivatePoints,
		.PointsMemmory = PrivatePointsMemory,
		.SizeOfPointsMemmory = sizeof(PrivatePointsMemory),

		.Handle = &hadc1,
		.Timer = (void*)TIM3,
		.ChannelsCount = 3
	};

	ADC_AdapterInit(&mADC, &adapter_init);

	xADC_InitializationT adc_init =
	{
		.Parent = parent,
		.Number = 1,
		.Interface = &Private_ADC_SysInterface
	};

	xADC_Init(&mADC, &adc_init);

	xADC_Start(&mADC);
  
	return xResultAccept;
}
//==============================================================================


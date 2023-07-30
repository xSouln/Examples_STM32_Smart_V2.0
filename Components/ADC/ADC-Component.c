//==============================================================================
//includes:

#include "ADC-Component.h"
#include "Adapters/ADC-Adapter.h"
#include "Peripherals/xADC/Communications/xADC-RxTransactions.h"
//==============================================================================
//defines:


//==============================================================================
//variables:

struct
{
	xADC_TransactionEventNewPointsT Header;
	uint16_t Buffer[ADC_NUMBER_OF_CHANNELS * ADC_POINTS_IN_PACKET];

} PointsPacket;
//==============================================================================
//import:


//==============================================================================
//functions:

static void EventListener(xADC_T* adc, xADC_EventSelector selector, void* arg)
{
	switch((int)selector)
	{
		default: break;
	}
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

		if (number_of_points > 0)
		{
			number_of_points = xCircleBufferReadObject(&mADC.Points->Buffer, PointsPacket.Buffer, ADC_POINTS_IN_PACKET, 0, 0);

			if (number_of_points <= 0)
			{
				return;
			}

			PointsPacket.Header.Channels = 0x7;
			PointsPacket.Header.PointsCount = number_of_points;
			PointsPacket.Header.PointSize = SIZE_ELEMENT(PointsPacket.Buffer);

			xArgT args[] =
			{
				{
					.Element = &PointsPacket,
					.Size = sizeof(PointsPacket.Header)
				},
				{
					.Element = &PointsPacket.Buffer,
					.Size = mADC.Points->Buffer.TypeSize * number_of_points
				}
			};

			xRxTransactionTransmitEvent(mADC.EventPort,
					mADC.Base.Description->ObjectId,
					TRANSACTION_EVENT_NEW_POTINTS,
					args, 2);
		}
	}
}

//==============================================================================
//initialization:

static ADC_AdapterT Private_ADC_Adapter;
//------------------------------------------------------------------------------
static uint16_t PrivatePointsMemory[sizeof(uint16_t) * ADC_NUMBER_OF_CHANNELS * ADC_POINTS_PER_CHANNEL];
static xADC_PointsT PrivatePoints;
//------------------------------------------------------------------------------
xADC_T mADC;
//==============================================================================
//component initialization:

xResult ADC_ComponentInit(void* parent)
{
	ADC_AdapterInitT adapterInit =
	{
		.PointsBuffer = &PrivatePoints,
		.PointsMemmory = PrivatePointsMemory,
		.SizeOfPointsMemmory = sizeof(PrivatePointsMemory),

		.Handle = &hadc1,
		.Timer = (void*)TIM3,
		.ChannelsCount = ADC_NUMBER_OF_CHANNELS
	};

	xADC_InitT init =
	{
		.Parent = parent,
		.Number = xADC1,
		.EventListener = (xADC_EventListenerT)EventListener,

		.AdapterInit =
		{
			.Adapter = &Private_ADC_Adapter,
			.Init = &adapterInit,
			.Initializer = ADC_AdapterInit
		}
	};

	xADC_Init(&mADC, &init);

	xADC_Start(&mADC);
  
	return xResultAccept;
}
//==============================================================================


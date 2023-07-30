//==============================================================================
//includes:

#include "Components.h"
#include "main.h"
#include "Templates/Adapters/Terminal-TransferLayer/Terminal-TxTransferLayerAdapter.h"
#include "Templates/Adapters/Terminal-TransferLayer/Terminal-RxTransferLayerAdapter.h"
#include "Abstractions/xTxRequest/xTxRequest.h"
//==============================================================================
//defines:

#define TX_DATA "dvmoqwhgolSDJVIOSionpmpcoKQFEWJGUBETBVNAIMEO,CSS{lcspJHBRYEVIU5GHI	PockoZMCL{LlOJFUWRBVETQIUVNOMPOM,Cp[ewpomgo	iwrngojMV\r"
//==============================================================================
//variables:

static uint32_t led_toggle_time_stamp;
static uint32_t sntp_update_time_stamp;

xTransferLayerT TxTransferLayer;
xTransferLayerT RxTransferLayer;

char tx_transfer_data[] = TX_DATA;
char rx_transfer_data[sizeof(tx_transfer_data)];

char tx_request0_data[] = "Request 0: " TX_DATA;
char tx_request1_data[] = "Request 1: " TX_DATA;
//------------------------------------------------------------------------------
xTxRequestControlT TxRequestControl;

static xResult TxRequestAccept(xRxRequestManagerT* manager, uint8_t* data, uint16_t size)
{
	return xResultAccept;
}

static const xRxRequestT TxRequestResponse[] =
{
	NEW_RX_REQUEST0("Request 0: Accept", TxRequestAccept),
	NEW_RX_REQUEST0("Request 1: Accept", TxRequestAccept),
	{ 0 }
};

xTxRequestT TxRequest0 =
{
	.Data = tx_request0_data,
	.DataSize = sizeof_str(tx_request0_data),
	.TimeOut = 300,
	.AttemptsCount = 3,
	.Rx = TxRequestResponse
};

xTxRequestT TxRequest1 =
{
	.Data = tx_request1_data,
	.DataSize = sizeof_str(tx_request1_data),
	.TimeOut = 300,
	.AttemptsCount = 3,
	.Rx = TxRequestResponse
};
//==============================================================================
//functions:

static void PrivateTerminalComponentEventListener(TerminalT* terminal, TerminalSysEventSelector selector, void* arg)
{
	switch((int)selector)
	{
		case TerminalSysEventTime_1000ms:
			//xTxTransferSetTxLine(&Terminal.Transfer, &SerialPortUART.Tx);
			//xTxTransferStart(&Terminal.Transfer, "qwerty", 6);
			break;

		default: break;
	}
}
//==============================================================================
//default functions:

xResult ComponentsRequestListener(ObjectBaseT* object, int selector, void* arg)
{
	return xResultNotSupported;
}
//------------------------------------------------------------------------------
void ComponentsEventListener(ObjectBaseT* object, int selector, void* arg)
{
	if (object->Description->Key != OBJECT_DESCRIPTION_KEY)
	{
		return;
	}

	switch(object->Description->ObjectId)
	{
		case TERMINAL_OBJECT_ID:
			PrivateTerminalComponentEventListener((TerminalT*)object, selector, arg);
			break;
	}
}
//------------------------------------------------------------------------------
/**
 * @brief main handler
 */
void ComponentsHandler()
{
	UsartPortComponentHandler();
	TerminalComponentHandler();
	ADC_ComponentHandler();

	xTxRequestHandler(&TxRequestControl);

	xTransferLayerHandler(&TxTransferLayer);
	xTransferLayerHandler(&RxTransferLayer);

	if (xSystemGetTime(ComponentsHandler) - led_toggle_time_stamp > 999)
	{
		led_toggle_time_stamp = xSystemGetTime(ComponentsHandler);

		if(TxRequest0.State == xTxRequesStateIdle)
		{
			xTxRequestSetPort(&TxRequest0, &SerialPort);
			xTxRequestAdd(&TxRequestControl, &TxRequest0);
		}

		if(TxRequest1.State == xTxRequesStateIdle)
		{
			xTxRequestSetPort(&TxRequest1, &SerialPort);
			xTxRequestAdd(&TxRequestControl, &TxRequest1);
		}

		//xTransferLayerStrartTransfer(&RxTransferLayer, rx_transfer_data, sizeof_str(rx_transfer_data));
		//xTransferLayerStrartTransfer(&TxTransferLayer, tx_transfer_data, sizeof_str(tx_transfer_data));

		LED1_GPIO_Port->ODR ^= LED1_Pin;
	}

	if (xSystemGetTime(ComponentsHandler) - sntp_update_time_stamp > 9999)
	{
		sntp_update_time_stamp = xSystemGetTime(ComponentsHandler);
	}
}
//------------------------------------------------------------------------------
/**
 * @brief time synchronization of time-dependent processes
 */
void ComponentsTimeSynchronization()
{
	TerminalComponentTimeSynchronization();
	UsartPortComponentTimeSynchronization();
	ADC_ComponentTimeSynchronization();
}
//------------------------------------------------------------------------------
static xResult RxRequest0ReceiveData(xRxRequestManagerT* manager, uint8_t* data, uint16_t size)
{
	const char response[] = "Request 0: Accept\r";

	xDataBufferAdd(manager->ResponseBuffer, (void*)response, sizeof_str(response));

	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult RxRequest1ReceiveData(xRxRequestManagerT* manager, uint8_t* data, uint16_t size)
{
	const char response[] = "Request 1: Accept\r";

	xDataBufferAdd(manager->ResponseBuffer, (void*)response, sizeof_str(response));

	return xResultAccept;
}
//==============================================================================
//initialization:

TerminalTxTransferLayerAdapterT TerminalTxTransferLayerAdapter =
{
	.HeaderTransferStart = "HeaderTransferStart",
	.HeaderTransfer = "HeaderTransfer :",
	.HeaderTransferEnd = "HeaderTransferEnd\r"
};
//------------------------------------------------------------------------------
TerminalRxTransferLayerAdapterT TerminalRxTransferLayerAdapter;

//------------------------------------------------------------------------------
static const xRxRequestT RxRequests[] =
{
	NEW_RX_REQUEST0("Request 0: ", RxRequest0ReceiveData),
	NEW_RX_REQUEST0("Request 1: ", RxRequest1ReceiveData),

	{ 0 }
};

static TerminalObjectT TerminalObject =
{
	.Requests = RxRequests
};
//------------------------------------------------------------------------------
/**
 * @brief initializing the component
 * @param parent binding to the parent object
 * @return int
 */
xResult ComponentsInit(void* parent)
{
	TerminalComponentInit(parent);

	xSystemInit(parent);
	UsartPortComponentInit(parent);
	ADC_ComponentInit(parent);

	xTxRequestControlInit(&TxRequestControl, parent);
	TerminalAddObject(&TxRequestControl.TerminalObject);

	TerminalTxTransferLayerAdapterInitT txTransferLayerAdapterInit =
	{
		.HeaderTransferStart = "HeaderTransferStart",
		.HeaderTransfer = "HeaderTransfer :",
		.HeaderTransferEnd = "HeaderTransferEnd\r"
	};

	xTransferLayerInitT transferLayerInit;
	transferLayerInit.Parent = parent;
	transferLayerInit.AdapterInit.Adapter = &TerminalTxTransferLayerAdapter;
	transferLayerInit.AdapterInit.Init = &txTransferLayerAdapterInit;
	transferLayerInit.AdapterInit.Initializer = TerminalTxTransferLayerAdapterInit;

	xTransferLayerInit(&TxTransferLayer, &transferLayerInit);
	xTransferLayerSetBinding(&TxTransferLayer, &SerialPort);

	TerminalRxTransferLayerAdapterInitT rxTransferLayerAdapterInit;

	transferLayerInit.AdapterInit.Adapter = &TerminalRxTransferLayerAdapter;
	transferLayerInit.AdapterInit.Init = &rxTransferLayerAdapterInit;
	transferLayerInit.AdapterInit.Initializer = TerminalRxTransferLayerAdapterInit;

	xTransferLayerInit(&RxTransferLayer,  &transferLayerInit);
	xTransferLayerSetBinding(&RxTransferLayer, &SerialPort);

	TerminalAddObjectList(&TxTransferLayer.Objects);
	TerminalAddObjectList(&RxTransferLayer.Objects);

	TerminalRemoveObjectList(&TxTransferLayer.Objects);
	TerminalRemoveObjectList(&RxTransferLayer.Objects);

	TerminalAddObjectList(&TxTransferLayer.Objects);
	TerminalAddObjectList(&RxTransferLayer.Objects);

	TerminalRemoveObjectList(&TxTransferLayer.Objects);
	TerminalAddObjectList(&TxTransferLayer.Objects);

	TerminalAddObject(&TerminalObject);

	return xResultAccept;
}
//==============================================================================

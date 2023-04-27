//==============================================================================
//includes:

#include "Components.h"
#include "xTransferLayer/Adapters/Terminal-TxTransferLayerAdapter.h"
#include "xTransferLayer/Adapters/Terminal-RxTransferLayerAdapter.h"
#include "Common/xTxRequest/xTxRequest.h"
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
//------------------------------------------------------------------------------
static xResult PrivateTransferLayerRequestListener(xTransferLayerT* layer, xTransferLayerSysRequestSelector selector, void* arg)
{
	switch((int)selector)
	{
		case xTransferLayerSysRequestGetTime:
			*(uint32_t*)arg = ComponentsSysGetTime();
			break;

		default: return xResultNotSupported;
	}

	return xResultAccept;
}
//------------------------------------------------------------------------------
#ifdef SERIAL_PORT_UART_COMPONENT_ENABLE
static void PrivateSerialPortComponentEventListener(SerialPortT* port, SerialPortEventSelector selector, void* arg)
{
	switch((int)selector)
	{
		case SerialPortEventEndLine:
			TerminalReceiveData(&port->Rx,
								((SerialPortReceivedDataT*)arg)->Data,
								((SerialPortReceivedDataT*)arg)->Size);
			break;

		case SerialPortEventBufferIsFull:
			TerminalReceiveData(&port->Rx,
								((SerialPortReceivedDataT*)arg)->Data,
								((SerialPortReceivedDataT*)arg)->Size);
			break;

		default: break;
	}
}
#endif
//==============================================================================
//default functions:

void ComponentsEventListener(ObjectBaseT* object, int selector, void* arg)
{
	if (object->Description->Key != OBJECT_DESCRIPTION_KEY)
	{
		return;
	}

	switch(object->Description->ObjectId)
	{
#ifdef SERIAL_PORT_UART_COMPONENT_ENABLE
		case SERIAL_PORT_OBJECT_ID:
			PrivateSerialPortComponentEventListener((SerialPortT*)object, selector, arg);
			break;
#endif

		case TERMINAL_OBJECT_ID:
			PrivateTerminalComponentEventListener((TerminalT*)object, selector, arg);
			break;
	}
}
//------------------------------------------------------------------------------

xResult ComponentsRequestListener(ObjectBaseT* object, int selector, void* arg)
{
	switch((int)selector)
	{
		case xSystemRequestGetTime:
			*(uint32_t*)arg = ComponentsSysGetTime();
			break;

		default: return xResultNotSupported;
	}

	return xResultAccept;
}
//------------------------------------------------------------------------------
xResult xSystemRequestListener(ObjectBaseT* object, int selector, void* arg)
{
	switch((int)selector)
	{
		case xSystemRequestGetTime:
			*(uint32_t*)arg = HAL_GetTick();
			break;

		default: return xResultNotSupported;
	}

	return xResultAccept;
}
//------------------------------------------------------------------------------
/**
 * @brief main handler
 */
void ComponentsHandler()
{
	UsartPortComponentHandler();
	TCPServerWIZspiComponentHandler();
	TerminalComponentHandler();

	xTxRequestHandler(&TxRequestControl);

	xTransferLayerHandler(&TxTransferLayer);
	xTransferLayerHandler(&RxTransferLayer);

	if (ComponentsSysGetTime() - led_toggle_time_stamp > 999)
	{
		led_toggle_time_stamp = ComponentsSysGetTime();

		if(TxRequest0.State == xTxRequesStateIdle)
		{
			xTxRequestSetPort(&TxRequest0, &UsartPort);
			xTxRequestAdd(&TxRequestControl, &TxRequest0);
		}

		if(TxRequest1.State == xTxRequesStateIdle)
		{
			xTxRequestSetPort(&TxRequest1, &UsartPort);
			xTxRequestAdd(&TxRequestControl, &TxRequest1);
		}

		//xTransferLayerStrartTransfer(&RxTransferLayer, rx_transfer_data, sizeof_str(rx_transfer_data));
		//xTransferLayerStrartTransfer(&TxTransferLayer, tx_transfer_data, sizeof_str(tx_transfer_data));

		LED1_GPIO_Port->ODR ^= LED1_Pin;
	}

	if (ComponentsSysGetTime() - sntp_update_time_stamp > 9999)
	{
		sntp_update_time_stamp = ComponentsSysGetTime();
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
	TCPServerWIZspiComponentTimeSynchronization();
}
//------------------------------------------------------------------------------

uint32_t ComponentsSysGetTime()
{
	return HAL_GetTick();
}
//------------------------------------------------------------------------------

void ComponentsSysDelay(uint32_t time)
{
	HAL_Delay(time);
}
//------------------------------------------------------------------------------

void ComponentsTrace(char* text)
{

}
//------------------------------------------------------------------------------

void ComponentsSysEnableIRQ()
{

}
//------------------------------------------------------------------------------

void ComponentsSysDisableIRQ()
{

}
//------------------------------------------------------------------------------

void ComponentsSysReset()
{

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
	//SerialPortUARTComponentInit(parent);
	UsartPortComponentInit(parent);
	TCPServerWIZspiComponentInit(parent);

	xTxRequestControlInit(&TxRequestControl, parent, 0);
	TerminalAddObject(&TxRequestControl.TerminalObject);

	TerminalTxTransferLayerAdapterInit(&TxTransferLayer, &TerminalTxTransferLayerAdapter);
	xTransferLayerInit(&TxTransferLayer, parent, 0);
	xTransferLayerSetBinding(&TxTransferLayer, &UsartPort);

	TerminalRxTransferLayerAdapterInit(&RxTransferLayer, &TerminalRxTransferLayerAdapter);
	xTransferLayerInit(&RxTransferLayer, parent, 0);
	xTransferLayerSetBinding(&RxTransferLayer, &UsartPort);

	TerminalAddObjectList(&TxTransferLayer.Objects);
	TerminalAddObjectList(&RxTransferLayer.Objects);

	TerminalRemoveObjectList(&TxTransferLayer.Objects);
	TerminalRemoveObjectList(&RxTransferLayer.Objects);

	TerminalAddObjectList(&TxTransferLayer.Objects);
	TerminalAddObjectList(&RxTransferLayer.Objects);

	TerminalRemoveObjectList(&TxTransferLayer.Objects);
	TerminalAddObjectList(&TxTransferLayer.Objects);

	TerminalAddObject(&TerminalObject);

	//TerminalTxBind(&SerialPortUART.Tx);

	return xResultAccept;
}
//==============================================================================

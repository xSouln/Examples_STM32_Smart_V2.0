//==============================================================================
//includes:

#include "Components.h"
//==============================================================================
//variables:

static uint8_t time1_ms;
static uint8_t time5_ms;

static uint32_t led_toggle_time_stamp;
static uint32_t sntp_update_time_stamp;
//==============================================================================
//functions:

static void PrivateTerminalComponentEventListener(TerminalT* terminal, TerminalEventSelector selector, void* arg, ...)
{
	switch((int)selector)
	{
		case TerminalEventTime_1000ms:
			xTxTransferSetTxLine(&Terminal.Transfer, &SerialPortUART.Tx);
			xTxTransferStart(&Terminal.Transfer, "qwerty", 6);
			break;

		default: break;
	}
}
//------------------------------------------------------------------------------
static void PrivateSerialPortComponentEventListener(SerialPortT* port, SerialPortEventSelector selector, void* arg, ...)
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
//==============================================================================
//default functions:

void ComponentsEventListener(ComponentObjectBaseT* object, int selector, void* arg, ...)
{
	if (object->Description->Key != OBJECT_DESCRIPTION_KEY)
	{
		return;
	}

	switch(object->Description->ObjectId)
	{
		case SERIAL_PORT_OBJECT_ID:
			PrivateSerialPortComponentEventListener((SerialPortT*)object, selector, arg);
			break;

		case TERMINAL_OBJECT_ID:
			PrivateTerminalComponentEventListener((TerminalT*)object, selector, arg);
			break;
	}
}
//------------------------------------------------------------------------------

void ComponentsRequestListener(ComponentObjectBaseT* port, int selector, void* arg, ...)
{
	switch((int)selector)
	{
		default: break;
	}
}
//------------------------------------------------------------------------------
/**
 * @brief main handler
 */
void ComponentsHandler()
{
	if (!time1_ms)
	{
		time1_ms = 1;
	}

	if (!time5_ms)
	{
		time5_ms = 5;

		SerialPortUARTComponentHandler();
		TerminalComponentHandler();
	}

	if (ComponentsSysGetTime() - led_toggle_time_stamp > 999)
	{
		led_toggle_time_stamp = ComponentsSysGetTime();

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
	SerialPortUARTComponentTimeSynchronization();

	if (time5_ms)
	{
		time5_ms--;
	}
}
//------------------------------------------------------------------------------

void ComponentsSystemDelay(ComponentObjectBaseT* object, uint32_t time)
{
	HAL_Delay(time);
}
//------------------------------------------------------------------------------

void ComponentsTrace(char* text)
{

}
//------------------------------------------------------------------------------

void ComponentsSystemEnableIRQ()
{

}
//------------------------------------------------------------------------------

void ComponentsSystemDisableIRQ()
{

}
//------------------------------------------------------------------------------

void ComponentsSystemReset()
{

}
//------------------------------------------------------------------------------

uint32_t ComponentsSystemGetTime()
{
	return HAL_GetTick();
}
//------------------------------------------------------------------------------
/**
 * @brief initializing the component
 * @param parent binding to the parent object
 * @return int
 */
xResult ComponentsInit(void* parent)
{
	TerminalComponentInit(parent);
	SerialPortUARTComponentInit(parent);
	TerminalTxBind(&SerialPortUART.Tx);

	return xResultAccept;
}
//==============================================================================

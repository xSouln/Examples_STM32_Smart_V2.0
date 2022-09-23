//==============================================================================
#include "Common/xMemory.h"
#include "SerialPort.h"
//==============================================================================
void SerialPort_Handler(SerialPortT* serial_port)
{
	xRxHandler(&serial_port->Rx);
  xTxHandler(&serial_port->Tx);
}
//------------------------------------------------------------------------------
SerialPortResult SerialPort_Init(SerialPortT* serial_port, void* parent)
{
	if (serial_port)
	{
		serial_port->Description = "SerialPortT";
		serial_port->Parent = parent;
	}
	return SerialPortResultError;
}
//==============================================================================

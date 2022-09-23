//==============================================================================
#include "SerialPortUART_Adapter.h"
#include "SerialPortUART_TxAdapter.h"
#include "SerialPortUART_RxAdapter.h"
//==============================================================================
int SerialPortUART_AdapterInit(SerialPortT* serial_port, SerialPortUART_AdapterT* adapter)
{
	serial_port->Adapter = adapter;
	
	SerialPortUART_TxAdapterInit(&serial_port->Tx, serial_port, adapter);
	SerialPortUART_RxAdapterInit(&serial_port->Rx, serial_port, adapter);
	
	serial_port->Rx.Tx = &serial_port->Tx;
  
  return SerialPortAdapterResultAccept;
}
//==============================================================================

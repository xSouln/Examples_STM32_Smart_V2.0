//==============================================================================
//header:

#ifndef SERIAL_PORT_UART_COMPONENT_H
#define SERIAL_PORT_UART_COMPONENT_H
//==============================================================================
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "SerialPort_UART_ComponentConfig.h"
#include "SerialPort/Controls/SerialPort.h"
//==============================================================================
//defines:


//==============================================================================
//macros:


//==============================================================================
//functions:

xResult _SerialPortUARTComponentInit(void* parent);

void _SerialPortUARTComponentHandler();
void _SerialPortUARTComponentTimeSynchronization();
void _SerialPortUARTComponentIRQListener();
//==============================================================================
//import:


//==============================================================================
//override:

#define SerialPortUARTComponentInit(parent) _SerialPortUARTComponentInit(parent)

#define SerialPortUARTComponentHandler() SerialPortHandler(&SerialPortUART)
#define SerialPortUARTComponentTimeSynchronization() SerialPortTimeSynchronization(&SerialPortUART)

#define SerialPortUARTComponentIRQListener() SerialPortIRQListener(&SerialPortUART)
//==============================================================================
//export:

extern SerialPortT SerialPortUART;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif //SERIAL_PORT_UART_COMPONENT_H

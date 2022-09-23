//==============================================================================
#ifndef SERIAL_PORT_COMPONENTS_H
#define SERIAL_PORT_COMPONENTS_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "SerialPort/Controls/SerialPort.h"
//==============================================================================
extern SerialPortT SerialPortUART;
extern SerialPortT SerialPortUSB;

int SerialPortUART_ComponentInit(void* parent);
int SerialPortUSB_ComponentInit(void* parent);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* SERIAL_PORT_UART_COMPONENT_H */

//==============================================================================
#ifndef SERIAL_PORT_UART_RX_ADAPTER_H
#define SERIAL_PORT_UART_RX_ADAPTER_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "SerialPortUART_AdapterTypes.h"
//==============================================================================
int SerialPortUART_RxAdapterInit(xRxT* rx, void* parent, SerialPortUART_AdapterT* adapter);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* SERIAL_PORT_UART_RX_ADAPTER_H */


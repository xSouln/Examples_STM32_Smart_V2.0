//==============================================================================
#ifndef SERIAL_PORT_USB_ADAPTER_H
#define SERIAL_PORT_USB_ADAPTER_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "SerialPortUSB_AdapterTypes.h"
#include "SerialPort/SerialPort_Types.h"
//==============================================================================
SerialPortAdapterResult SerialPortUSB_AdapterInit(SerialPortT* serial_port,
																										SerialPortUSB_AdapterT* adapter);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* SERIAL_PORT_USB_ADAPTER_H */


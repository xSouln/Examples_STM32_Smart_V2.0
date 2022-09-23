//==============================================================================
#ifndef SERIAL_PORT_USB_TX_ADAPTER_H
#define SERIAL_PORT_USB_TX_ADAPTER_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "SerialPortUSB_AdapterTypes.h"
//==============================================================================
int SerialPortUSB_TxAdapterInit(xTxT* tx, void* parent, SerialPortUSB_AdapterT* adapter);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* SERIAL_PORT_USB_TX_ADAPTER_H */


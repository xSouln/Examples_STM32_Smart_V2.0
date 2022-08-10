//==============================================================================
#ifndef _USB_SERIAL_PORT_H_
#define _USB_SERIAL_PORT_H_
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include <stdint.h>
#include <stdbool.h>
#include "Common/xType.h"
#include "Common/xRx.h"
#include "Common/xTx.h"
#include "usbd_cdc.h"
//==============================================================================
typedef struct
{
  OBJECT_HEADER;

  xRxT *Rx;
  xTxT *Tx;
  
  USBD_CDC_HandleTypeDef* Handle;

} USBSerialPortT;
//==============================================================================
extern USBSerialPortT USBSerialPort;
//==============================================================================
int USBSerialPortInit(void* parent);
void USBSerialPortRxHandler();

int8_t USBSerialPortTransmit(uint8_t* buf, uint16_t len);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* _USB_SERIAL_PORT_H_ */

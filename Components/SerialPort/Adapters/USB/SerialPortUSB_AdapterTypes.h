//==============================================================================
#ifndef SERIAL_PORT_USB_ADAPTER_TYPES_H
#define SERIAL_PORT_USB_ADAPTER_TYPES_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "SerialPort/Adapters/SerialPort_AdaptersTypes.h"
#include "SerialPortUSB_AdapterInfo.h"
#include "SerialPortUSB_AdapterConfig.h"
#include "usbd_cdc_if.h"
//==============================================================================
typedef struct
{
	USBD_CDC_ItfTypeDef USBD_CDC_Itf;
	USBD_CDC_HandleTypeDef* Handle;
	USBD_HandleTypeDef* hUsbDeviceFS;
	
	xDataBufferT* TxDataBuffer;
	
	xCircleBufferT RxCircleBuffer;
	xRxReceiverT RxReceiver;
	
	xCircleBufferT TxCircleBuffer;
	  
} SerialPortUSB_AdapterT;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* SERIAL_PORT_USB_ADAPTER_TYPES_H */
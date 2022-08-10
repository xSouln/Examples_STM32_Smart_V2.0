//==============================================================================
#include <string.h>
#include "USBSerialPort.h"
#include "usbd_conf.h"
#include "stm32f1xx_hal_pcd.h"
//==============================================================================
#define RX_CIRCLE_BUF_SIZE CDC_DATA_HS_OUT_PACKET_SIZE
#define RX_OBJECT_BUF_SIZE 0x1ff
#define TX_CIRCLE_BUF_SIZE_MASK 0x3ff

static uint8_t rx_circle_buf[RX_CIRCLE_BUF_SIZE];
static uint8_t rx_object_buf[RX_OBJECT_BUF_SIZE];
static uint8_t tx_circle_buf[TX_CIRCLE_BUF_SIZE_MASK + 1];

static xRxT Rx;
static xTxT Tx;

extern USBD_HandleTypeDef hUsbDeviceFS; //override at usb_device.c

USBSerialPortT USBSerialPort;
//==============================================================================
static void Handler(xTxT* tx)
{
  if (tx->TotalIndex != tx->HandlerIndex && tx->Control->GetTransmiterStatus(tx) == xTxStatusFree)
	{
		uint16_t size = (tx->TotalIndex - tx->HandlerIndex) & tx->SizeMask;
		if (tx->Control->TransmitData(tx, tx->Buffer + tx->HandlerIndex, size) == xResultAccept)
		{
			tx->HandlerIndex += size;
			tx->HandlerIndex &= tx->SizeMask;
		}
	}
}
//------------------------------------------------------------------------------
static xTxStatus GetTransmiterStatus(xTxT* tx)
{
	switch (USBSerialPort.Handle->TxState)
	{
		case 0: return xTxStatusFree;
		default: return xTxStatusTransmitted;
	}
}
//------------------------------------------------------------------------------
static xTxControlT tx_control =
{
	.GetTransmiterStatus = (xTxActionGetTransmiterStatus)GetTransmiterStatus,
	.Handler = (xTxActionHandler)Handler
};
//==============================================================================
/**
 * called when a "/r" is encountered or a buffer overflow is encountered
 * can be override
 */
/*
xRxResult USBSerialPortRxEndLine(xRxT* rx, uint8_t* obj, uint16_t size)
{
  return xRxResultAccept;
}
*/
//==============================================================================
int USBSerialPortInit(void* parent)
{
	extern xResult RxEndLine(xObject rx, uint8_t* obj, uint16_t size);
	
	if (USBSerialPort.Description) { USBSerialPort.Description = "USBSerialPortT"; }
	USBSerialPort.Parent = parent;
	
  Tx.Rx = &Rx;
  Rx.Tx = &Tx;
  
  xRxInit(&Rx, &USBSerialPort,
          rx_circle_buf, RX_CIRCLE_BUF_SIZE,
          rx_object_buf, RX_OBJECT_BUF_SIZE,
          (xRxEventEndLine)RxEndLine);
	
	tx_control.Handle = USBSerialPort.Handle;
  
  xTxInit(&Tx, &USBSerialPort,
          tx_circle_buf, TX_CIRCLE_BUF_SIZE_MASK,
          &tx_control);
  
  USBSerialPort.Rx = &Rx;
  USBSerialPort.Tx = &Tx;
	
  return RESULT_ACCEPT;
}
//==============================================================================
static int8_t USBSerialPortDriverInit()
{
  USBSerialPort.Handle = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  USBSerialPort.Handle->RxBuffer = rx_circle_buf;
  return RESULT_ACCEPT;
}
//==============================================================================
static int8_t USBSerialPortDriverDeInit()
{
  
  return RESULT_ACCEPT;
}
//==============================================================================
static int8_t USBSerialPortDriverHandler(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  
  return RESULT_ACCEPT;
}
//==============================================================================
static int8_t USBSerialPortDriverReceive(uint8_t* buf, uint32_t *len)
{
  xRxObjectReceive(USBSerialPort.Rx, buf, (uint16_t)*len);  
  HAL_PCD_EP_Receive(hUsbDeviceFS.pData, CDC_OUT_EP, rx_circle_buf, RX_CIRCLE_BUF_SIZE);
  return RESULT_ACCEPT;
}
//==============================================================================
static int8_t USBSerialPortTransmitionComplite(uint8_t *Buf, uint32_t *Len, uint8_t epnum)
{
  //USBSerialPort.Handle->TxState = RESULT_ACCEPT;
  return RESULT_ACCEPT;
}
//==============================================================================
int8_t USBSerialPortTransmit(uint8_t* buf, uint16_t len)
{
  if(USBSerialPort.Handle->TxState == 0)
  {
    USBSerialPort.Handle->TxState = RESULT_BUSY;
    HAL_PCD_EP_Transmit(hUsbDeviceFS.pData, CDC_IN_EP, buf, len);
    return RESULT_ACCEPT;
  }
  return RESULT_BUSY;
}
//==============================================================================
/**
 * must be called to process the received data
 */
void USBSerialPortRxHandler()
{
  xRxObjectUpdate(USBSerialPort.Rx);
}
//==============================================================================
/**
 * overide init structure at usb_device.c
 * initializes basic control functions
 */
USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
  USBSerialPortDriverInit,
  USBSerialPortDriverDeInit,
  USBSerialPortDriverHandler,
  USBSerialPortDriverReceive,
	//USBSerialPortTransmitionComplite
};
//==============================================================================

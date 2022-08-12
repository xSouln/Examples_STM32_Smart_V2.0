//==============================================================================
#include <string.h>
#include "stm32f1xx_hal.h"
#include "SerialPort/SerialPort.h"
//==============================================================================
#define RX_CIRCLE_BUF_SIZE_MASK 0x1ff
#define RX_OBJECT_BUF_SIZE 0x200
#define TX_CIRCLE_BUF_SIZE_MASK 0x3ff
//------------------------------------------------------------------------------
static uint8_t rx_circle_buf[RX_CIRCLE_BUF_SIZE_MASK + 1];
static uint8_t rx_object_buf[RX_OBJECT_BUF_SIZE];
static uint8_t tx_circle_buf[TX_CIRCLE_BUF_SIZE_MASK + 1];

static xRxT Rx;
static xTxT Tx;

SerialPortT SerialPort;

static volatile DMA_Channel_TypeDef* DMA_RX;
//==============================================================================
static xResult SetTransmiterState(xTxT* tx, xTxState state)
{
	switch ((uint8_t)state)
	{
		case xTxStateDisable:
			SerialPort.Reg->Control_1.TxEmptyInterruptEnable = false;
			break;
		
		case xTxStateEnable:
			SerialPort.Reg->Control_1.TxEmptyInterruptEnable = true;
			break;
	}
  return xResultAccept;
}
//------------------------------------------------------------------------------
static xTxStatus GetTransmiterStatus(xTxT* tx)
{
	return SerialPort.Reg->Control_1.TxEmptyInterruptEnable ? xTxStatusTransmitted : xTxStatusFree;
}
//------------------------------------------------------------------------------
static xTxControlT tx_control =
{
	.SetTransmiterState = (xTxActionSetTransmiterState)SetTransmiterState,
	.GetTransmiterStatus = (xTxActionGetTransmiterStatus)GetTransmiterStatus
};
//==============================================================================
int SerialPortHandler()
{
	SerialPort.Rx->CircleReceiver.TotalIndex = sizeof(rx_circle_buf) - DMA_RX->CNDTR;
	xRxObjectUpdate(SerialPort.Rx);
  xTxHandler(SerialPort.Tx);
	
  return 0;
}
//==============================================================================
int SerialPortInit(void* reg, void* parent)
{
	extern xResult RxEndLine(xObject rx, uint8_t* obj, uint16_t size);
	
	if (SerialPort.Description) { SerialPort.Description = "SerialPortT"; }
	SerialPort.Parent = parent;
	
  xRxInit(&Rx, &SerialPort,
          rx_circle_buf, RX_CIRCLE_BUF_SIZE_MASK,
          rx_object_buf, RX_OBJECT_BUF_SIZE,
          (xRxEventEndLine)RxEndLine);
  
  xTxInit(&Tx, &SerialPort,
          tx_circle_buf, TX_CIRCLE_BUF_SIZE_MASK,
					&tx_control);
  
  Tx.Rx = &Rx;
  Rx.Tx = &Tx;
  
  SerialPort.Rx = &Rx;
  SerialPort.Tx = &Tx;
	
	tx_control.Handle = reg;
	SerialPort.Reg = (volatile STM32_UART_REG_T*)reg;

  SerialPort.Reg->Control_1.ReceiverEnable = true;
	SerialPort.Reg->Control_3.DMA_EnableReceiver = true;
  //SerialPort.Reg->Control_1.RxNotEmptyInterruptEnable = true;
  
  SerialPort.Reg->Control_1.TransmitterEnable = true;
  SerialPort.Reg->Control_1.TxCompleteInterruptEnable = false;
  SerialPort.Reg->Control_1.TxEmptyInterruptEnable = false;
	
	extern UART_HandleTypeDef huart1;
	HAL_UART_Receive_DMA(&huart1, rx_circle_buf, sizeof(rx_circle_buf));
	/*
	DMA_RX = DMA1_Channel5;// huart1.hdmarx->Instance;
	
	DMA_RX->CCR &= ~DMA_CCR_EN;
	DMA_RX->CCR |= DMA_CCR_CIRC;
	DMA_RX->CCR |= DMA_CCR_MINC;
	DMA_RX->CCR |= DMA_CCR_PL_0;
	
	DMA_RX->CNDTR = sizeof(rx_circle_buf);
	DMA_RX->CPAR = (uint32_t)&SerialPort.Reg->Data;
	DMA_RX->CMAR = (uint32_t)rx_circle_buf;
	
	DMA_RX->CCR |= DMA_CCR_EN;
  */
  return 0;
}
//==============================================================================
inline void SerialPortIRQ()
{
	/*
  if(SerialPort.Reg->Status.RxNotEmpty)
  {
    SerialPort.Reg->Status.RxNotEmpty = false;
    xRxCircleReceiveByte(SerialPort.Rx, SerialPort.Reg->Data);
    return;
  }
  */
  if(SerialPort.Reg->Control_1.TxEmptyInterruptEnable && SerialPort.Reg->Status.TxEmpty)
  {
    if(SerialPort.Tx->HandlerIndex != SerialPort.Tx->TotalIndex)
    {
      SerialPort.Reg->Data = SerialPort.Tx->Buffer[SerialPort.Tx->HandlerIndex];
      SerialPort.Tx->HandlerIndex++;
      SerialPort.Tx->HandlerIndex &= SerialPort.Tx->SizeMask;
    }
    else
    {
      SerialPort.Reg->Control_1.TxEmptyInterruptEnable = false;
    }
  }
}
//==============================================================================

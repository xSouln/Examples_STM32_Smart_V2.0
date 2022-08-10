/*
 * macroces_stm32f4xx_uart.h
 *
 * Created: 16.05.2019 14:29:38
 *  Author: rekuts
 */ 
//==============================================================================
#ifndef MACROCES_STM32X4XX_UART_H_
#define MACROCES_STM32X4XX_UART_H_
//==============================================================================
#include "xRx.h"
//==============================================================================
#define UARTX_INIT(Reg)\
  Reg->SR;\
  Reg->CR3.ErrorInterruptEnable = false;\
  Reg->CR1.ReceiverEnable = true;\
  Reg->CR1.RxNotEmptyInterruptEnable = true;\
  Reg->CR1.TransmitterEnable = true;\
  Reg->CR1.TxCompleteInterruptEnable = false;\
  Reg->CR1.TxEmptyInterruptEnable = false
//==============================================================================
#define UARTX_IRQ_REF(Reg, Rx, Tx)\
  if(Reg->SR.RxNotEmpty)\
  {\
    Reg->SR.RxNotEmpty = false;\
    Rx->CircleReceiver.Buffer[Rx->CircleReceiver.TotalIndex] = Reg->DR;\
    Rx->CircleReceiver.TotalIndex++;\
    Rx->CircleReceiver.TotalIndex &= Rx->CircleReceiver.SizeMask;\
    return;\
  }\
  if(Reg->CR1.TxEmptyInterruptEnable && Reg->SR.TxEmpty)\
  {\
    if(Tx->State.HandlerIndex != Tx->State.TotalIndex)\
    {\
      Reg->DR = Tx->Buffer[Tx->State.HandlerIndex];\
      Tx->State.HandlerIndex++;\
      Tx->State.HandlerIndex &= Tx->State.SizeMask;\
    }\
    else\
    {\
      Reg->CR1.TxEmptyInterruptEnable = false;\
    }\
  }
//==============================================================================
#define UARTX_IRQ(Reg, Rx, Tx)\
  if(Reg->SR.RxNotEmpty)\
  {\
    Reg->SR.RxNotEmpty = false;\
    Rx.CircleReceiver.Buffer[Rx.CircleReceiver.TotalIndex] = Reg->DR;\
    Rx.CircleReceiver.TotalIndex++;\
    Rx.CircleReceiver.TotalIndex &= Rx.CircleReceiver.SizeMask;\
    return;\
  }\
  if(Reg->CR1.TxEmptyInterruptEnable && Reg->SR.TxEmpty)\
  {\
    if(Tx.State.HandlerIndex != Tx.State.TotalIndex)\
    {\
      Reg->DR = Tx.Buffer[Tx.State.HandlerIndex];\
      Tx.State.HandlerIndex++;\
      Tx.State.HandlerIndex &= Tx.State.SizeMask;\
    }\
    else\
    {\
      Reg->CR1.TxEmptyInterruptEnable = false;\
    }\
  }
//==============================================================================
#define UARTX_RX_HANDLER_REF(Context, Reg, Rx)\
  xRxUpdate(Context, Rx);\
  if(Reg->SR.ErrorOverRun)\
  {\
    Reg->SR;\
    Reg->SR.ErrorOverRun = false;\
  }

#define UARTX_RX_HANDLER(Context, Reg, Rx)\
  xRxUpdate(Context, &Rx);\
  if(Reg->SR.ErrorOverRun)\
  {\
    Reg->SR;\
    Reg->SR.ErrorOverRun = false;\
  }
//==============================================================================
#define UARTX_TX_HANDLER_REF(Context, Reg, Tx)\
  if(!Reg->CR1.TxEmptyInterruptEnable && Tx->State.TotalIndex != Tx->State.HandlerIndex)\
  {\
    Reg->CR1.TxEmptyInterruptEnable = true;\
  }

#define UARTX_TX_HANDLER(Context, Reg, Tx)\
  if(!Reg->CR1.TxEmptyInterruptEnable && Tx.State.TotalIndex != Tx.State.HandlerIndex)\
  {\
    Reg->CR1.TxEmptyInterruptEnable = true;\
  }
//==============================================================================
#endif /* MACROCES_STM32X4XX_UART_H_ */

//==============================================================================
#ifndef _SERIAL_PORT_UART_COMPONENT_CONFIG_H_
#define _SERIAL_PORT_UART_COMPONENT_CONFIG_H_
//==============================================================================
#ifdef __cplusplus
extern "C" {
#endif 
//==============================================================================
//includes:

#include "Components_Types.h"

//==============================================================================
//import:

extern DMA_HandleTypeDef hdma_usart1_rx;
//==============================================================================
//defines:

#define SERIAL_PORT_UART_RX_CIRCLE_BUF_SIZE_MASK 0x0ff
#define SERIAL_PORT_UART_RX_OBJECT_BUF_SIZE 0x1ff
#define SERIAL_PORT_UART_TX_CIRCLE_BUF_SIZE_MASK 0x1ff

#define SERIAL_PORT_UART_REG USART1
#define SERIAL_PORT_UART_RX_DMA hdma_usart1_rx
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif //_SERIAL_PORT_UART_COMPONENT_CONFIG_H_

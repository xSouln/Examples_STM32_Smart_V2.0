//==============================================================================
#ifndef _UART_PORT_COMPONENT_CONFIG_H_
#define _UART_PORT_COMPONENT_CONFIG_H_
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

#define UART_PORT_RX_CIRCLE_BUF_SIZE_MASK 0x0ff
#define UART_PORT_RX_OBJECT_BUF_SIZE 0x1ff
#define UART_PORT_TX_CIRCLE_BUF_SIZE_MASK 0x1ff

#define UART_PORT_REG USART1
#define UART_PORT_RX_DMA hdma_usart1_rx
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif //_UART_PORT_COMPONENT_CONFIG_H_

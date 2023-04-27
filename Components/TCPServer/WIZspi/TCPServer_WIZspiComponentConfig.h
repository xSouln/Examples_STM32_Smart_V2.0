//==============================================================================
//header:

#ifndef _TCP_SERVER_WIZ_SPI_COMPONENT_CONFIG_H_
#define _TCP_SERVER_WIZ_SPI_COMPONENT_CONFIG_H_
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
//includes:

#include "Components_Types.h"
#include "spi.h"
//==============================================================================
//defines:

#define TCP_SERVER_WIZ_SPI_COMPONENT_ENABLE 0

#define TCP_SERVER_WIZ_OPERATION_BUF_SIZE 0x200
#define TCP_SERVER_WIZ_SPI_RX_RECEIVER_BUF_SIZE 0x100

#define TCP_SERVER_WIZ_SPI_REG (REG_SPI_T*)SPI1
//==============================================================================
//macros:


//==============================================================================
//import:

//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif //_TCP_SERVER_WIZ_SPI_COMPONENT_CONFIG_H_

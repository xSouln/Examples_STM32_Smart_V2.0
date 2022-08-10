//==============================================================================
#ifndef _TCP_H_
#define _TCP_H_
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif
//==============================================================================
#include "Common/xType.h"
#include "Common/xTx.h"
#include "Common/xRx.h"
#include "Patterns/xPatterns.h"
//==============================================================================
static const uint8_t TCP_DEFAULT_IP[4] = { 192, 168, 100, 40 };
static const uint8_t TCP_DEFAULT_GETAWAY[4] = { 192, 168, 100, 1 };
static const uint8_t TCP_DEFAULT_NETMASK[4] = { 255, 255, 255, 0 };
static const uint8_t TCP_DEFAULT_MAC_ADDRESS[6] = { 0xdc, 0x31, 0x82, 0xc5, 0xfe, 0x11 };
//==============================================================================
#define TCP_DEFAULT_PORT 5000

#define WIZ_HARD_RESET_ENABLE false
#define WIZ_HARD_RESET_DISABLE (!WIZ_HARD_RESET_ENABLE)

#define TCP_PACKET_BUFFER_SIZE 0x3fff
//==============================================================================
typedef union
{
  struct
	{
    uint32_t Connected : 1;
    uint32_t IsOpen : 1;
    uint32_t Update : 1;
  };
  uint32_t Value;
	
} TCP_SockStatusT;
//==============================================================================
typedef struct
{
  TCP_SockStatusT Status;
  uint8_t Number;
  uint8_t State;
  uint16_t Port;
  uint16_t FreeSize;
	
} TCP_SockT;
//==============================================================================
typedef struct
{
  uint8_t Ip[4];
  uint8_t Gateway[4];
  uint8_t Subnet [4];
  uint8_t Mac[6];
	
} TCP_InfoT;
//==============================================================================
typedef struct
{
	OBJECT_HEADER;
	
  TCP_SockT Sock;
  TCP_InfoT Info;
  
  xTxT Tx;
  xRxT Rx;
  
  volatile uint32_t Delay;
  
  volatile STM32_SPI_REG_T *SPI;
  
} TCP_T;
//==============================================================================
extern TCP_T TCP;
//==============================================================================
int TCP_Init(void* spi_reg, void* parent);

extern void TCP_RX_HANDLER();
extern void TCP_TX_HANDLER();
extern void TCP_CONNECTION_HANDLER();

xResult TCP_TransmitData(xObject data, uint16_t size);
xResult TCP_TransmitString(char* string);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* _TCP_H_ */

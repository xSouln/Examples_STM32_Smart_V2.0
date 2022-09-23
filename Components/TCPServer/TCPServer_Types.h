//==============================================================================
#ifndef TCP_SERVER_TYPES_H
#define TCP_SERVER_TYPES_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "main.h"
#include "Common/xTx.h"
#include "Common/xRx.h"
#include "TCPServer_Config.h"
#include "TCPServer_Info.h"
#include "w5500.h"
#include "socket.h"
//==============================================================================
typedef enum
{
	TCPServerResultAccept,
	TCPServerResultError
	
} TCPServerResult;
//------------------------------------------------------------------------------
typedef enum
{
	TCPServerUpdateState = 1U,
	
} TCPServerEventSelector;
//------------------------------------------------------------------------------
typedef enum
{
	TCPServerRequestDelay = 1U,
	
} TCPServerRequestSelector;
//------------------------------------------------------------------------------
typedef enum
{
	TCPServerValueTransmitterStatus = 1U,
	
} TCPServerValueSelector;
//------------------------------------------------------------------------------
typedef union
{
  struct
	{
    uint32_t Connected : 1;
    uint32_t IsOpen : 1;
    uint32_t Update : 1;
  };
  uint32_t Value;
	
} TCPServerSockStatusT;
//------------------------------------------------------------------------------
typedef struct
{
  TCPServerSockStatusT Status;
  uint8_t Number;
  uint8_t State;
  uint16_t Port;
  uint16_t FreeSize;
	
} TCPServerSockT;
//------------------------------------------------------------------------------
typedef struct
{
  uint8_t Ip[4];
  uint8_t Gateway[4];
  uint8_t Subnet [4];
  uint8_t Mac[6];
	
} TCPServerInfoT;
//------------------------------------------------------------------------------
typedef void (*TCPServerActionHandler)(void* server);

typedef void (*TCPServerEventListener)(void* server, TCPServerEventSelector event, uint32_t args, uint32_t count);
typedef xResult (*TCPServerRequestListener)(void* server, TCPServerRequestSelector selector, uint32_t args, uint32_t count);

typedef int (*TCPServerActionGetValue)(void* server, TCPServerValueSelector selector);
typedef xResult (*TCPServerActionSetValue)(void* server, TCPServerValueSelector selector, uint32_t value);
//------------------------------------------------------------------------------
typedef struct
{
	TCPServerActionHandler Handler;
	
	TCPServerEventListener EventListener;
	TCPServerRequestListener RequestListener;
	
	TCPServerActionGetValue GetValue;
	TCPServerActionSetValue SetValue;
	
} TCPServerInterfaceT;
//------------------------------------------------------------------------------
typedef struct
{
	OBJECT_HEADER;
	
	void* Adapter;
	
	TCPServerInterfaceT Interface;
	
	TCPServerSockT Sock;
  TCPServerInfoT Info;
  
  xTxT Tx;
  xRxT Rx;
	  
} TCPServerT;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* TCP_SERVER_TYPES_H */

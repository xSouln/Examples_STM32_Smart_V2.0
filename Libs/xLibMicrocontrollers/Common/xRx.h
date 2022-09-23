//==============================================================================
#ifndef X_RX_RECIVER_H
#define X_RX_RECIVER_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
#include "xTypes.h"
#include "xTx.h"
//==============================================================================
typedef enum
{
	xRxEventIRQ = 1U,
	xRxEventReceiveComplete
	
} xRxEventSelector;
//------------------------------------------------------------------------------
typedef enum
{
	xRxRequestReceive = 1U,
	xRxRequestEnableReceiver,
	xRxRequestDisableReceiver,
	
	xRxRequestClearBuffer,
	
} xRxRequestSelector;
//------------------------------------------------------------------------------
typedef enum
{
	xRxValueReceiverEnableState = 1U
	
} xRxValueSelector;
//------------------------------------------------------------------------------
typedef union
{
  struct
	{
		uint32_t IsInit : 1;
		
		uint32_t Receiver : 4;
	};
	uint32_t Value;
	
} xRxStatusT;
//------------------------------------------------------------------------------
typedef void (*xRxActionHandler)(void* rx);
typedef void (*xRxEventListener)(void* rx, xRxEventSelector event, uint32_t args, uint32_t count);

typedef xResult (*xRxRequestListener)(void* rx, xRxRequestSelector selector, uint32_t args, uint32_t count);

typedef int (*xRxActionGetValue)(void* rx, xRxValueSelector selector);
typedef xResult (*xRxActionSetValue)(void* rx, xRxValueSelector selector, uint32_t value);
//------------------------------------------------------------------------------
typedef struct
{
	xRxActionHandler Handler;
	
	xRxEventListener EventListener;
	xRxRequestListener RequestListener;
	
	xRxActionGetValue GetValue;
	xRxActionSetValue SetValue;
	
} xRxInterfaceT;
//------------------------------------------------------------------------------
typedef void xRxAdapterT;
//------------------------------------------------------------------------------
typedef struct
{
  OBJECT_HEADER;
  
	xRxStatusT Status;
	
	xRxAdapterT* Adapter;
	xRxInterfaceT* Interface;
	
  xTxT* Tx;
	
} xRxT;
//==============================================================================
extern void xRxHandler(xRxT* rx);
extern int xRxReceive(xRxT* rx, uint8_t* data, uint32_t size);

extern void xRxDeclareEventEvent(xRxT* rx, xRxEventSelector event, uint32_t args, uint32_t count);
extern xResult xRxDeclareEventRequest(xRxT* rx, xRxRequestSelector selector, uint32_t args, uint32_t count);

extern xResult xRxSetValue(xRxT* rx, xRxValueSelector selector, uint32_t value);
extern int xRxGetValue(xRxT* rx, xRxValueSelector selector);

xResult xRxInit(xRxT* rx,
									void* parent,
									xTxAdapterT* adapter,
									xRxInterfaceT* interface);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif

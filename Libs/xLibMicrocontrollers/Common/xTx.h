//==============================================================================
#ifndef X_TX_H
#define X_TX_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//------------------------------------------------------------------------------
#include "xTypes.h"
#include "xDataBuffer.h"
#include "xCircleBuffer.h"
//==============================================================================
#define xtx_word_t uint32_t
//==============================================================================
typedef enum
{
	xTxStatusFree,
	xTxStatusIsTransmits,
	xTxStatusError
	
} xTxTransmitterStatus;
//------------------------------------------------------------------------------
typedef enum
{
	xTxStateDisable,
	xTxStateEnable,
	
} xTxState;
//------------------------------------------------------------------------------
typedef enum
{
	xTxEventIRQ = 1U,
	xTxEventTransmissionComplete
	
} xTxEventSelector;
//------------------------------------------------------------------------------
typedef enum
{
	xTxRequestTransmitData = 1U,
	xTxRequestEnableTransmitter,
	xTxRequestDisableTransmitter,
	
	xTxRequestStartTransmission,
	xTxRequestStopTransmission,
	xTxRequestAbortTransmission,
	
	xTxRequestClearBuffer,
	
} xTxRequestSelector;
//------------------------------------------------------------------------------
typedef enum
{
	xTxValueTransmitterStatus = 1U,
	xTxValueBufferSize,
	xTxValueFreeBufferSize,
	
} xTxValueSelector;
//==============================================================================
typedef void (*xTxActionHandler)(void* tx);

typedef void (*xTxEventListener)(void* tx, xTxEventSelector event, uint32_t args, uint32_t count);
typedef xResult (*xTxRequestListener)(void* tx, xTxRequestSelector selector, uint32_t args, uint32_t count);

typedef int (*xTxActionGetValue)(void* tx, xTxValueSelector selector);
typedef xResult (*xTxActionSetValue)(void* tx, xTxValueSelector selector, uint32_t value);
//==============================================================================
typedef union
{
  struct
  {
    uint32_t Transmitted : 1;
		
		uint32_t RequestState : 4;
	};
	
  uint32_t Value;
	
} xTxHandleT;
//------------------------------------------------------------------------------
typedef union
{
  struct
  {
		uint32_t IsInit : 1;
		
    xTxTransmitterStatus Transmitter : 4;
  };
	
  uint32_t Value;
	
} xTxStatusT;
//------------------------------------------------------------------------------
typedef struct
{
	xTxActionHandler Handler;
	
	xTxEventListener EventListener;
	xTxRequestListener RequestListener;
	
	xTxActionGetValue GetValue;
	xTxActionSetValue SetValue;
	
} xTxInterfaceT;
//------------------------------------------------------------------------------
typedef void xTxAdapterT;
//------------------------------------------------------------------------------
typedef struct
{
  OBJECT_HEADER;
	
  xTxHandleT Handle;
	xTxStatusT Status;
	
	xTxAdapterT* Adapter;
	xTxInterfaceT* Interface;
	
	xDataBufferT* ObjectBuffer;
	
} xTxT;
//==============================================================================								
extern void xTxHandler(xTxT* tx);

extern void xTxDeclareEvent(xTxT* tx, xTxEventSelector event, uint32_t args, uint32_t count);
extern xResult xTxDeclareRequest(xTxT* tx, xTxRequestSelector selector, uint32_t args, uint32_t count);

extern xResult xTxSetValue(xTxT* tx, xTxValueSelector selector, uint32_t value);
extern int xTxGetValue(xTxT* tx, xTxValueSelector selector);

extern int xTxTransmitData(xTxT* tx, void* data, uint32_t data_size);
extern int xTxTransmitByte(xTxT* tx, uint8_t byte);
extern int xTxTransmitString(xTxT* tx, char* str);

xResult xTxInit(xTxT* tx,
								void* parent,
								xTxAdapterT* adapter,
								xTxInterfaceT* interface,
								xDataBufferT* object_buffer);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif

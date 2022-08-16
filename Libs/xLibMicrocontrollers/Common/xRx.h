//==============================================================================
#ifndef X_RX_RECIVER_H
#define X_RX_RECIVER_H
//==============================================================================
#include <stdint.h>
#include <stdbool.h>
#include "xType.h"
#include "xTx.h"
//==============================================================================
typedef enum
{
  xRxEndLineResultContinue,
  xRxEndLineResultReset
	
} xRxEndLineResult;
//==============================================================================
typedef struct
{
  uint16_t Storage : 1;
  uint16_t BufLoaded : 1;
  uint16_t ReceiveComlite : 1;
  uint16_t EndLineControl : 1;
  
  uint16_t CrcEnable : 1;
	
}xRxStateT;
//==============================================================================
typedef void (*xRxCircleReceiverControlActionReceive)(void* receiver,
																											uint8_t* data,
																											uint16_t data_size);

typedef struct
{
	xRxCircleReceiverControlActionReceive Receive;
	
} xRxCircleReceiverControlT;
//==============================================================================
typedef struct
{
  uint8_t *Buffer;
	uint16_t SizeMask;
  volatile uint16_t TotalIndex;
  uint16_t HandlerIndex;
	
} xRxCircleReceiverT;
//==============================================================================
typedef void (*xRxHandler)(xObject rx);
typedef xRxEndLineResult (*xRxEventEndLine)(xObject rx, uint8_t* object, uint16_t size);

typedef struct
{
	xRxHandler Handler;
	xRxEventEndLine EventEndLine;
	
} xRxObjectReceiverControlT;
//==============================================================================
typedef struct
{
  uint8_t* Object;
  uint16_t ObjectMaxSize;
  uint16_t ObjectSize;
	
  xRxEventEndLine EventEndLine;
	
} xRxObjectReceiverT;
//==============================================================================
typedef struct
{
  OBJECT_HEADER;
  
  volatile xRxStateT State;
  xRxCircleReceiverT CircleReceiver;
  xRxObjectReceiverT ObjectReceiver;
  
  xTxT* Tx;
	
} xRxT;
//==============================================================================
xResult xRxInit(xRxT* rx,
								void* parent,
								xTxT* tx,
								uint8_t* circle_buf, uint16_t circle_buf_size_mask,
								uint8_t* object_buf, uint16_t object_buf_size,
								xRxEventEndLine event_end_line);

void xRxObjectReceive(xRxT *rx, uint8_t *data, uint16_t data_size);
void xRxCircleReceiveByte(xRxT *rx, uint8_t data);
void xRxObjectUpdate(xRxT* rx);

void xRxCircleReceive(xRxT *rx, uint8_t *data, uint16_t data_size);

void xRxClear(xRxT* rx);
//==============================================================================
#endif

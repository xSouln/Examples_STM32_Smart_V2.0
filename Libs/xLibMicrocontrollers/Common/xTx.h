//==============================================================================
#ifndef X_TX_H
#define X_TX_H
//==============================================================================
#include <stdint.h>
#include <stdbool.h>
#include "xType.h"
#include "xDataBuffer.h"
//==============================================================================
#define xTX_SET_MASK_SIZE (bits)(~(0xffff << bits))
//==============================================================================
typedef enum
{
	xTxStatusFree,
	xTxStatusTransmitted,
	xTxStatusError
	
} xTxStatus;
//==============================================================================
typedef enum
{
	xTxStateDisable,
	xTxStateEnable,
	
} xTxState;
//==============================================================================
typedef xResult (*xTxActionStartTransmission)(xObject tx);
typedef xResult (*xTxActionTransmitData)(xObject tx, xObject data, uint16_t data_size);
typedef xResult (*xTxActionEndTransmission)(xObject tx);

typedef xResult (*xTxActionSetTransmiterState)(xObject tx, xTxState state);
typedef uint16_t (*xTxActionGetFreeSize)(xObject tx);
typedef xTxStatus (*xTxActionGetTransmiterStatus)(xObject tx);

typedef void (*xTxActionHandler)(xObject tx);
typedef void (*xTxActionIRQHandler)(xObject tx);
//==============================================================================
typedef union
{
  struct
  {
    uint16_t Transmitted : 1;
    uint16_t IsAdd : 1;
    uint16_t IsUpdate : 1;
    uint16_t TransferComplite : 1;
    
    uint16_t TxBufFull : 1;
    uint16_t Lock : 1;
    uint16_t CrcEnable : 1;
  };
  uint16_t Value;
	
} xTxHandleT;
//==============================================================================
typedef struct
{
	void* Handle;
	
	xTxActionStartTransmission StartTransmission;
	xTxActionTransmitData TransmitData;
	xTxActionEndTransmission EndTransmission;
	
	xTxActionSetTransmiterState SetTransmiterState;
	xTxActionGetFreeSize GetFreeSize;
	xTxActionGetTransmiterStatus GetTransmiterStatus;
	
	xTxActionHandler Handler;
	xTxActionIRQHandler IRQHandler;
	
} xTxControlT;
//==============================================================================
typedef struct
{
  OBJECT_HEADER;
	
  xTxHandleT Handle;
	xTxStatus Status;
	xTxControlT* Control;
	
	uint8_t* Buffer;
	volatile uint16_t TotalIndex;
	uint16_t HandlerIndex;
	uint16_t SizeMask;
  uint16_t Crc;
	
	xDataBufferT* ObjectBuffer;
  xObject Rx;
	
} xTxT;
//==============================================================================
xResult xTxInit(xTxT* tx, void* parent,
								uint8_t* buf, uint16_t buf_size_mask,
								xTxControlT* control);
								
void xTxHandler(xTxT *tx);
void xTxIRQHandler(xTxT *tx);

extern xResult xTxTransmitData(xTxT *Tx, xObject data, uint16_t data_size);
extern xResult xTxTransmitByte(xTxT *Tx, uint8_t byte);
extern xResult xTxTransmitString(xTxT *Tx, char* str);
		
extern xResult xTxStartTransmission(xTxT* tx);
extern xResult xTxEndTransmission(xTxT* tx);
		
extern uint16_t xTxGetFreeSize(xTxT* tx);
extern void xTxClear(xTxT* tx);
//==============================================================================
#endif

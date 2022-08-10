/*
 * XTRANSACTION_H_.h
 *
 * Created: 28.09.2021 13:42:04
 *  Author: rekuts
 */
#ifndef XTRANSACTION_H_
#define XTRANSACTION_H_
//==============================================================================
#include "xRequest.h"
#include "xRx.h"
#include "xDataBuffer.h"
//==============================================================================
typedef void (*xTransactionAction)(xRxT* rx,
																		xDataBufferT* data_buffer,
																		xObject object,
																		uint16_t object_size);
//==============================================================================
typedef struct
{
  uint16_t Description;
  uint16_t Id;
  xTransactionAction Action;
  
} xTransactionT;
//==============================================================================
xTransactionT* xTransactionIdentify(xTransactionT* transaction, uint16_t key);
//==============================================================================
#endif /* XTRANSACTION_H_ */

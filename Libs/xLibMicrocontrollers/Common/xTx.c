//==============================================================================
#include <string.h>
#include "xTx.h"
//==============================================================================
static uint16_t xTxGetFreeSizeDefault(xTxT *tx)
{
	int difference = (tx->TotalIndex - tx->HandlerIndex) & tx->SizeMask;
	return (tx->SizeMask - difference);
}
//------------------------------------------------------------------------------
static xTxStatus xTxGetTransmiterStatusDefault(xTxT *tx)
{
	return xTxStatusFree;
}
//------------------------------------------------------------------------------
static xResult xTxSetTransmiterStateDefault(xTxT *tx, xTxState state)
{
	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult xTxStartTransmissionDefault(xTxT *tx)
{
	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult xTxTransmitDataDefault(xTxT *tx, xObject data, uint16_t data_size)
{
	uint8_t *ptr = (uint8_t*)data;
	
	if (data_size <= tx->Control->GetFreeSize(tx))
	{
		while(data_size)
		{
			tx->Buffer[tx->TotalIndex] = *ptr++;
			tx->TotalIndex++;
			tx->TotalIndex &= tx->SizeMask;
			data_size--;
		}
		tx->Control->Handler(tx);
		return xResultAccept;
	}
	
	return xResultError;
}
//------------------------------------------------------------------------------
static xResult xTxEndTransmissionDefault(xTxT *tx)
{
	return xResultAccept;
}
//------------------------------------------------------------------------------
static void xTxHandlerDefault(xTxT *tx)
{
	if (tx->TotalIndex != tx->HandlerIndex && tx->Control->GetTransmiterStatus(tx) == xTxStatusFree)
	{
		tx->Control->SetTransmiterState(tx, xTxStateEnable);
	}
}
//------------------------------------------------------------------------------
static void xTxIRQHandlerDefault(xTxT *tx)
{
	
}
//------------------------------------------------------------------------------
static xTxControlT ControlDefault =
{
	.StartTransmission = (xTxActionStartTransmission)xTxStartTransmissionDefault,
	.TransmitData = (xTxActionTransmitData)xTxTransmitDataDefault,
	.EndTransmission = (xTxActionEndTransmission)xTxEndTransmissionDefault,
	
	.SetTransmiterState = (xTxActionSetTransmiterState)xTxSetTransmiterStateDefault,
	.GetFreeSize = (xTxActionGetFreeSize)xTxGetFreeSizeDefault,
	.GetTransmiterStatus = (xTxActionGetTransmiterStatus)xTxGetTransmiterStatusDefault,
	
	.Handler = (xTxActionHandler)xTxHandlerDefault,
	.IRQHandler = (xTxActionIRQHandler)xTxIRQHandlerDefault
};
//==============================================================================
xResult xTxStartTransmission(xTxT* tx)
{
	return tx->Control->StartTransmission(tx);
}
//------------------------------------------------------------------------------
xResult xTxTransmitData(xTxT *tx, xObject data, uint16_t data_size)
{
	return tx->Control->TransmitData(tx, data, data_size);
}
//------------------------------------------------------------------------------
xResult xTxTransmitByte(xTxT *tx, uint8_t byte)
{
	return tx->Control->TransmitData(tx, &byte, sizeof(byte));
}
//------------------------------------------------------------------------------
xResult xTxTransmitString(xTxT *tx, char* str)
{
  return tx->Control->TransmitData(tx, str, strlen(str));
}
//------------------------------------------------------------------------------
xResult xTxEndTransmission(xTxT* tx)
{
	return tx->Control->EndTransmission(tx);
}
//==============================================================================
void xTxHandler(xTxT *tx)
{
	tx->Control->Handler(tx);
}
//------------------------------------------------------------------------------
void xTxIRQHandler(xTxT *tx)
{
	tx->Control->IRQHandler(tx);
}
//==============================================================================
uint16_t xTxGetFreeSize(xTxT *tx)
{
	return tx->Control->GetFreeSize(tx);
}
//==============================================================================
void xTxClear(xTxT* tx)
{
	tx->TotalIndex = 0;
	tx->HandlerIndex = 0;
}
//==============================================================================
xResult xTxInit(xTxT* tx,
								void* parent,
								uint8_t* buf,
								uint16_t buf_size_mask,
								xTxControlT* control)
{
  if (tx && control)
  {
		if (!tx->Description) { tx->Description = "xTxT"; }
		tx->Parent = parent;
		
    tx->Buffer = buf;
    tx->SizeMask = buf_size_mask;
		tx->Control = control;
		
		uint32_t* control_request = (uint32_t*)tx->Control;
		uint32_t* control_default = (uint32_t*)&ControlDefault;
		
		for (uint16_t i = 1; i < sizeof(xTxControlT) / sizeof(void*); i++)
		{
			if (!control_request[i]) { control_request[i] = control_default[i]; }
		}
		
		return xResultError;
  }
  return xResultAccept;
}
//==============================================================================

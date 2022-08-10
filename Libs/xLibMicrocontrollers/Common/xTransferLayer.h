/*
 * X_TRANSFER_LAYER_H.h
 *
 * Created: 28.09.2021 13:42:04
 *  Author:
 */
#ifndef X_TRANSFER_LAYER_H
#define X_TRANSFER_LAYER_H
//==============================================================================
#include "xType.h"
#include "xTx.h"
//==============================================================================
typedef struct
{
	uint32_t PacketSize;
	
} xTransferLayerOptionsT;
//==============================================================================
typedef enum
{
	xTransferLayerStatusFree,
	xTransferLayerStatusRequestTransfer,
	xTransferLayerStatusTransmits,
	xTransferLayerStatusEndTransfer,
	xTransferLayerStatusComplite
	
} xTransferLayerStatus;
//==============================================================================
typedef xResult (*xTransferLayerActionStartTransfer)(xObject layer);
typedef xResult (*xTransferLayerActionTransfer)(xObject layer, uint8_t* data, uint32_t data_size);
typedef xResult (*xTransferLayerActionEndTransfer)(xObject layer);

typedef struct
{
	void* Handle;
	
	xTransferLayerActionStartTransfer StartTransfer;
	xTransferLayerActionTransfer Transfer;
	xTransferLayerActionEndTransfer EndTransfer;
	
} xTransferLayerControlT;
//==============================================================================
typedef union
{
	struct
	{
		uint32_t Busy : 1;
		uint32_t TransferRequest : 1;
		uint32_t IsComplite : 1;
	};
	
	uint32_t Value;
	
} xTransferLayerHandleT;
//==============================================================================
typedef struct
{
	OBJECT_HEADER;
	
	xTransferLayerStatus Status;
	xTransferLayerOptionsT Options;
	xTransferLayerControlT* Control;
	
	uint8_t* Data;
	uint32_t DataSize;
	uint32_t DataTransferred;
	
	uint32_t TimeOut;
	
} xTransferLayerT;
//==============================================================================
xResult xTransferLayerInit(xTransferLayerT* layer, void* parent);
														
xResult xTransferLayerTransmit(xTransferLayerT* layer,
																xTransferLayerControlT* control,
																void* data,
																uint32_t data_size,
																uint32_t packet_size);

void xTransferLayerAbort(xTransferLayerT* layer);
void xTransferLayerHandler(xTransferLayerT* layer);
//==============================================================================
#endif /* X_TRANSFER_LAYER_H */

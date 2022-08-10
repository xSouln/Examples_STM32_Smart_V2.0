//==============================================================================
#include <string.h>
#include "xTransferLayer.h"
//==============================================================================
void xTransferLayerHandler(xTransferLayerT* layer)
{
	uint32_t packet_size;
	
	switch ((uint8_t)layer->Status)
	{
		case xTransferLayerStatusRequestTransfer:
			if (layer->Control->StartTransfer && layer->Control->StartTransfer(layer) != xResultAccept)
			{
				return;
			}
			layer->Status = xTransferLayerStatusTransmits;
			
		case xTransferLayerStatusTransmits:
			packet_size = layer->DataSize - layer->DataTransferred;
		
			if (packet_size > layer->Options.PacketSize)
			{
				packet_size = layer->Options.PacketSize;
			}
			
			if (packet_size)
			{
				if (layer->Control->Transfer(layer, layer->Data + layer->DataTransferred, packet_size) == xResultAccept)
				{
					layer->DataTransferred += packet_size;
				}
				return;
			}
			else
			{
				layer->Status = xTransferLayerStatusEndTransfer;
			}
			
		case xTransferLayerStatusEndTransfer:
			if (layer->Control->EndTransfer && layer->Control->EndTransfer(layer) != xResultAccept)
			{
				return;
			}
			layer->Status = xTransferLayerStatusComplite;
			break;
			
		default: return;
	}
}
//==============================================================================
xResult xTransferLayerTransmit(xTransferLayerT* layer,
																xTransferLayerControlT* control,
																void* data,
																uint32_t data_size,
																uint32_t packet_size)
{
	if (!layer || !control || !control->Transfer)
	{
		return xResultError;
	}
	
	if (!data || !data_size || !packet_size)
	{
		return xResultError;
	}
	
	if (layer->Status == xTransferLayerStatusFree || layer->Status == xTransferLayerStatusComplite)
	{
		layer->Data = (uint8_t*)data;
		layer->DataSize = data_size;
		layer->DataTransferred = 0;
		layer->Options.PacketSize = packet_size;
		layer->Control = control;
		
		layer->Status = xTransferLayerStatusRequestTransfer;
		
		return xResultAccept;
	}
	
	return xResultError;
}
//==============================================================================
void xTransferLayerAbort(xTransferLayerT* layer)
{
	
}
//==============================================================================
xResult xTransferLayerInit(xTransferLayerT* layer, void* parent)
{
	if (layer)
	{
		if (!layer->Description) { layer->Description = "xTransferLayerT";}
		layer->Parent = parent;
		
		return xResultAccept;
	}
	return xResultError;
}
//==============================================================================

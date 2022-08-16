//==============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xRx.h"
//==============================================================================
void xRxObjectUpdate(xRxT* rx)
{
  while(rx->CircleReceiver.TotalIndex != rx->CircleReceiver.HandlerIndex)
  {    
    rx->ObjectReceiver.Object[rx->ObjectReceiver.ObjectSize] = rx->CircleReceiver.Buffer[rx->CircleReceiver.HandlerIndex];        
    rx->ObjectReceiver.ObjectSize++;

    if(rx->ObjectReceiver.ObjectSize >= rx->ObjectReceiver.ObjectMaxSize)
    {
      rx->ObjectReceiver.EventEndLine(rx, rx->ObjectReceiver.Object, rx->ObjectReceiver.ObjectSize);
      rx->ObjectReceiver.ObjectSize = 0;
    }
    else if(rx->CircleReceiver.Buffer[rx->CircleReceiver.HandlerIndex] == '\r')
    {
      if(rx->ObjectReceiver.EventEndLine(rx, rx->ObjectReceiver.Object, rx->ObjectReceiver.ObjectSize - 1) == xRxEndLineResultReset)
      {
        rx->ObjectReceiver.ObjectSize = 0;
      }
    }
    
    rx->CircleReceiver.HandlerIndex++;
    rx->CircleReceiver.HandlerIndex &= rx->CircleReceiver.SizeMask;
  }
}
//==============================================================================
void xRxObjectReceive(xRxT* rx, uint8_t *data, uint16_t data_size)
{
  for(uint16_t i = 0; i < data_size; i++)
  {
    rx->ObjectReceiver.Object[rx->ObjectReceiver.ObjectSize] = data[i];
    rx->ObjectReceiver.ObjectSize++;
    
    if(rx->ObjectReceiver.ObjectSize >= rx->ObjectReceiver.ObjectMaxSize)
    {
      rx->ObjectReceiver.EventEndLine(rx, rx->ObjectReceiver.Object, rx->ObjectReceiver.ObjectSize);
      rx->ObjectReceiver.ObjectSize = 0;
    }    
    else if(data[i] == '\r')
    {
      if(rx->ObjectReceiver.EventEndLine(rx, rx->ObjectReceiver.Object, rx->ObjectReceiver.ObjectSize - 1) == xRxEndLineResultReset)
      {
        rx->ObjectReceiver.ObjectSize = 0;
      }
    }
  }
}
//==============================================================================
void xRxCircleReceive(xRxT *rx, uint8_t *data, uint16_t data_size)
{
	while (data_size)
	{
		rx->CircleReceiver.Buffer[rx->CircleReceiver.TotalIndex] = *data++;
		rx->CircleReceiver.TotalIndex++;
		rx->CircleReceiver.TotalIndex &= rx->CircleReceiver.SizeMask;
		
		data_size--;
	}
}
//==============================================================================
void xRxCircleReceiveByte(xRxT *rx, uint8_t data)
{
	rx->CircleReceiver.Buffer[rx->CircleReceiver.TotalIndex] = data;
	rx->CircleReceiver.TotalIndex++;
	rx->CircleReceiver.TotalIndex &= rx->CircleReceiver.SizeMask;
}
//==============================================================================
void xRxClear(xRxT* rx)
{
  rx->CircleReceiver.TotalIndex = 0;
  rx->CircleReceiver.HandlerIndex = 0;
  rx->ObjectReceiver.ObjectSize = 0;
}
//==============================================================================
xResult xRxInit(xRxT* rx,
								void* parent,
								xTxT* tx,
								uint8_t* circle_buf, uint16_t circle_buf_size_mask,
								uint8_t* object_buf, uint16_t object_buf_size,
								xRxEventEndLine event_end_line)
{
	if (rx)
	{
		if (!rx->Description) { rx->Description = "xRxT";}
		rx->Parent = parent;
		
		rx->Tx = tx;
		
		rx->CircleReceiver.Buffer = circle_buf;
		rx->CircleReceiver.SizeMask = circle_buf_size_mask;
		
		rx->ObjectReceiver.Object = object_buf;
		rx->ObjectReceiver.ObjectMaxSize = object_buf_size;
		rx->ObjectReceiver.EventEndLine = event_end_line;
		
		return xResultAccept;
	}
	
	return xResultError;
}
//==============================================================================

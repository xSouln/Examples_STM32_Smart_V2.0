//==============================================================================
#include "xCircleBuffer.h"
//==============================================================================
uint32_t xCircleBufferAdd(xCircleBufferT* buffer, uint8_t* data, uint32_t size)
{
	for (uint16_t i = 0; i < size; i++)
	{
		buffer->Buffer[buffer->TotalIndex] = data[i];
		buffer->TotalIndex++;
		buffer->TotalIndex &= buffer->SizeMask;
	}
	
	return size;
}
//------------------------------------------------------------------------------
uint8_t xCircleBufferGet(xCircleBufferT* buffer)
{
	uint8_t value = buffer->Buffer[buffer->HandlerIndex];
	buffer->HandlerIndex++;
	buffer->HandlerIndex &= buffer->SizeMask;
	
	return value;
}
//------------------------------------------------------------------------------
xResult xCircleBufferInit(xCircleBufferT* circle_buffer,
														void* parent,
														uint8_t* buffer,
														uint32_t buffer_size_mask)
{
  if (circle_buffer && buffer && buffer_size_mask)
  {
		circle_buffer->Description = "xCircleBufferT";
		circle_buffer->Parent = parent;
		
		circle_buffer->Buffer = buffer;
		circle_buffer->SizeMask = buffer_size_mask;
		
		return xResultError;
  }
  return xResultError;
}
//==============================================================================

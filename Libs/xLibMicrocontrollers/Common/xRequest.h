/*
 * X_REQUEST_H.h
 *
 * Created: 28.09.2021 13:42:04
 *  Author:
 */
#ifndef X_REQUEST_H
#define X_REQUEST_H
//==============================================================================
#include <stdint.h>
#include <stdbool.h>
#include "xType.h"
#include "xRx.h"
//==============================================================================
typedef uint16_t (*xRequestAction)(xObject manager, //xRequestManagerT
																		xObject object,
																		uint16_t object_size);
//------------------------------------------------------------------------------
typedef struct
{
  xObject Header;
  uint8_t HeaderLength;
  uint8_t Mode;
  xRequestAction Action;
   
} xRequestT;
//------------------------------------------------------------------------------
typedef struct
{
	OBJECT_HEADER;
	
	void* Device;
	
  xRequestT* Requests;
	xRequestT* FoundRequest;
	
	xRxT* Rx;
   
} xRequestManagerT;
//==============================================================================
xRequestT* xRequestIdentify(xRxT* rx, xRequestManagerT* manager, uint8_t data[], uint16_t data_size);
xResult xRequestManagerInit(xRequestManagerT* manager, void* parent, void* device, xRequestT* requests);
//==============================================================================
#define NEW_REQUEST0(header, request)\
{\
  .Header = (char*)header,\
  .HeaderLength = SIZE_STRING(header),\
  .Action = (xRequestAction)request\
}
//------------------------------------------------------------------------------
#define NEW_REQUEST1(header, request)\
{\
  .Header = (char*)&header,\
  .HeaderLength = sizeof(header),\
  .Action = (xRequestAction)request\
}
//==============================================================================
#endif /* X_REQUEST_H */

//==============================================================================
#ifndef X_RX_REQUEST_H
#define X_RX_REQUEST_H
//==============================================================================
#include "xTypes.h"
#include "xRx.h"
//==============================================================================
typedef uint16_t (*xRxRequestAction)(xObject manager, //xRequestManagerT
																		xObject object,
																		uint16_t object_size);
//------------------------------------------------------------------------------
typedef struct
{
  xObject Header;
  uint8_t HeaderLength;
  uint8_t Mode;
  xRxRequestAction Action;
   
} xRxRequestT;
//------------------------------------------------------------------------------
typedef struct
{
	OBJECT_HEADER;
	
	void* Device;
	
  xRxRequestT* Requests;
	xRxRequestT* FoundRequest;
	
	xRxT* RxLine;
   
} xRxRequestManagerT;
//==============================================================================
xRxRequestT* xRxRequestIdentify(xRxT* rx, xRxRequestManagerT* manager, uint8_t data[], uint16_t data_size);
xResult xRxRequestManagerInit(xRxRequestManagerT* manager, void* parent, void* device, xRxRequestT* requests);
//==============================================================================
#define NEW_RX_REQUEST0(header, request)\
{\
  .Header = (char*)header,\
  .HeaderLength = SIZE_STRING(header),\
  .Action = (xRxRequestAction)request\
}
//------------------------------------------------------------------------------
#define NEW_RX_REQUEST1(header, request)\
{\
  .Header = (char*)&header,\
  .HeaderLength = sizeof(header),\
  .Action = (xRxRequestAction)request\
}
//==============================================================================
#endif /* X_RECEIVER_REQUEST_H */

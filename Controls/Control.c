//==============================================================================
#include "Control.h"
#include "main.h"
#include "tim.h"
//==============================================================================
ActionResults ControlInit(void* parent)
{
	Control.Description = "ControlT";
	Control.Parent = parent;
	
	SerialPortInit(USART1, "main");
	
	CupsRGB_ControlInit(&Control);
	Control.CupsRGB_Control = &CupsRGB_Control;
	
	xRequestManagerInit(&Control.RequestManager,
											&Control,
											&Control,
											(xRequestT*)Requests);
	
	return ResultAccept;
}
//==============================================================================
ControlT Control;
//==============================================================================

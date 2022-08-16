//==============================================================================
#include "CupsRGB_Control.h"
#include "main.h"
#include "tim.h"
//==============================================================================
CupsRGB_Result CupsRGB_ControlInit(void* parent)
{
	CupsRGB_Control.Description = "CupsRGB_ControlT";
	CupsRGB_Control.Parent = parent;
	
	xRequestManagerInit(&CupsRGB_Control.RequestManager,
											&CupsRGB_Control,
											&CupsRGB_Control,
											(xRequestT*)CupsRGB_Requests);
	
	return CupsRGB_ResultAccept;
}
//==============================================================================
CupsRGB_ControlT CupsRGB_Control;
//==============================================================================

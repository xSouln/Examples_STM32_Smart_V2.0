//==============================================================================
#ifndef CUPS_RGB_CONTROL_H
#define CUPS_RGB_CONTROL_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "CupsRGB_Types.h"
#include "CupsRGB_Info.h"
#include "CupsRGB_Config.h"
#include "CupsRGB/Adapters/CupsRGB_Adapter.h"
#include "CupsRGB/Communications/CupsRGB_Requests.h"
#include "CupsRGB/Communications/CupsRGB_Transactions.h"
//==============================================================================
typedef struct
{
	OBJECT_HEADER;
	
	xRequestManagerT RequestManager;
	
} CupsRGB_ControlT;
//==============================================================================
extern CupsRGB_ControlT CupsRGB_Control;
//------------------------------------------------------------------------------
CupsRGB_Result CupsRGB_ControlInit(void* parent);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* CUPS_RGB_CONTROL_H */

//==============================================================================
#include <string.h>
#include <stdbool.h>
#include "L298.h"
//==============================================================================
//L298_Result 
//==============================================================================
L298_Result L298_DriverInit(L298_DriverT* driver,
														void* parent,
														L298_ControlT* control,
														L298_OptionsT* options)
{
	if (driver && control)
	{
		if (!driver->Description) { driver->Description = "L298_DriverT"; }
		driver->Parent = parent;
		driver->Control = control;
		if (options)
		{
			memcpy((uint8_t*)&driver->Options, options, sizeof(L298_OptionsT));
		}
		driver->Status.IsInit = true;
		
		return L298_ResultAccept;
	}
	return L298_ResultNullPointer;
}
//==============================================================================

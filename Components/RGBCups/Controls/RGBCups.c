//==============================================================================
#include "RGBCups.h"
#include "RGBCups/Adapters/RGBCup_Adapter.h"
//==============================================================================
//==============================================================================
RGBCupsResult RGBCups_Init(void* parent, RGBCupInterfaceT* interface)
{
	for (uint8_t i = 0; i < RGBCupsCount; i++)
	{
		RGBCups[i].Description = "RGBCupsT";
		RGBCups[i].Parent = parent;
		RGBCups[i].Interface = interface;
		
		RGBCups[i].Id = i;
	}
	
	return RGBCupsResultAccept;
}
//==============================================================================

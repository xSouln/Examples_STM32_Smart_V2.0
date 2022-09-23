//==============================================================================
#include <string.h>
#include "RGBCups/Logic/RGBCups_Logics.h"
#include "RGBCups/Controls/RGBCups.h"
#include "RGBCups/Adapters/RGBCup_Adapter.h"
//==============================================================================
extern 
//==============================================================================
RGBCupsResult RGBCups_SetColor(RGBCupSelector cups, WS2812_PixelT pixel)
{
	uint8_t request = cups;
	uint8_t i = 0;
	
	while (request && i < RGBCupsCount)
	{
		if (request & 0x01)
		{
			WS2812_FillPixels(&RGBCups[i].Driver, pixel, 0, RGBCups[i].PixelsCount);
		}
		
		request >>= 1;
		i++;
	}
	
	return RGBCupsResultAccept;
}
//==============================================================================
RGBCupsResult RGBCups_UpdateLayout(RGBCupSelector cups, uint32_t time_out)
{
	uint8_t request = cups;
	uint8_t i = 0;
	
	while (request && i < RGBCupsCount)
	{
		if (request & 0x03)
		{
			while (WS2812_GetTransmitterStatus(&RGBCups[i].Driver) != WS2812_TransmitterStopped && time_out)
			{
				if (!time_out)
				{
					return RGBCupsResultTimeOut;
				}
			}
			WS2812_UpdateLayout(&RGBCups[i].Driver);
		}
		
		request >>= 2;
		i += 2;
	}
	
	return RGBCupsResultAccept;
}
//==============================================================================
static void RGBCups_DrawingManagerHandler(WS2812_T* driver)
{
	RGBCupDrawingRuleT* rule = driver->DrawingManager.Rule;
	
	if (rule->StepNumber >= rule->Steps)
	{
		rule->StepNumber = rule->Steps;
		rule->Diraction = -1;
	}
	else if (rule->StepNumber < 0)
	{
		rule->StepNumber = 0;
		rule->Diraction = 1;
	}
	
	WS2812_PixelT pixel =
	{
		.Green = (uint8_t)(rule->GreenIncrement * rule->StepNumber),
		.Red = (uint8_t)(rule->RedIncrement * rule->StepNumber),
		.Blue = (uint8_t)(rule->BlueIncrement * rule->StepNumber)
	};
	
	rule->StepNumber += rule->Diraction;
	
	WS2812_FillPixels(driver, pixel, 0, ((RGBCupT*)driver->Parent)->PixelsCount);
}
//------------------------------------------------------------------------------
RGBCupsResult RGBCups_DrawingStart(RGBCupSelector cups, RGBCupDrawingRuleT* drawing_rule)
{
	uint8_t request = cups;
	uint8_t i = 0;
	
	WS2812_DrawingManagerT drawing_manager =
	{
		.Handler = (WS2812_ActionHandler)RGBCups_DrawingManagerHandler,
		.Rule = &drawing_rule,
	};
	
	while (request && i < RGBCupsCount)
	{
		if (request & 0x01)
		{
			memcpy((uint8_t*)&RGBCups[i].DrawingRule, (uint8_t*)drawing_rule, sizeof(RGBCupDrawingRuleT));
			
			drawing_manager.Rule = &RGBCups[i].DrawingRule;
			WS2812_DrawingStart(&RGBCups[i].Driver, &drawing_manager);
		}
		
		request >>= 1;
		i++;
	}
	
	return RGBCupsResultAccept;
}
//------------------------------------------------------------------------------
RGBCupsResult RGBCups_DrawingStop(RGBCupSelector cups)
{
	uint8_t request = cups;
	uint8_t i = 0;
	
	while (request && i < RGBCupsCount)
	{
		if (request & 0x01)
		{
			WS2812_DrawingStop(&RGBCups[i].Driver);
		}
		
		request >>= 1;
		i++;
	}
	
	return RGBCupsResultAccept;
}
//------------------------------------------------------------------------------
void RGBCups_Draw(RGBCupSelector cups)
{
	uint8_t request = cups;
	uint8_t i = 0;
	
	while (request && i < RGBCupsCount)
	{
		if (request & 0x01)
		{
			WS2812_Draw(&RGBCups[i].Driver);
		}
		
		request >>= 1;
		i++;
	}
}
//==============================================================================

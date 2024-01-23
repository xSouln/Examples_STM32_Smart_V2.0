//==============================================================================
//includes:

#include <stdlib.h>
#include "Abstractions/xSystem/xSystem.h"
#include "Components.h"
//==============================================================================
//types:


//==============================================================================
//variables:

#if OS_TYPE == OS_TYPE_FREERTOS
static SemaphoreHandle_t privateRNGMutex;
#endif
//==============================================================================
//functions:

xSystemTimeT xSystemGetTime(void)
{
	return HAL_GetTick();
}
//------------------------------------------------------------------------------
void xSystemDelay(xSystemTimeT time)
{
	HAL_Delay(time);
}
//------------------------------------------------------------------------------
void xSystemEnableIRQ(void* context)
{
	__enable_irq();
}
//------------------------------------------------------------------------------
void xSystemDisableIRQ(void* context)
{
	__disable_irq();
}
//------------------------------------------------------------------------------
void xSystemReset(void* context)
{

}
//------------------------------------------------------------------------------
uint32_t xSystemGetRandom()
{
	uint32_t result;

#if OS_TYPE == OS_TYPE_FREERTOS
	xSemaphoreTake(privateRNGMutex, portMAX_DELAY);
#endif

	result = rand();

#if OS_TYPE == OS_TYPE_FREERTOS
	xSemaphoreGive(privateRNGMutex);
#endif

	return result;
}
//------------------------------------------------------------------------------
xResult xSystemInterfaceInit()
{
#if OS_TYPE == OS_TYPE_FREERTOS
	privateRNGMutex = xSemaphoreCreateMutex();
#endif

	return xResultAccept;
}
//==============================================================================

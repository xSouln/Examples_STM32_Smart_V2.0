//==============================================================================
#include <string.h>
#include <stdbool.h>
#include "L298/L298_Adapter.h"
#include "main.h"
#include "tim.h"
#include "cmsis_os.h"
//==============================================================================
//L298_Result
//==============================================================================
static L298_Result SetMoveState(L298_DriverT* driver, L298_State state)
{
	switch((uint8_t)state)
	{
		case L298_StateEnable:
			if (driver->Diraction == L298_MoveDiractionForward)
			{
				HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
				HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
			}
			else
			{
				HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
				HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
			}
			break;
			
		case L298_StateDisable:
			HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
			HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
			break;
	}
	
	return L298_ResultAccept;
}
//==============================================================================
static L298_Result SetDriverState(void* driver, L298_State state)
{
	switch((uint8_t)state)
	{
		case L298_StateEnable: MOTOR1_ENABLE_GPIO_Port->ODR &= ~MOTOR1_ENABLE_Pin; break;
		case L298_StateDisable: MOTOR1_ENABLE_GPIO_Port->ODR |= MOTOR1_ENABLE_Pin; break;
	}
	return L298_ResultAccept;
}
//==============================================================================
L298_Result SetPWMHandlerState(void* driver, L298_State state)
{
	switch((uint8_t)state)
	{
		case L298_StateEnable: htim3.Instance->DIER |= TIM_DIER_UIE; break;
		case L298_StateDisable: htim3.Instance->DIER &= ~TIM_DIER_UIE; break;
	}
	return L298_ResultAccept;
}
//==============================================================================
static L298_Result SetLockState(void* driver, L298_State state)
{
	
	return L298_ResultAccept;
}
//==============================================================================
static L298_Result Delay(void* driver, uint32_t ms)
{
	osDelay(ms);
	return L298_ResultAccept;
}
//==============================================================================
static L298_Result SetSpeed(void* driver, float speed)
{
	const uint16_t base_period;
	htim3.Instance->CCR2 = (uint16_t)(htim3.Instance->ARR * speed) - 1;
	htim3.Instance->CCR3 = (uint16_t)(htim3.Instance->ARR * speed) - 1;
	return L298_ResultAccept;
}
//==============================================================================
float GetSpeed(void* driver)
{
	return 0.0;
}
//==============================================================================
L298_Result SetFrequency(void* driver, float frequency)
{
	return L298_ResultAccept;
}
//==============================================================================
float GetFrequency(void* driver)
{
	return 0.0;
}
//==============================================================================
L298_Result SetEncoderPosition(void* driver, int position)
{
	
	return L298_ResultAccept;
}
//==============================================================================
int GetEncoderPosition(L298_DriverT* driver)
{
	return 0;
}
//==============================================================================
static L298_CONTROL_LAYER_DEF(Adapter,
	SetMoveState,
	SetSpeed,
	GetSpeed,
	SetFrequency,
	GetFrequency,
	SetDriverState,
	GetEncoderPosition,
	SetEncoderPosition,
	SetPWMHandlerState,
	SetLockState,
	Delay
);
//==============================================================================
L298_Result L298_DeviceInit(L298_DeviceT* device,
														void* parent)
{
	if (device)
	{
		if (!device->Description) { device->Description = "L298_DeviceT"; }
		device->Parent = parent;
		
		device->Driver.Options.HandlerUpdateFrequency = 1000;
		
		device->Driver.Options.StartSpeed = 0.1;
		device->Driver.Options.StopSpeed = 0.1;
		
		device->Driver.Options.Acceleration = 0.33;
		device->Driver.Options.Deceleration = 0.33;
		
		return L298_DriverInit(&device->Driver,
														device,
														&AdapterControl,
														0);
	}
	return L298_ResultNullPointer;
}
//==============================================================================
//==============================================================================

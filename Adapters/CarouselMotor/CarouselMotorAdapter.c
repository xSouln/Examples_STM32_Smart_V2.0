//==============================================================================
#include <string.h>
#include <stdbool.h>
#include "CarouselMotor/CarouselMotorAdapter.h"
#include "main.h"
#include "tim.h"
#include "cmsis_os.h"
//==============================================================================

//==============================================================================
static CarouselMotorResult SetMoveState(MotorDriverT* driver, MotorDriverState state)
{
	switch((uint8_t)state)
	{
		case MotorDriverStateEnable:
			if (driver->Diraction == MotorDriverMoveDiractionForward)
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
			
		case MotorDriverStateDisable:
			HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
			HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
			break;
	}
	
	return CarouselMotorResultAccept;
}
//==============================================================================
static CarouselMotorResult SetDriverState(MotorDriverT* driver, MotorDriverState state)
{
	switch((uint8_t)state)
	{
		case MotorDriverStateEnable: MOTOR1_ENABLE_GPIO_Port->ODR &= ~MOTOR1_ENABLE_Pin; break;
		case MotorDriverStateDisable: MOTOR1_ENABLE_GPIO_Port->ODR |= MOTOR1_ENABLE_Pin; break;
	}
	return CarouselMotorResultAccept;
}
//==============================================================================
CarouselMotorResult SetPWMHandlerState(MotorDriverT* driver, MotorDriverState state)
{
	switch((uint8_t)state)
	{
		case MotorDriverStateEnable: htim3.Instance->DIER |= TIM_DIER_UIE; break;
		case MotorDriverStateDisable: htim3.Instance->DIER &= ~TIM_DIER_UIE; break;
	}
	return CarouselMotorResultAccept;
}
//==============================================================================
static CarouselMotorResult SetLockState(MotorDriverT* driver, MotorDriverMotorState state)
{
	
	return CarouselMotorResultAccept;
}
//==============================================================================
static CarouselMotorResult Delay(MotorDriverT* driver, uint32_t ms)
{
	osDelay(ms);
	return CarouselMotorResultAccept;
}
//==============================================================================
static CarouselMotorResult SetSpeed(MotorDriverT* driver, float speed)
{
	const uint16_t base_period;
	htim3.Instance->CCR2 = (uint16_t)((htim3.Instance->ARR + 1) * speed) - 1;
	htim3.Instance->CCR3 = (uint16_t)((htim3.Instance->ARR + 1) * speed) - 1;
	return CarouselMotorResultAccept;
}
//==============================================================================
float GetSpeed(void* driver)
{
	return 0.0;
}
//==============================================================================
CarouselMotorResult SetFrequency(MotorDriverT* driver, float frequency)
{
	return CarouselMotorResultAccept;
}
//==============================================================================
float GetFrequency(void* driver)
{
	return 0.0;
}
//==============================================================================
CarouselMotorResult SetEncoderPosition(MotorDriverT* driver, int position)
{
	
	return CarouselMotorResultAccept;
}
//==============================================================================
int GetEncoderPosition(MotorDriverT* driver)
{
	return 0;
}
//==============================================================================
static MOTOR_DRIVER_CONTROL_LAYER_DEF(Adapter,
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
CarouselMotorResult CarouselMotorAdapterInit(CarouselMotorAdapterT* adapter,
														void* parent)
{
	if (adapter)
	{
		if (!adapter->Description) { adapter->Description = "CarouselMotorAdapterT"; }
		adapter->Parent = parent;
		
		adapter->Driver.Options.HandlerUpdateFrequency = 1000;
		
		adapter->Driver.Options.StartSpeed = 0.1;
		adapter->Driver.Options.StopSpeed = 0.1;
		
		adapter->Driver.Options.Acceleration = 0.33;
		adapter->Driver.Options.Deceleration = 0.33;
		
		return MotorDriverInit(&adapter->Driver,
														adapter,
														&AdapterControl,
														0);
	}
	return CarouselMotorResultNullPointer;
}
//==============================================================================
//==============================================================================

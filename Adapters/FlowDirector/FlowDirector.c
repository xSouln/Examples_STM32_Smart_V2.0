//==============================================================================
#include <string.h>
#include <stdbool.h>
#include "FlowDirector/FlowDirector.h"
#include "main.h"
#include "tim.h"
#include "cmsis_os.h"
//==============================================================================
//==============================================================================
static FlowDirectorResult SetMotorState(MotorDriverT* driver, MotorDriverState state)
{
	switch((uint8_t)state)
	{
		case MotorDriverStateEnable:
			//HAL_TIM_Base_Start_IT(&htim2);
			break;
			
		case MotorDriverStateDisable:
			//HAL_TIM_Base_Stop_IT(&htim2);
			break;
	}
	
	return FlowDirectorResultAccept;
}
//==============================================================================
static FlowDirectorResult SetDriverState(MotorDriverT* driver, MotorDriverState state)
{
	switch((uint8_t)state)
	{
		case MotorDriverStateEnable:
			SERVO_ENABLE_GPIO_Port->ODR &= ~SERVO_ENABLE_Pin;
			HAL_TIM_Base_Start_IT(&htim2);
			break;
		
		case MotorDriverStateDisable:
			SERVO_ENABLE_GPIO_Port->ODR |= SERVO_ENABLE_Pin;
			HAL_TIM_Base_Stop_IT(&htim2);
			break;
	}
	return FlowDirectorResultAccept;
}
//==============================================================================
static void PWM_Handler(MotorDriverT* driver)
{
	TIM3->CCR1 = TIM3->ARR - (driver->Position * 2000 / 18000 + 500);
}
//==============================================================================
static void Handler(MotorDriverT* driver)
{
	
}
//==============================================================================
static FlowDirectorResult SetLockState(MotorDriverT* driver, MotorDriverMotorState state)
{
	
	return FlowDirectorResultAccept;
}
//==============================================================================
static FlowDirectorResult Delay(MotorDriverT* driver, uint32_t ms)
{
	osDelay(ms);
	return FlowDirectorResultAccept;
}
//==============================================================================
static FlowDirectorResult SetSpeed(MotorDriverT* driver, float speed)
{
	//const uint16_t base_period;
	//htim3.Instance->CCR2 = (uint16_t)((htim3.Instance->ARR + 1) * speed) - 1;
	//htim3.Instance->CCR3 = (uint16_t)((htim3.Instance->ARR + 1) * speed) - 1;
	return FlowDirectorResultAccept;
}
//==============================================================================
float GetSpeed(void* driver)
{
	return 0.0;
}
//==============================================================================
FlowDirectorResult SetFrequency(MotorDriverT* driver, float frequency)
{
	return FlowDirectorResultAccept;
}
//==============================================================================
float GetFrequency(void* driver)
{
	return 0.0;
}
//==============================================================================
static FlowDirectorResult SetPosition(MotorDriverT* driver, int position)
{
	
	return FlowDirectorResultAccept;
}
//==============================================================================
static int GetPosition(MotorDriverT* driver)
{
	return (int)driver->CalculatedValues.Steps;
}
//==============================================================================
static MOTOR_DRIVER_CONTROL_LAYER_DEF(Adapter,
	SetMotorState,
	SetSpeed,
	GetSpeed,
	SetFrequency,
	GetFrequency,
	SetDriverState,
	GetPosition,
	SetPosition,
	PWM_Handler,
	Handler,
	SetLockState,
	Delay
);
//==============================================================================
FlowDirectorResult FlowDirectorInit(FlowDirectorT* adapter, void* parent)
{
	if (adapter)
	{
		if (!adapter->Description) { adapter->Description = "FlowDirectorT"; }
		adapter->Parent = parent;
		
		adapter->Driver.Options.HandlerUpdateFrequency = 1000;
		
		adapter->Driver.Options.StartSpeed = 1000;
		adapter->Driver.Options.StopSpeed = 1000;
		
		adapter->Driver.Options.Acceleration = 2000;
		adapter->Driver.Options.Deceleration = 2000;
		
		adapter->Driver.Options.EnableDelay = 1000;
		adapter->Driver.Options.DisableDelay = 1000;
		
		return MotorDriverInit(&adapter->Driver,
														adapter,
														&AdapterControl,
														0);
	}
	return FlowDirectorResultNullPointer;
}
//==============================================================================
//==============================================================================

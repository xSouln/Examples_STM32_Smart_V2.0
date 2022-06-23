//==============================================================================
#include <string.h>
#include <stdbool.h>
#include "L298.h"
//==============================================================================
L298_Result L298_SetLockeState(L298_DriverT* driver, L298_State state)
{
	L298_Result result = driver->Control->SetLockState(driver, state);
	if (result == L298_ResultAccept)
	{
		driver->Status.IsLocked = state;
	}
	return result;
}
//==============================================================================
L298_Result L298_DriverEnable(L298_DriverT* driver)
{
	if (driver->Status.DriverState == L298_DriverStateDisable)
	{
		driver->EnableDelay = driver->Options.EnableDelay;
		driver->Status.DriverState = L298_DriverStateEnabling;
	}
	return L298_ResultAccept;
}
//==============================================================================
L298_Result L298_DriverDisable(L298_DriverT* driver)
{
	driver->Status.DriverState = L298_DriverStateDisable;
	return driver->Control->SetDriverState(driver, L298_StateDisable);
}
//==============================================================================
L298_Result L298_Stop(L298_DriverT* driver)
{
	driver->Status.MotorState = L298_MotorStateStopped;
	return driver->Control->SetMoveState(driver, L298_StateDisable);
}
//==============================================================================
void L298_Handler(L298_DriverT* driver)
{
	int diferense;
	
	switch((uint8_t)driver->Status.DriverState)
	{
		case L298_DriverStateEnabling:
			if (!driver->EnableDelay)
			{
				driver->Status.DriverState = L298_DriverStateIsEnable;
				driver->Control->SetDriverState(driver, L298_StateEnable);
			}
			else
			{
				driver->EnableDelay--;
			}
			return;
			
		case L298_DriverStateIsEnable:
			if (driver->Status.MotorState == L298_MotorStateStopped)
			{
				if (driver->DisableDelay)
				{
					driver->DisableDelay--;
				}
				else
				{
					 L298_DriverDisable(driver);
				}
				return;
			}
			break;
			
		case L298_DriverStateDisable:
			return;
	}
	
	switch((uint8_t)driver->Status.MotorState)
	{
		case L298_MotorStateStartingMove:
			driver->Status.MotorState = L298_MotorStateInMoving;
			driver->Control->SetMoveState(driver, L298_StateEnable);
			
		case L298_MotorStateInMoving:
			if (driver->MoveTime > driver->CalculatedValues.DecelerationTime)
			{
				if (driver->Speed > driver->Options.StopSpeed)
				{
					driver->Speed -= driver->CalculatedValues.DecelerationStep;
					if (driver->Speed < driver->Options.StopSpeed)
					{
						driver->Speed = driver->Options.StopSpeed;
					}
					driver->DebugValues.DecelerationTime++;
					driver->Control->SetSpeed(driver, driver->Speed);
				}
			}
			else if (driver->Speed < driver->SpeedRequest)
			{
				driver->Speed += driver->CalculatedValues.AccelerationStep;
				if (driver->Speed > driver->SpeedRequest)
				{
					driver->Speed = driver->SpeedRequest;
				}
				driver->DebugValues.AccelerationTime++;
				driver->Control->SetSpeed(driver, driver->Speed);
			}
			
			if (driver->MoveTime >= driver->MoveTimeRequest)
			{
				driver->Status.MotorState = L298_MotorStateStopped;
				driver->Control->SetMoveState(driver, L298_StateDisable);
			}
			else
			{
				driver->MoveTime += 1;
			}
			return;
			
		case L298_MotorStateStopped:
			return;
	}
}
//==============================================================================
void L298_PWM_Handler(L298_DriverT* driver)
{
	if (driver->Position != driver->PositionRequest)
	{
		driver->Position += driver->Diraction;
	}
	else
	{
		driver->Status.MotorState = L298_MotorStateStopped;
		driver->Control->SetMoveState(driver, L298_StateDisable);
	}
}
//==============================================================================
L298_Result L298_DriverInit(L298_DriverT* driver,
														void* parent,
														L298_ControlT* control,
														L298_OptionsT* options)
{
	if (driver && control)
	{
		if (!driver->Description)
		{
			driver->Description = "L298_DriverT";
		}
		
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
static L298_Result CalculationRacingFallingForSteps(L298_DriverT* driver)
{
	float steps = (driver->PositionRequest - driver->Position) * driver->Diraction;
  float speed = driver->Options.StartSpeed;
  float speed_offset = 0;
	uint32_t time_move = driver->MoveTimeRequest;
	
	driver->CalculatedValues.dxAccelerationSteps = 0;
	driver->CalculatedValues.dxDecelerationSteps = 0;
	
	driver->CalculatedValues.AccelerationTime = 0;
	driver->CalculatedValues.DecelerationTime = 0;
	
	driver->DebugValues.AccelerationTime = 0;
	driver->DebugValues.DecelerationTime = 0;
	
  while (speed < driver->SpeedRequest && steps > 0 && time_move)
  {
    if ((speed - speed_offset) > driver->Options.StopSpeed)
    {
			driver->CalculatedValues.dxDecelerationSteps += (speed - speed_offset) / 2;
			steps -= (speed - speed_offset) / 2;
      speed_offset += driver->CalculatedValues.DecelerationStep;
      driver->CalculatedValues.DecelerationTime++;
    }
    else
		{
			driver->CalculatedValues.dxAccelerationSteps += (speed) / 2;
			steps -= (speed) / 2;
      speed += driver->CalculatedValues.AccelerationStep;
      driver->CalculatedValues.AccelerationTime++;
    }
		
		time_move--;
	}
	
	if (steps < 0)
	{
		steps = 0;
	}
	
	//time_move = steps / speed;
	
	if (!time_move)
	{
		driver->CalculatedValues.DecelerationTime = driver->MoveTimeRequest - driver->CalculatedValues.DecelerationTime;
	}
	else
	{
		driver->CalculatedValues.DecelerationTime = driver->CalculatedValues.AccelerationTime + steps / speed;
	}
	
	return L298_ResultAccept;
}
//==============================================================================
L298_Result L298_SetPosition(L298_DriverT* driver,
														L298_MotorOptionsT* options,
														int position,
														float speed,
														int move_time)
{
	L298_Result result;
	driver->Control->SetLockState(driver, L298_StateEnable);
	
	if (driver->Status.MotorState != L298_MotorStateStopped)
	{
		result = L298_ResultBusy;
		goto end;
	}
	
	if (options)
	{
		memcpy((uint8_t*)&driver->Options, options, sizeof(L298_OptionsT));
	}
	
	if (move_time == 0)
	{
		result = L298_ResultInvalidParameter;
		goto end;
	}
	
	if (speed <= 0)
	{
		result = L298_ResultInvalidParameter;
		goto end;
	}
	
	if (driver->PositionRequest < position)
	{
		driver->Diraction = L298_MoveDiractionForward;
	}
	else if (driver->PositionRequest > position)
	{
		driver->Diraction = L298_MoveDiractionBackward;
	}
	else
	{
		result = L298_ResultInvalidParameter;
		goto end;
	}
	
	driver->MoveTime = 0;
	driver->MoveTimeRequest = move_time;
	driver->SpeedRequest = speed;
	driver->PositionRequest = position;
	driver->Speed = driver->Options.StartSpeed;
	driver->DisableDelay = driver->Options.DisableDelay;

	driver->CalculatedValues.AccelerationStep = driver->Options.Acceleration / driver->Options.HandlerUpdateFrequency;
	driver->CalculatedValues.DecelerationStep = driver->Options.Deceleration / driver->Options.HandlerUpdateFrequency;
	
	//CalculationRacingFallingForTime(driver);
	CalculationRacingFallingForSteps(driver);

	driver->Control->SetSpeed(driver, driver->Speed);
	//driver->Control->SetFrequency(driver, driver->Options.Frequency);
	driver->Control->SetPWMHandlerState(driver, L298_StateEnable);
	L298_DriverEnable(driver);
	driver->Status.MotorState = L298_MotorStateStartingMove;
	
	end:;
	driver->Control->SetLockState(driver, L298_StateDisable);
	return result;
}
//==============================================================================

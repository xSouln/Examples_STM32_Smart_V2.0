//==============================================================================
#include <string.h>
#include <stdbool.h>
#include "MotorDriver.h"
//==============================================================================
MotorDriverResult MotorDriverSetLockeState(MotorDriverT* driver, MotorDriverState state)
{
	MotorDriverResult result = driver->Control->SetLockState(driver, state);
	if (result == MotorDriverResultAccept)
	{
		driver->Status.IsLocked = state;
	}
	return result;
}
//==============================================================================
MotorDriverResult MotorDriverDriverEnable(MotorDriverT* driver)
{
	if (driver->Status.DriverState == MotorDriverDriverStateDisable)
	{
		driver->EnableDelay = driver->Options.EnableDelay;
		driver->Status.DriverState = MotorDriverDriverStateEnabling;
	}
	return MotorDriverResultAccept;
}
//==============================================================================
MotorDriverResult MotorDriverDriverDisable(MotorDriverT* driver)
{
	driver->Status.DriverState = MotorDriverDriverStateDisable;
	return driver->Control->SetDriverState(driver, MotorDriverStateDisable);
}
//==============================================================================
MotorDriverResult MotorDriverStop(MotorDriverT* driver)
{
	driver->Status.MotorState = MotorDriverMotorStateStopped;
	return driver->Control->SetMoveState(driver, MotorDriverStateDisable);
}
//==============================================================================
void MotorDriverHandler(MotorDriverT* driver)
{
	int diferense;
	
	switch((uint8_t)driver->Status.DriverState)
	{
		case MotorDriverDriverStateEnabling:
			if (!driver->EnableDelay)
			{
				driver->Status.DriverState = MotorDriverDriverStateIsEnable;
				driver->Control->SetDriverState(driver, MotorDriverStateEnable);
			}
			else
			{
				driver->EnableDelay--;
			}
			return;
			
		case MotorDriverDriverStateIsEnable:
			if (driver->Status.MotorState == MotorDriverMotorStateStopped)
			{
				if (driver->DisableDelay)
				{
					driver->DisableDelay--;
				}
				else
				{
					 MotorDriverDriverDisable(driver);
				}
				return;
			}
			break;
			
		case MotorDriverDriverStateDisable:
			return;
	}
	
	switch((uint8_t)driver->Status.MotorState)
	{
		case MotorDriverMotorStateStartingMove:
			driver->Status.MotorState = MotorDriverMotorStateInMoving;
			driver->Control->SetMoveState(driver, MotorDriverStateEnable);
			
		case MotorDriverMotorStateInMoving:
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
					driver->DebugValues.DeccelarationSteps = driver->Position * driver->Diraction;
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
				driver->DebugValues.AccelarationSteps = driver->Position * driver->Diraction;
			}
			
			if (driver->MoveTime >= driver->MoveTimeRequest)
			{
				driver->Status.MotorState = MotorDriverMotorStateStopped;
				driver->Control->SetMoveState(driver, MotorDriverStateDisable);
			}
			else
			{
				driver->MoveTime += 1;
			}
			return;
			
		case MotorDriverMotorStateStopped:
			return;
	}
}
//==============================================================================
void MotorDriverPWMHandler(MotorDriverT* driver)
{
	if (driver->Position != driver->PositionRequest)
	{
		driver->Position += driver->Diraction;
	}
	else
	{
		driver->Status.MotorState = MotorDriverMotorStateStopped;
		driver->Control->SetMoveState(driver, MotorDriverStateDisable);
	}
}
//==============================================================================
MotorDriverResult MotorDriverInit(MotorDriverT* driver,
														void* parent,
														MotorDriverControlT* control,
														MotorDriverOptionsT* options)
{
	if (driver && control)
	{
		if (!driver->Description)
		{
			driver->Description = "MotorDriverT";
		}
		
		driver->Parent = parent;
		driver->Control = control;
		
		if (options)
		{
			memcpy((uint8_t*)&driver->Options, options, sizeof(MotorDriverOptionsT));
		}
		
		driver->Status.IsInit = true;
		
		return MotorDriverResultAccept;
	}
	return MotorDriverResultNullPointer;
}
//==============================================================================
static MotorDriverResult CalculationRacingFallingForSteps(MotorDriverT* driver)
{
	float steps = (driver->PositionRequest - driver->Position) * driver->Diraction;
  float speed = driver->Options.StartSpeed;
  float speed_offset = 0;
	uint32_t time_move = driver->MoveTimeRequest;
	
	driver->CalculatedValues.AccelerationTime = 0;
	driver->CalculatedValues.DecelerationTime = 0;
	
	memset(&driver->DebugValues, 0, sizeof(driver->DebugValues));
	
  while (speed < driver->SpeedRequest && time_move)
  {
    if ((speed - speed_offset) > driver->Options.StopSpeed)
    {
      speed_offset += driver->CalculatedValues.DecelerationStep;
      driver->CalculatedValues.DecelerationTime++;
    }
    else
		{
      speed += driver->CalculatedValues.AccelerationStep;
      driver->CalculatedValues.AccelerationTime++;
    }
		
		time_move--;
	}
	
	driver->CalculatedValues.DecelerationTime = driver->MoveTimeRequest - driver->CalculatedValues.DecelerationTime;
	
	return MotorDriverResultAccept;
}
//==============================================================================
MotorDriverResult MotorDriverSetPosition(MotorDriverT* driver,
														MotorDriverMotorOptionsT* options,
														int position,
														float speed,
														int move_time)
{
	MotorDriverResult result;
	driver->Control->SetLockState(driver, MotorDriverStateEnable);
	
	if (driver->Status.MotorState != MotorDriverMotorStateStopped)
	{
		result = MotorDriverResultBusy;
		goto end;
	}
	
	if (options)
	{
		memcpy((uint8_t*)&driver->Options, options, sizeof(MotorDriverOptionsT));
	}
	
	if (move_time == 0)
	{
		result = MotorDriverResultInvalidParameter;
		goto end;
	}
	
	if (speed <= 0)
	{
		result = MotorDriverResultInvalidParameter;
		goto end;
	}
	
	if (driver->PositionRequest < position)
	{
		driver->Diraction = MotorDriverMoveDiractionForward;
	}
	else if (driver->PositionRequest > position)
	{
		driver->Diraction = MotorDriverMoveDiractionBackward;
	}
	else
	{
		result = MotorDriverResultInvalidParameter;
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
	driver->Control->SetPWMHandlerState(driver, MotorDriverStateEnable);
	MotorDriverDriverEnable(driver);
	driver->Status.MotorState = MotorDriverMotorStateStartingMove;
	
	end:;
	driver->Control->SetLockState(driver, MotorDriverStateDisable);
	return result;
}
//==============================================================================

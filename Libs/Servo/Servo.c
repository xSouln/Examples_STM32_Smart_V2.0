//==============================================================================
#include <string.h>
#include <stdbool.h>
#include "Servo.h"
//==============================================================================
//------------------------------------------------------------------------------
ServoResult ServoLock(ServoT* driver, ServoLockState state)
{
	if (driver && driver->Control && driver->Control->Lock)
	{
		return driver->Control->Lock(driver, state);
	}
	return ServoResultNullPointer;
}
//==============================================================================
//------------------------------------------------------------------------------
ServoResult ServoDriverEnable(ServoT* driver)
{
	switch ((uint8_t)driver->Status.DriverState)
	{
		case ServoDriverStateDisable:
			driver->Control->SetDriverState(driver, ServoStateEnable);
			driver->EnableDelay = driver->Options.EnableDelay;
			driver->Status.DriverState = ServoDriverStateEnabling;
			return ServoResultAccept;
		
		default : return ServoResultError;
	}
}
//------------------------------------------------------------------------------
ServoResult ServoDriverDisable(ServoT* driver)
{
	driver->Status.DriverState = ServoDriverStateDisable;
	return driver->Control->SetDriverState(driver, ServoStateDisable);
}
//==============================================================================
//------------------------------------------------------------------------------
ServoResult ServoStart(ServoT* driver)
{
	return driver->Control->SetMotorState(driver, ServoStateEnable);
}
//------------------------------------------------------------------------------
ServoResult ServoStop(ServoT* driver)
{
	driver->Status.MotorState = ServoMotorStateStopped;
	return driver->Control->SetMotorState(driver, ServoStateDisable);
}
//==============================================================================
void ServoHandler(ServoT* driver)
{
	int diferense;
	
	switch((uint8_t)driver->Status.DriverState)
	{
		case ServoDriverStateEnabling:
			if (!driver->EnableDelay)
			{
				driver->Control->SetDriverState(driver, ServoStateEnable);
				driver->Status.DriverState = ServoDriverStateIsEnable;
			}
			else
			{
				driver->EnableDelay--;
			}
			return;
			
		case ServoDriverStateIsEnable:
			if (driver->Status.MotorState == ServoMotorStateStopped)
			{
				if (driver->DisableDelay)
				{
					driver->DisableDelay--;
				}
				else
				{
					 ServoDriverDisable(driver);
				}
				return;
			}
			break;
			
		case ServoDriverStateDisable:
			return;
	}
}
//==============================================================================
ServoResult ServoInit(ServoT* driver,
														void* parent,
														ServoControlT* control,
														ServoOptionsT* options)
{
	if (driver && control)
	{
		if (!driver->Description)
		{
			driver->Description = "ServoT";
		}
		
		driver->Parent = parent;
		driver->Control = control;
		
		if (options)
		{
			memcpy((uint8_t*)&driver->Options, options, sizeof(ServoOptionsT));
		}
		
		driver->Status.IsInit = true;
		
		return ServoResultAccept;
	}
	return ServoResultNullPointer;
}
//==============================================================================
ServoResult ServoSetAngle(ServoT* driver, float angle, int move_time)
{
	ServoResult result;
	ServoLock(driver, ServoLockEnable);
	
	if (driver->Status.MotorState != ServoMotorStateStopped)
	{
		result = ServoResultBusy;
		goto end;
	}
	
	end:;
	ServoLock(driver, ServoLockDisable);
	return result;
}
//==============================================================================

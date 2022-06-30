//==============================================================================
#ifndef __SERVO_H
#define __SERVO_H
//==============================================================================
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
#include <stdint.h>
//------------------------------------------------------------------------------
#ifdef SERVO_USER_CONFIG
#include "ServoUserConfig.h" // override default macroses
#endif
//==============================================================================
#ifndef ServoResult
#define ServoResult int8_t
#endif
//------------------------------------------------------------------------------
#ifndef ServoResultAccept
#define ServoResultAccept 0
#endif
//------------------------------------------------------------------------------
#ifndef ServoResultError
#define ServoResultError -1
#endif
//------------------------------------------------------------------------------
#ifndef ServoResultBusy
#define ServoResultBusy -2
#endif
//------------------------------------------------------------------------------
#ifndef ServoResultNullPointer
#define ServoResultNullPointer -3
#endif
//------------------------------------------------------------------------------
#ifndef ServoResultInvalidParameter
#define ServoResultInvalidParameter -4
#endif
//=============================================================================
//------------------------------------------------------------------------------
typedef enum
{
	ServoStateDisable,
	ServoStateEnable
	
} ServoState;
//------------------------------------------------------------------------------
typedef enum
{
	ServoLockDisable,
	ServoLockEnable
	
} ServoLockState;
//------------------------------------------------------------------------------
typedef enum
{
	ServoMoveDiractionForward = 1,
	ServoMoveDiractionBackward = -1
	
} ServoMoveDiraction;
//------------------------------------------------------------------------------
typedef enum
{
	ServoHandlerStateInWork,
	ServoHandlerStateError
	
} ServoHandlerState;
//------------------------------------------------------------------------------
typedef enum
{
	ServoDriverStateDisable,
	ServoDriverStateEnabling,
	ServoDriverStateIsEnable,
	
} ServoDriverState;
//------------------------------------------------------------------------------
typedef enum
{
	ServoMotorStateStopped,
	ServoMotorStateStartingMove,
	ServoMotorStateInMoving,
	
} ServoMotorState;
//==============================================================================
typedef ServoResult (*ServoActionSetDriverState)(void* driver, ServoState);
typedef ServoResult (*ServoActionSetMotorState)(void* driver, ServoState);
typedef ServoResult (*ServoActionSetFrequency)(void* driver, float frequency);
typedef ServoResult (*ServoActionSetAngle)(void* driver, float angle);

typedef void (*ServoActionUserHandler)(void* driver);
																						
typedef int (*ServoActionGetEncoderPosition)(void* driver);
typedef ServoResult (*ServoActionSetEncoderPosition)(void* driver, int position);

typedef ServoResult (*ServoActionDelay)(void* driver, uint32_t ms);
typedef ServoResult (*ServoActionSetLock)(void* driver, ServoLockState state);
//------------------------------------------------------------------------------
typedef union
{
	struct
	{
		uint32_t IsInit : 1;
		uint32_t IsLocked : 1;
		ServoDriverState DriverState : 2;
		ServoMotorState MotorState : 2;
		ServoHandlerState HandlerState : 2;
	};
	
	uint32_t Value;
	
} ServoStatusT;
//------------------------------------------------------------------------------
typedef struct
{
	float MinAngle;
	float MaxAngle;
	
	uint16_t DisableDelay;
	uint16_t EnableDelay;
	
	uint32_t HandlerUpdateFrequency;
	
} ServoOptionsT;
//------------------------------------------------------------------------------
typedef struct
{
	ServoActionSetFrequency SetFrequency;
	
	ServoActionSetEncoderPosition SetEncoderPosition;
	ServoActionGetEncoderPosition GetEncoderPosition;
	
	ServoActionSetAngle SetAngle;
	ServoActionSetDriverState SetDriverState;
	ServoActionSetMotorState SetMotorState;
	
	ServoActionDelay Delay;
	ServoActionSetLock Lock;
	
} ServoControlT;
//------------------------------------------------------------------------------
#define SERVO_CONTROL_LAYER_DEF(Name,\
															SetFrequency,\
															SetEncoderPosition,\
															GetEncoderPosition,\
															SetAngle,\
															SetDriverState,\
															SetMotorState,\
															Delay,\
															Lock)\
ServoControlT Name##Control =\
{\
	.SetFrequency = (ServoActionSetFrequency)SetFrequency,\
	.SetEncoderPosition = (ServoActionSetEncoderPosition)SetEncoderPosition,\
	.GetEncoderPosition = (ServoActionGetEncoderPosition)GetEncoderPosition,\
	.SetAngle = (ServoActionSetAngle)SetAngle,\
	.SetDriverState = (ServoActionSetDriverState)SetDriverState,\
	.SetMotorState = (ServoActionSetMotorState)SetMotorState,\
	.Delay = (ServoActionDelay)Delay,\
	.Lock = (ServoActionSetLock)Lock,\
}
//------------------------------------------------------------------------------
typedef struct
{
	uint32_t AccelerationTime;
	uint32_t DecelerationTime;
	
	uint32_t AccelarationSteps;
	uint32_t DeccelarationSteps;
	
} ServoDebugValuesT;
//------------------------------------------------------------------------------
typedef struct
{
	char* Description;
	void* Parent;
	
	ServoControlT* Control;
	
	ServoStatusT Status;
	ServoOptionsT Options;
	ServoDebugValuesT DebugValues;
	
	int MoveTimeRequest;
	int MoveTime;
	
	float AngleRequest;
	float Angle;
	
	ServoMoveDiraction Diraction;
	
	uint16_t DisableDelay;
	uint16_t EnableDelay;
	
} ServoT;
//==============================================================================
ServoResult ServoInit(ServoT* driver,
														void* parent,
														ServoControlT* control,
														ServoOptionsT* options);
														
void ServoHandler(ServoT* driver);
														
ServoResult ServoSetAngle(ServoT* driver, float angle, int move_time);
ServoResult ServoStop(ServoT* driver);

ServoResult ServoLock(ServoT* driver, ServoLockState state);
//==============================================================================
#ifdef __cplusplus
}
#endif
//==============================================================================
#endif /* __SERVO_H */

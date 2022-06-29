//==============================================================================
#ifndef __MOTOR_DRIVER_H
#define __MOTOR_DRIVER_H
//==============================================================================
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
#include <stdint.h>
//------------------------------------------------------------------------------
#ifdef MOTOR_DRIVER_USER_CONFIG
#include "MotorDriverUserConfig.h" // override default macroses
#endif
//==============================================================================
#ifndef MotorDriverResult
#define MotorDriverResult int8_t
#endif
//------------------------------------------------------------------------------
#ifndef MotorDriverResultAccept
#define MotorDriverResultAccept 0
#endif
//------------------------------------------------------------------------------
#ifndef MotorDriverResultError
#define MotorDriverResultError -1
#endif
//------------------------------------------------------------------------------
#ifndef MotorDriverResultBusy
#define MotorDriverResultBusy -2
#endif
//------------------------------------------------------------------------------
#ifndef MotorDriverResultNullPointer
#define MotorDriverResultNullPointer -3
#endif
//------------------------------------------------------------------------------
#ifndef MotorDriverResultInvalidParameter
#define MotorDriverResultInvalidParameter -4
#endif
//==============================================================================
#ifndef MotorDriverDelay
#define MotorDriverDelay(ms)
#endif
//==============================================================================
#ifndef MotorDriverPWMHandleT
#define MotorDriverPWMHandleT void*
#endif
//=============================================================================
#define MOTOR_DRIVER_FORWARD_MAX_VALUE 	0x7fffffff
#define MOTOR_DRIVER_BACKWARD_MAX_VALUE 0x80000000
//=============================================================================
typedef enum
{
	MotorDriverMoveModeTime,
	MotorDriverMotorModePosition,
	MotorDriverMotorModeEncoder
	
} MotorDriverMoveMode;
//------------------------------------------------------------------------------
typedef enum
{
	MotorDriverStateDisable,
	MotorDriverStateEnable
	
} MotorDriverState;
//------------------------------------------------------------------------------
typedef enum
{
	MotorDriverMoveDiractionForward = 1,
	MotorDriverMoveDiractionBackward = -1
	
} MotorDriverMoveDiraction;
//------------------------------------------------------------------------------
typedef enum
{
	MotorDriverHandlerStateInWork,
	MotorDriverHandlerStateError
	
} MotorDriverHandlerState;
//------------------------------------------------------------------------------
typedef enum
{
	MotorDriverDriverStateDisable,
	MotorDriverDriverStateEnabling,
	MotorDriverDriverStateIsEnable,
	
} MotorDriverDriverState;
//------------------------------------------------------------------------------
typedef enum
{
	MotorDriverMotorStateStopped,
	MotorDriverMotorStateStartingMove,
	MotorDriverMotorStateInMoving,
	
} MotorDriverMotorState;
//==============================================================================
typedef MotorDriverResult (*MotorDriverActionSetMoveState)(void* driver, MotorDriverState state);

typedef MotorDriverResult (*MotorDriverActionSetSpeed)(void* driver, float speed);
typedef float (*MotorDriverActionGetSpeed)(void* driver);

typedef MotorDriverResult (*MotorDriverActionSetFrequency)(void* driver, float frequency);
typedef float (*MotorDriverActionGetFrequency)(void* driver);

typedef MotorDriverResult (*MotorDriverActionSetDriverState)(void* driver, MotorDriverState);
																						
typedef int (*MotorDriverGetEncoderPosition)(void* driver);
typedef MotorDriverResult (*MotorDriverSetEncoderPosition)(void* driver, int position);

typedef MotorDriverResult (*MotorDriverActionSetPWMHandlerState)(void* driver, MotorDriverState state);
typedef MotorDriverResult (*MotorDriverActionSetLockState)(void* driver, MotorDriverState);

typedef MotorDriverResult (*MotorDriverActionDelay)(void* driver, uint32_t ms);
//------------------------------------------------------------------------------
typedef union
{
	struct
	{
		uint32_t IsInit : 1;
		uint32_t IsLocked : 1;
		MotorDriverDriverState DriverState : 2;
		MotorDriverMotorState MotorState : 2;
		MotorDriverHandlerState HandlerState : 2;
	};
	
	uint32_t Value;
	
} MotorDriverStatusT;
//------------------------------------------------------------------------------
typedef struct
{
	float Acceleration;
	float Deceleration;
	
	float StartSpeed;
	float StopSpeed;
	
	uint16_t DisableDelay;
	uint16_t EnableDelay;
	
} MotorDriverMotorOptionsT;
//------------------------------------------------------------------------------
typedef struct
{
	float Acceleration;
	float Deceleration;
	
	float StartSpeed;
	float StopSpeed;
	
	uint16_t DisableDelay;
	uint16_t EnableDelay;
	
	uint32_t HandlerUpdateFrequency;
	
} MotorDriverOptionsT;
//------------------------------------------------------------------------------
typedef struct
{
	MotorDriverActionSetMoveState SetMoveState;
	
	MotorDriverActionSetSpeed SetSpeed;
	MotorDriverActionGetSpeed GetSpeed;
	
	MotorDriverActionSetFrequency SetFrequency;
	MotorDriverActionGetFrequency GetFrequency;
	
	MotorDriverActionSetDriverState SetDriverState;
	
	MotorDriverGetEncoderPosition GetEncoderPosition;
	MotorDriverSetEncoderPosition SetEncoderPosition;
	
	MotorDriverActionSetPWMHandlerState SetPWMHandlerState;
	MotorDriverActionSetLockState SetLockState;
	
	MotorDriverActionDelay Delay;
	
} MotorDriverControlT;
//------------------------------------------------------------------------------
#define MOTOR_DRIVER_CONTROL_LAYER_DEF(Name,\
															SetMoveState,\
															SetSpeed,\
															GetSpeed,\
															SetFrequency,\
															GetFrequency,\
															SetDriverState,\
															GetEncoderPosition,\
															SetEncoderPosition,\
															SetPWMHandlerState,\
															SetLockState,\
															Delay)\
MotorDriverControlT Name##Control =\
{\
	.SetMoveState = (MotorDriverActionSetMoveState)SetMoveState,\
	.SetSpeed = (MotorDriverActionSetSpeed)SetSpeed,\
	.GetSpeed = (MotorDriverActionGetSpeed)GetSpeed,\
	.SetFrequency = (MotorDriverActionSetFrequency)SetFrequency,\
	.GetFrequency = (MotorDriverActionGetFrequency)GetFrequency,\
	.SetDriverState = (MotorDriverActionSetDriverState)SetDriverState,\
	.GetEncoderPosition = (MotorDriverGetEncoderPosition)GetEncoderPosition,\
	.SetEncoderPosition = (MotorDriverSetEncoderPosition)SetEncoderPosition,\
	.SetPWMHandlerState = (MotorDriverActionSetPWMHandlerState)SetPWMHandlerState,\
	.SetLockState = (MotorDriverActionSetLockState)SetLockState,\
	.Delay = (MotorDriverActionDelay)Delay,\
}
//------------------------------------------------------------------------------
typedef struct
{
	float AccelerationStep;
	float DecelerationStep;
	
	uint32_t AccelerationTime;
	uint32_t DecelerationTime;
	
	float dxAccelerationSteps;
	float dxDecelerationSteps;
	
} MotorDriverCalculatedValuesT;
//------------------------------------------------------------------------------
typedef struct
{
	uint32_t AccelerationTime;
	uint32_t DecelerationTime;
	
	uint32_t AccelarationSteps;
	uint32_t DeccelarationSteps;
	
} MotorDriverDebugValuesT;
//------------------------------------------------------------------------------
typedef struct
{
	char* Description;
	void* Parent;
	
	MotorDriverControlT* Control;
	
	MotorDriverStatusT Status;
	MotorDriverOptionsT Options;
	MotorDriverCalculatedValuesT CalculatedValues;
	MotorDriverDebugValuesT DebugValues;
	
	int PositionRequest;
	int Position;
	
	int MoveTimeRequest;
	int MoveTime;
	
	float SpeedRequest;
	float Speed;
	
	MotorDriverMoveDiraction Diraction;
	
	uint16_t DisableDelay;
	uint16_t EnableDelay;
	
} MotorDriverT;
//==============================================================================
MotorDriverResult MotorDriverInit(MotorDriverT* driver,
														void* parent,
														MotorDriverControlT* control,
														MotorDriverOptionsT* options);
														
void MotorDriverPWMHandler(MotorDriverT* driver);
void MotorDriverHandler(MotorDriverT* driver);
														
MotorDriverResult MotorDriverSetPosition(MotorDriverT* driver,
														MotorDriverMotorOptionsT* options,
														int position,
														float speed,
														int move_time);

MotorDriverResult MotorDriverStop(MotorDriverT* driver);
MotorDriverResult MotorDriverSetDriverState(MotorDriverT* driver);
//==============================================================================
#ifdef __cplusplus
}
#endif
//==============================================================================
#endif /* __MOTOR_DRIVER_H */

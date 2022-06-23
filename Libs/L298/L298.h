//==============================================================================
#ifndef __L298_H
#define __L298_H
//==============================================================================
#ifdef __cplusplus
extern "C" {
#endif
//==============================================================================
#include <stdint.h>
//------------------------------------------------------------------------------
#ifdef L298_USER_CONFIG
#include "L298_UserConfig.h" // override default macroses
#endif
//==============================================================================
#ifndef L298_Result
#define L298_Result int8_t
#endif
//------------------------------------------------------------------------------
#ifndef L298_ResultAccept
#define L298_ResultAccept 0
#endif
//------------------------------------------------------------------------------
#ifndef L298_ResultError
#define L298_ResultError -1
#endif
//------------------------------------------------------------------------------
#ifndef L298_ResultBusy
#define L298_ResultBusy -2
#endif
//------------------------------------------------------------------------------
#ifndef L298_ResultNullPointer
#define L298_ResultNullPointer -3
#endif
//------------------------------------------------------------------------------
#ifndef L298_ResultInvalidParameter
#define L298_ResultInvalidParameter -4
#endif
//==============================================================================
#ifndef L298_Delay
#define L298_Delay(ms)
#endif
//==============================================================================
#ifndef L298_PWM_HandleT
#define L298_PWM_HandleT void*
#endif
//=============================================================================
#define L298_FORWARD_MAX_VALUE 	0x7fffffff
#define L298_BACKWARD_MAX_VALUE 0x80000000
//=============================================================================
typedef enum
{
	L298_MoveModeTime,
	L298_MotorModePosition,
	L298_MotorModeEncoder
	
} L298_MoveMode;
//------------------------------------------------------------------------------
typedef enum
{
	L298_StateDisable,
	L298_StateEnable
	
} L298_State;
//------------------------------------------------------------------------------
typedef enum
{
	L298_MoveDiractionForward = 1,
	L298_MoveDiractionBackward = -1
	
} L298_MoveDiraction;
//------------------------------------------------------------------------------
typedef enum
{
	L298_HandlerStateInWork,
	L298_HandlerStateError
	
} L298_HandlerState;
//------------------------------------------------------------------------------
typedef enum
{
	L298_DriverStateDisable,
	L298_DriverStateEnabling,
	L298_DriverStateIsEnable,
	
} L298_DriverState;
//------------------------------------------------------------------------------
typedef enum
{
	L298_MotorStateStopped,
	L298_MotorStateStartingMove,
	L298_MotorStateInMoving,
	
} L298_MotorState;
//==============================================================================
typedef L298_Result (*L298_ActionSetMoveState)(void* driver, L298_State state);

typedef L298_Result (*L298_ActionSetSpeed)(void* driver, float speed);
typedef float (*L298_ActionGetSpeed)(void* driver);

typedef L298_Result (*L298_ActionSetFrequency)(void* driver, float frequency);
typedef float (*L298_ActionGetFrequency)(void* driver);

typedef L298_Result (*L298_ActionSetDriverState)(void* driver, L298_State);
																						
typedef int (*L298_GetEncoderPosition)(void* driver);
typedef L298_Result (*L298_SetEncoderPosition)(void* driver, int position);

typedef L298_Result (*L298_ActionSetPWMHandlerState)(void* driver, L298_State state);
typedef L298_Result (*L298_ActionSetLockState)(void* driver, L298_State);

typedef L298_Result (*L298_ActionDelay)(void* driver, uint32_t ms);
//------------------------------------------------------------------------------
typedef union
{
	struct
	{
		uint32_t IsInit : 1;
		uint32_t IsLocked : 1;
		L298_DriverState DriverState : 2;
		L298_MotorState MotorState : 2;
		L298_HandlerState HandlerState : 2;
	};
	
	uint32_t Value;
	
} L298_StatusT;
//------------------------------------------------------------------------------
typedef struct
{
	float Acceleration;
	float Deceleration;
	
	float StartSpeed;
	float StopSpeed;
	
	uint16_t DisableDelay;
	uint16_t EnableDelay;
	
} L298_MotorOptionsT;
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
	
} L298_OptionsT;
//------------------------------------------------------------------------------
typedef struct
{
	L298_ActionSetMoveState SetMoveState;
	
	L298_ActionSetSpeed SetSpeed;
	L298_ActionGetSpeed GetSpeed;
	
	L298_ActionSetFrequency SetFrequency;
	L298_ActionGetFrequency GetFrequency;
	
	L298_ActionSetDriverState SetDriverState;
	
	L298_GetEncoderPosition GetEncoderPosition;
	L298_SetEncoderPosition SetEncoderPosition;
	
	L298_ActionSetPWMHandlerState SetPWMHandlerState;
	L298_ActionSetLockState SetLockState;
	
	L298_ActionDelay Delay;
	
} L298_ControlT;
//------------------------------------------------------------------------------
#define L298_CONTROL_LAYER_DEF(Name,\
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
L298_ControlT Name##Control =\
{\
	.SetMoveState = (L298_ActionSetMoveState)SetMoveState,\
	.SetSpeed = (L298_ActionSetSpeed)SetSpeed,\
	.GetSpeed = (L298_ActionGetSpeed)GetSpeed,\
	.SetFrequency = (L298_ActionSetFrequency)SetFrequency,\
	.GetFrequency = (L298_ActionGetFrequency)GetFrequency,\
	.SetDriverState = (L298_ActionSetDriverState)SetDriverState,\
	.GetEncoderPosition = (L298_GetEncoderPosition)GetEncoderPosition,\
	.SetEncoderPosition = (L298_SetEncoderPosition)SetEncoderPosition,\
	.SetPWMHandlerState = (L298_ActionSetPWMHandlerState)SetPWMHandlerState,\
	.SetLockState = (L298_ActionSetLockState)SetLockState,\
	.Delay = (L298_ActionDelay)Delay,\
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
	
} L298_CalculatedValuesT;
//------------------------------------------------------------------------------
typedef struct
{
	uint32_t AccelerationTime;
	uint32_t DecelerationTime;
	
} L298_DebugValuesT;
//------------------------------------------------------------------------------
typedef struct
{
	char* Description;
	void* Parent;
	
	L298_ControlT* Control;
	
	L298_StatusT Status;
	L298_OptionsT Options;
	L298_CalculatedValuesT CalculatedValues;
	L298_DebugValuesT DebugValues;
	
	int PositionRequest;
	int Position;
	
	int MoveTimeRequest;
	int MoveTime;
	
	float SpeedRequest;
	float Speed;
	
	L298_MoveDiraction Diraction;
	
	uint16_t DisableDelay;
	uint16_t EnableDelay;
	
} L298_DriverT;
//==============================================================================
L298_Result L298_DriverInit(L298_DriverT* driver,
														void* parent,
														L298_ControlT* control,
														L298_OptionsT* options);
														
void L298_PWM_Handler(L298_DriverT* driver);
void L298_Handler(L298_DriverT* driver);
														
L298_Result L298_SetPosition(L298_DriverT* driver,
														L298_MotorOptionsT* options,
														int position,
														float speed,
														int move_time);

L298_Result L298_Stop(L298_DriverT* driver);
L298_Result L298_SetDriverState(L298_DriverT* driver);
//==============================================================================
#ifdef __cplusplus
}
#endif
//==============================================================================
#endif /* __L298_H */

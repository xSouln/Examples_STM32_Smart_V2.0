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
//==============================================================================
enum L298_MotorNumberBits
{
	L298_Motor1Bit,
	L298_Motor2Bit,
	L298_MotorBitsCount
};
//------------------------------------------------------------------------------
typedef enum
{
	L298_Motor1 = 1 << L298_Motor1Bit,
	L298_Motor2 = 1 << L298_Motor2Bit,
	L298_MotorCount = L298_MotorBitsCount
	
} L298_MotorNumber;
//------------------------------------------------------------------------------
typedef enum
{
	L298_MotorStateStoped,
	L298_MotorStateInMove,
	L298_MotorStateOvercurrent,
	L298_MotorStatesCount
	
} L298_MotorState;
//==============================================================================
typedef L298_Result (*L298_ActionStartMove)(void* driver, L298_MotorNumber motor, int move_time);
typedef L298_Result (*L298_ActionStopMove)(void* driver, L298_MotorNumber motor);
typedef L298_Result (*L298_ActionSetSpeed)(void* driver, L298_MotorNumber motor, float speed);
typedef float (*L298_ActionGetSpeed)(void* driver, L298_MotorNumber motor);
typedef L298_Result (*L298_ActionLock)(void* driver);
typedef L298_Result (*L298_ActionUnlock)(void* driver);
//------------------------------------------------------------------------------
typedef union
{
	struct
	{
		uint32_t IsInit : 1;
		uint32_t IsLocked : 1;
		
		L298_MotorState Motor1 : L298_MotorStatesCount;
		L298_MotorState Motor2 : L298_MotorStatesCount;
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
	
} L298_MotorOptionsT;
//------------------------------------------------------------------------------
typedef struct
{
	L298_MotorOptionsT Motor1;
	L298_MotorOptionsT Motor2;
	
} L298_OptionsT;
//------------------------------------------------------------------------------
typedef struct
{
	L298_PWM_HandleT Handle;
	
	L298_ActionStartMove StartMove;
	L298_ActionStopMove StopeMove;
	
	L298_ActionSetSpeed SetSpeed;
	L298_ActionGetSpeed GetSpeed;
	
	L298_ActionLock Lock;
	L298_ActionUnlock Unlock;
	
} L298_ControlT;
//------------------------------------------------------------------------------
typedef struct
{
	char* Description;
	void* Parent;
	
	L298_ControlT* Control;
	
	L298_StatusT Status;
	L298_OptionsT Options;
	
	
} L298_DriverT;
//==============================================================================
L298_Result L298_DriverInit(L298_DriverT* driver,
														void* parent,
														L298_ControlT* control,
														L298_OptionsT* options);
//==============================================================================
#ifdef __cplusplus
}
#endif
//==============================================================================
#endif /* __L298_H */

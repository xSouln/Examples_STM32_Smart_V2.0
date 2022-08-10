//==============================================================================
#include "patterns_stm32h7xx/pattern_stm32h7xx_timer.h"
#include "stm32h7xx_hal.h"
//==============================================================================
TimStm32_T *xTimer1 = (TimStm32_T*)TIM1;
TimStm32_T *xTimer2 = (TimStm32_T*)TIM2;
TimStm32_T *xTimer3 = (TimStm32_T*)TIM3;
TimStm32_T *xTimer4 = (TimStm32_T*)TIM4;
TimStm32_T *xTimer5 = (TimStm32_T*)TIM5;
TimStm32_T *xTimer6 = (TimStm32_T*)TIM6;
TimStm32_T *xTimer7 = (TimStm32_T*)TIM7;
TimStm32_T *xTimer8 = (TimStm32_T*)TIM8;
//TimStm32_T *xTimer9 = (TimStm32_T*)TIM9;
//TimStm32_T *xTimer10 = (TimStm32_T*)TIM10;
extern TimStm32_T *xTimer12 = (TimStm32_T*)TIM12;
extern TimStm32_T *xTimer13 = (TimStm32_T*)TIM13;
extern TimStm32_T *xTimer14 = (TimStm32_T*)TIM14;
extern TimStm32_T *xTimer15 = (TimStm32_T*)TIM15;
extern TimStm32_T *xTimer16 = (TimStm32_T*)TIM16;
extern TimStm32_T *xTimer17 = (TimStm32_T*)TIM17;
//==============================================================================
inline void PATTERN_TIMER_INIT()
{
  xTimer1->SR; //motor 1 - PWM timer
  xTimer2->SR; //encoder 1
  xTimer3->SR; //motor 1 - period control
  xTimer4->SR; //motor 2 - period control
  xTimer5->SR; //encoder 2
  xTimer6->SR; //control timer
  xTimer7->SR; 
  xTimer8->SR; // motor 2 - PWM timer
  //xTimer9->SR;
  //xTimer10->SR;
  //xTimer11->SR;
  xTimer12->SR;
  xTimer13->SR;
  xTimer14->SR;
  xTimer15->SR;
  xTimer16->SR;
  xTimer17->SR;
}
//==============================================================================

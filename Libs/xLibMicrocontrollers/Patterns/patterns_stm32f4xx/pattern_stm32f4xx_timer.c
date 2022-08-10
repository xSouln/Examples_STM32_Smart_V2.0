//==============================================================================
#include "patterns_stm32f4xx/pattern_stm32f4xx_timer.h"
#include "stm32f4xx_hal.h"
//==============================================================================
TimStm32_T *xTimer1 = (TimStm32_T*)TIM1;
TimStm32_T *xTimer2 = (TimStm32_T*)TIM2;
TimStm32_T *xTimer3 = (TimStm32_T*)TIM3;
TimStm32_T *xTimer4 = (TimStm32_T*)TIM4;
TimStm32_T *xTimer5 = (TimStm32_T*)TIM5;
TimStm32_T *xTimer6 = (TimStm32_T*)TIM6;
TimStm32_T *xTimer7 = (TimStm32_T*)TIM7;
TimStm32_T *xTimer8 = (TimStm32_T*)TIM8;
TimStm32_T *xTimer9 = (TimStm32_T*)TIM9;
TimStm32_T *xTimer10 = (TimStm32_T*)TIM10;
//==============================================================================
inline void PATTERN_TIMER_INIT()
{
  xTimer1->SR;
  xTimer2->SR;
  xTimer3->SR;
  xTimer4->SR;
  xTimer5->SR;
  xTimer6->SR;
  xTimer7->SR;
  xTimer8->SR;
  xTimer9->SR;
  xTimer10->SR;
}
//==============================================================================

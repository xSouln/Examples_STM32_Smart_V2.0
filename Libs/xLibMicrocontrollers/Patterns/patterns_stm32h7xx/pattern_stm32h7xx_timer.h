/*
 * pattern_stm32h7xx_timer.h
 *
 * Created: 16.05.2019 14:29:38
 *  Author: rekuts
 */ 
//==============================================================================
#ifndef PATTERN_STM32H7XX_TIMER_H_
#define PATTERN_STM32H7XX_TIMER_H_
//==============================================================================
#include <stdint.h>
//==============================================================================
typedef struct{
  union{
    struct{
      /* 0x00000001 */ uint32_t CounterEnable: 1; //TIM_CR1_CEN: Counter enable
      /* 0x00000002 */ uint32_t UpdateDisable: 1; //TIM_CR1_UDIS: Update disable
      /* 0x00000004 */ uint32_t UpdateRequestSource: 1; //TIM_CR1_URS: Update request source
      /* 0x00000008 */ uint32_t OnePulseMode: 1; //TIM_CR1_OPM: One pulse mode
	  
      /* 0x00000010 */ uint32_t Direction: 1; //TIM_CR1_DIR: Direction
      /* 0x00000020 */ uint32_t AlignedModeSelection: 2; //TIM_CR1_DIR: CMS[1:0] bits (Center-aligned mode selection)
      /* 0x00000080 */ uint32_t ReloaPreloadEnable: 1; //TIM_CR1_ARPE: Auto-reload preload enable
	  
      /* 0x00000100 */ uint32_t ClockDivision: 2; //TIM_CR1_CKD: CKD[1:0] bits (clock division)
      /* 0x00000400 */ uint32_t Free10: 1; //TIM_CR1_CKD: CKD[1:0] bits (clock division)
      /* 0x00000800 */ uint32_t UpdateInterruptFlagRemap: 1; //TIM_CR1_UIFREMAP: Update interrupt flag remap
    };
    uint32_t Value;
  };
}TimCR1_T;
//==============================================================================
typedef struct{  
  union{
    struct{
      /* 0x00000001 */ uint32_t ComparePreloadedControl: 1; //TIM_CR2_CCPC: Capture/Compare Preloaded Control
      /* 0x00000002 */ uint32_t Free1: 1; //Free
      /* 0x00000004 */ uint32_t CompareControlUpdateSelection: 1; //TIM_CR2_CCUS: Capture/Compare Control Update Selection
      /* 0x00000008 */ uint32_t CompareDMA_Selection: 1; //TIM_CR2_CCDS: Capture/Compare DMA Selection
	  
      /* 0x00000010 */ uint32_t MasterModeSelection: 3; //TIM_CR2_MMS: Master Mode Selection
      /* 0x00000080 */ uint32_t TI1_Selection: 1; //TIM_CR2_TI1S: TI1 Selection
      
      /* 0x00000100 */ uint32_t OutputIdleState1: 1; //TIM_CR2_OIS1: Output Idle state 1 (OC1 output)
      /* 0x00000200 */ uint32_t OutputIdleStateN1: 1; //TIM_CR2_OIS1N: Output Idle state 1 (OC1N output)
      /* 0x00000400 */ uint32_t OutputIdleState2: 1; //TIM_CR2_OIS2: Output Idle state 2 (OC2 output)
      /* 0x00000800 */ uint32_t OutputIdleStateN2: 1; //TIM_CR2_OIS2N: Output Idle state 2 (OC2N output)
      
      /* 0x00001000 */ uint32_t OutputIdleState3: 1; //TIM_CR2_OIS3: Output Idle state 3 (OC3 output)
      /* 0x00002000 */ uint32_t OutputIdleStateN3: 1; //TIM_CR2_OIS3N: Output Idle state 3 (OC3N output)
      /* 0x00004000 */ uint32_t OutputIdleState4: 1; //TIM_CR2_OIS4: Output Idle state 4 (OC4 output)
      /* 0x00008000 */ uint32_t Free15: 1; //Free
	  
      /* 0x00010000 */ uint32_t OutputIdleState5: 1; //TIM_CR2_OIS5: Output Idle state 5 (OC5 output)
      /* 0x00020000 */ uint32_t OutputIdleState6: 1; //TIM_CR2_OIS6: Output Idle state 6 (OC6 output)
      /* 0x00040000 */ uint32_t Free18: 2; //Free
	  
      /* 0x00010000 */ uint32_t MasterModeSelection2: 4; //TIM_CR2_MMS2: Master Mode Selection
    };
    uint32_t Value;
  };
}TimCR2_T;
//==============================================================================
typedef struct{  
  union{
    struct{
      /* 0x00000001 */ uint32_t SlaveMode0: 3; //TIM_SMCR_SMS: SMS[2:0] bits (Slave mode selection)
      /* 0x00000008 */ uint32_t Free_1: 1; //Free_4
	  
      /* 0x00000010 */ uint32_t TriggerSelection0: 3; //TIM_SMCR_TS: Trigger selection
      /* 0x00000080 */ uint32_t MasterSlaveMode: 1; //TIM_SMCR_MSM: Master/slave mode
	  
      /* 0x00000100 */ uint32_t ExternalTriggerFilter: 4; //TIM_SMCR_ETF: ETF[3:0] bits (External trigger filter)
      
      /* 0x00001000 */ uint32_t ExternalTriggerPrescaler: 2; //TIM_SMCR_ETPS: ETPS[1:0] bits (External trigger prescaler)
      /* 0x00004000 */ uint32_t ExternalClockEnable : 1; //TIM_SMCR_ECE: External clock enable
      /* 0x00008000 */ uint32_t ExternalTriggerPolarity: 1; //TIM_SMCR_ETP: External trigger polarity
	  
      /* 0x00010000 */ uint32_t SlaveMode1: 2; //TIM_SMCR_SMS_3: Slave mode selection
      /* 0x00020000 */ uint32_t Free17_19 : 1; //Free_17_19
	  
      /* 0x00100000 */ uint32_t TriggerSelection1: 1; //TIM_SMCR_TS: Trigger selection
    };
    uint32_t Value;
  };
}TimSMCR_T;
//==============================================================================
typedef union{
  struct{
    /* 0x00000001 */ uint32_t UpdateInterruptEnable: 1; //TIM_DIER_UIE: Update interrupt enable
    /* 0x00000002 */ uint32_t Compare1InterruptEnable: 1; //TIM_DIER_CC1IE: Capture/Compare 1 interrupt enable
    /* 0x00000004 */ uint32_t Compare2InterruptEnable: 1; //TIM_DIER_CC2IE: Capture/Compare 2 interrupt enable
    /* 0x00000008 */ uint32_t Compare3InterruptEnable: 1; //TIM_DIER_CC3IE: Capture/Compare 3 interrupt enable
	
    /* 0x00000010 */ uint32_t Compare4InterruptEnable: 1; //TIM_DIER_CC4IE: Capture/Compare 4 interrupt enable
    /* 0x00000020 */ uint32_t COM_InterruptEnable: 1; //TIM_DIER_COMIE: COM interrupt enable
    /* 0x00000040 */ uint32_t TriggerInterruptEnable: 1; //TIM_DIER_TIE: Trigger interrupt enable
    /* 0x00000080 */ uint32_t BreakInterruptEnable: 1; //TIM_DIER_BIE: Break interrupt enable
    
    /* 0x00000100 */ uint32_t DMA_RequestEnable: 1; //TIM_DIER_UDE: Update DMA request enable
    /* 0x00000200 */ uint32_t DMA_RequestCompare1Enable: 1; //TIM_DIER_CC1DE: Capture/Compare 1 DMA request enable
    /* 0x00000400 */ uint32_t DMA_RequestCompare2Enable: 1; //TIM_DIER_CC2DE: Capture/Compare 2 DMA request enable
    /* 0x00000800 */ uint32_t DMA_RequestCompare3Enable: 1; //TIM_DIER_CC3DE: Capture/Compare 3 DMA request enable
	
    /* 0x00001000 */ uint32_t DMA_RequestCompare4Enable: 1; //TIM_DIER_CC4DE: Capture/Compare 4 DMA request enable
    /* 0x00002000 */ uint32_t COM_DMA_RequestEnable: 1; //TIM_DIER_COMDE: COM DMA request enable
    /* 0x00004000 */ uint32_t Trigger_DMA_RequestEnable: 1; //TIM_DIER_TDE: Trigger DMA request enable
  };
  uint32_t Value;
}TimDIER_T;
//==============================================================================
typedef union{
  struct{
    /* 0x00000001 */ uint32_t UpdateInterrupt: 1; //TIM_SR_UIF: Update interrupt Flag
    /* 0x00000002 */ uint32_t Compare1Interrupt: 1; //TIM_SR_CC1IF: Capture/Compare 1 interrupt Flag
    /* 0x00000004 */ uint32_t Compare2Interrupt: 1; //TIM_SR_CC2IF: Capture/Compare 2 interrupt Flag
    /* 0x00000008 */ uint32_t Compare3Interrupt: 1; //TIM_SR_CC3IF: Capture/Compare 3 interrupt Flag
	
    /* 0x00000010 */ uint32_t Compare4Interrupt: 1; //TIM_SR_CC4IF: Capture/Compare 4 interrupt Flag
    /* 0x00000020 */ uint32_t COM_Interrupt: 1; //TIM_SR_COMIF: COM interrupt Flag
    /* 0x00000040 */ uint32_t TriggerInterrupt: 1; //TIM_SR_TIF: Trigger interrupt Flag
    /* 0x00000080 */ uint32_t BreakInterrupt: 1; //TIM_SR_BIF: Break interrupt Flag
	
    /* 0x00000100 */ uint32_t Break2Interrupt: 1; //TIM_SR_B2IF: Break2 interrupt Flag
    /* 0x00000200 */ uint32_t Compare1Overcapture: 1; //TIM_SR_CC1OF: Capture/Compare 1 Overcapture Flag
    /* 0x00000400 */ uint32_t Compare2Overcapture: 1; //TIM_SR_CC2OF: Capture/Compare 2 Overcapture Flag
    /* 0x00000800 */ uint32_t Compare3Overcapture: 1; //TIM_SR_CC3OF: Capture/Compare 3 Overcapture Flag
	
    /* 0x00001000 */ uint32_t Compare4Overcapture: 1; //TIM_SR_CC4OF: Capture/Compare 4 Overcapture Flag
    /* 0x00002000 */ uint32_t SystemBreak: 1; //TIM_SR_SBIF: System Break Flag
    /* 0x00004000 */ uint32_t Free14_15: 1; //Free_13_15
    
    /* 0x00010000 */ uint32_t Compare5Interrupt: 1; //TIM_SR_CC5IF: Capture/Compare 5 interrupt Flag
    /* 0x00020000 */ uint32_t Compare6Interrupt: 1; //TIM_SR_CC6IF: Capture/Compare 6 interrupt Flag
  };
  uint32_t Value;
}TimSR_T;
//==============================================================================
typedef union{
  struct{
    /* 0x00000001 */ uint32_t UpdateGeneration: 1; //TIM_EGR_UG: Update Generation
    /* 0x00000002 */ uint32_t Compare1Generation : 1; //TIM_EGR_CC1G: Capture/Compare 1 Generation
    /* 0x00000004 */ uint32_t Compare2Generation : 1; //TIM_EGR_CC2G: Capture/Compare 2 Generation
    /* 0x00000008 */ uint32_t Compare3Generation: 1; //TIM_EGR_CC3G: Capture/Compare 3 Generation
	
    /* 0x00000010 */ uint32_t Compare4Generation: 1; //TIM_EGR_CC4G: Capture/Compare 4 Generation
    /* 0x00000020 */ uint32_t ControlUpdateGeneration : 1; //TIM_EGR_COMG: Capture/Compare Control Update Generation
    /* 0x00000040 */ uint32_t TriggerGeneration : 1; //TIM_EGR_TG: Trigger Generation
    /* 0x00000080 */ uint32_t BreakGeneration: 1; //TIM_EGR_BG: Break Generation
	
    /* 0x00000100 */ uint32_t Break2Generation: 1; //TIM_EGR_B2G: Break Generation
  };
  uint32_t Value;
}TimEGR_T;
//==============================================================================
typedef struct{
  union{
    struct{
      /* 0x00000001 */ uint32_t Compare1Selection: 2; //TIM_CCMR1_CC1S: CC1S[1:0] bits (Capture/Compare 1 Selection)
      /* 0x00000004 */ uint32_t Compare1FastEnable: 1; //TIM_CCMR1_OC1FE: Output Compare 1 Fast enable
      /* 0x00000008 */ uint32_t Compare1PreloadEnable: 1; //TIM_CCMR1_OC1PE: Output Compare 1 Preload enable
      
      /* 0x00000010 */ uint32_t Compare1OutputMode: 3; //TIM_CCMR1_OC1M: OC1M[2:0] bits (Output Compare 1 Mode)
      /* 0x00000080 */ uint32_t Compare1ClearEnable: 1; //TIM_CCMR1_OC1CE: Output Compare 1Clear Enable
      
      /* 0x00000100 */ uint32_t Compare2Selection: 2; //TIM_CCMR1_CC2S: CC2S[1:0] bits (Capture/Compare 2 Selection)
      /* 0x00000400 */ uint32_t Compare2FastEnable: 1; //TIM_CCMR1_OC2FE: Output Compare 2 Fast enable
      /* 0x00000800 */ uint32_t Compare2PreloadEnable: 1; //TIM_CCMR1_OC2PE: Output Compare 2 Preload enable
      
      /* 0x00001000 */ uint32_t Compare2OutputMode: 3; //TIM_CCMR1_OC2M: OC2M[2:0] bits (Output Compare 2 Mode)
      /* 0x00008000 */ uint32_t Compare2ClearEnable: 1; //TIM_CCMR1_OC2CE: Output Compare 2 Clear Enable
	  
      /* 0x00010000 */ uint32_t Compare1OutputMode1: 1; //TIM_CCMR1_OC1M_3: (Output Compare 1 Mode)
      /* 0x00020000 */ uint32_t Free13_15: 1; //Free_13_15
      
      /* 0x00100000 */ uint32_t Compare2OutputMode1: 1; //TIM_CCMR1_OC2M_3: (Output Compare 2 Mode)
    } Out;
	
	
    struct{
      /* 0x00000001 */ uint32_t Free0_1: 2; //Free_0_1
      /* 0x00000004 */ uint32_t Capture1Prescaler: 2; //TIM_CCMR1_IC1PSC: IC1PSC[1:0] bits (Input Capture 1 Prescaler)
      
      /* 0x00000010 */ uint32_t Capture1Filter: 4; //TIM_CCMR1_IC1F: IC1F[3:0] bits (Input Capture 1 Filter)
      
      /* 0x00000100 */ uint32_t Free8_9: 2; //Free_8_9
      /* 0x00000400 */ uint32_t Capture2Prescaler: 2; //TIM_CCMR1_IC2PSC: IC2PSC[1:0] bits (Input Capture 2 Prescaler)
      
      /* 0x00001000 */ uint32_t Capture2Filter: 4; //TIM_CCMR1_IC2F: IC2F[3:0] bits (Input Capture 2 Filter)
    } In;
	
    uint32_t Value;
  };
}TimCCMR1_T;
//==============================================================================
typedef struct{
  union{
    struct{
      /* 0x00000001 */ uint32_t Compare3Selection: 2; //TIM_CCMR2_CC3S: CC3S[1:0] bits (Capture/Compare 3 Selection)
      /* 0x00000004 */ uint32_t Compare3FastEnable: 1; //TIM_CCMR2_OC3FE: Output Compare 3 Fast enable
      /* 0x00000008 */ uint32_t Compare3PreloadEnable: 1; //TIM_CCMR2_OC3PE: Output Compare 3 Preload enable
      
      /* 0x00000010 */ uint32_t Compare3OutputMode: 3; //TIM_CCMR2_OC3M: OC3M[2:0] bits (Output Compare 3 Mode)
      /* 0x00000080 */ uint32_t Compare3ClearEnable: 1; //TIM_CCMR2_OC3CE: Output Compare 3 Clear Enable
      
      /* 0x00000100 */ uint32_t Compare4Selection: 2; //TIM_CCMR2_CC4S: CC4S[1:0] bits (Capture/Compare 4 Selection)
      /* 0x00000400 */ uint32_t Compare4FastEnable: 1; //TIM_CCMR2_OC4FE: Output Compare 4 Fast enable
      /* 0x00000800 */ uint32_t Compare4PreloadEnable: 1; //TIM_CCMR2_OC4PE: Output Compare 4 Preload enable
      
      /* 0x00001000 */ uint32_t Compare4OutputMode: 3; //TIM_CCMR2_OC4M: OC4M[2:0] bits (Output Compare 4 Mode)
      /* 0x00008000 */ uint32_t Compare4ClearEnable: 1; //TIM_CCMR2_OC4CE: Output Compare 4 Clear Enable
	  
      /* 0x00010000 */ uint32_t Compare3OutputMode1: 1; //TIM_CCMR2_OC3M_3: (Output Compare 3 Mode)
      /* 0x00020000 */ uint32_t Free13_15: 1; //Free_13_15
      
      /* 0x00100000 */ uint32_t Compare4OutputMode1: 1; //TIM_CCMR2_OC4M_3: (Output Compare 4 Mode)
    } Out;
	
    struct{
      /* 0x00000001 */ uint32_t Free0_1: 2; //Free_0_1
      /* 0x00000004 */ uint32_t Capture3Prescaler: 2; //TIM_CCMR2_IC3PSC: IC3PSC[1:0] bits (Input Capture 3 Prescaler)
      
      /* 0x00000010 */ uint32_t Capture3Filter: 4; //TIM_CCMR2_IC3F: IC3F[3:0] bits (Input Capture 3 Filter)
      
      /* 0x00000100 */ uint32_t Free8_9: 2; //Free_8_9
      /* 0x00000400 */ uint32_t Capture4Prescaler: 2; //TIM_CCMR2_IC4PSC: IC4PSC[1:0] bits (Input Capture 4 Prescaler)
      
      /* 0x00001000 */ uint32_t Capture4Filter: 4; //TIM_CCMR2_IC4F: IC4F[3:0] bits (Input Capture 4 Filter)
    } In;
	
    uint32_t Value;
  };
}TimCCMR2_T;
//==============================================================================
typedef union{
  struct{
    /* 0x00000001 */ uint32_t Compare1OutputEnable: 1; //TIM_CCER_CC1E: Capture/Compare 1 output enable
    /* 0x00000002 */ uint32_t Compare1Polarity : 1; //TIM_CCER_CC1P: Capture/Compare 1 output Polarity
    /* 0x00000004 */ uint32_t Compare1NOutputEnable : 1; //TIM_CCER_CC1NE: Capture/Compare 1 Complementary output enable
    /* 0x00000008 */ uint32_t Compare1NPolarity: 1; //TIM_CCER_CC1NP: Capture/Compare 1 Complementary output Polarity
    
    /* 0x00000010 */ uint32_t Compare2OutputEnable: 1; //TIM_CCER_CC2E: Capture/Compare 2 output enable
    /* 0x00000020 */ uint32_t Compare2Polarity : 1; //TIM_CCER_CC2P: Capture/Compare 2 output Polarity
    /* 0x00000040 */ uint32_t Compare2NOutputEnable : 1; //TIM_CCER_CC2NE: Capture/Compare 2 Complementary output enable
    /* 0x00000080 */ uint32_t Compare2NPolarity: 1; //TIM_CCER_CC2NP: Capture/Compare 2 Complementary output Polarity
    
    /* 0x00000100 */ uint32_t Compare3OutputEnable: 1; //TIM_CCER_CC3E: Capture/Compare 3 output enable
    /* 0x00000200 */ uint32_t Compare3Polarity : 1; //TIM_CCER_CC3P: Capture/Compare 3 output Polarity
    /* 0x00000400 */ uint32_t Compare3NOutputEnable : 1; //TIM_CCER_CC3NE: Capture/Compare 3 Complementary output enable
    /* 0x00000800 */ uint32_t Compare3NPolarity: 1; //TIM_CCER_CC3NP: Capture/Compare 3 Complementary output Polarity
    
    /* 0x00001000 */ uint32_t Compare4OutputEnable: 1; //TIM_CCER_CC4E: Capture/Compare 4 output enable
    /* 0x00002000 */ uint32_t Compare4Polarity : 1; //TIM_CCER_CC4P:Capture/Compare 4 output Polarity 
    /* 0x00004000 */ uint32_t Free14 : 1; //Free_14
    /* 0x00008000 */ uint32_t Compare4NPolarity: 1; //TIM_CCER_CC4NP: Capture/Compare 4 Complementary output Polarit
	
    /* 0x00010000 */ uint32_t Compare5OutputEnable: 1; //TIM_CCER_CC5E: Capture/Compare 5 output enable
    /* 0x00020000 */ uint32_t Compare5Polarity: 1; //TIM_CCER_CC5P: Capture/Compare 5 output Polarity
    /* 0x00040000 */ uint32_t Free18_19 : 2; //Free_14
    
    /* 0x00100000 */ uint32_t Compare6OutputEnable: 1; //TIM_CCER_CC6E: Capture/Compare 6 output enable
    /* 0x00200000 */ uint32_t Compare6Polarity: 1; //TIM_CCER_CC6P: Capture/Compare 6 output Polarity
  };
  uint32_t Value;
}TimCCER_T;
//==============================================================================
typedef union{
  struct{
    /* 0x00000001 */ uint32_t Counter: 32; //TIM_CNT_CNT: Counter Value
  };
  
  struct{
    /* 0x00000001 */ uint32_t Free0_30: 1; //TIM_CNT_UIFCPY: Update interrupt flag copy
    /* 0x80000000 */ uint32_t UpdateInterruptFlagCopy: 1; //TIM_CNT_UIFCPY: Update interrupt flag copy
  };
  
  uint32_t Value;
}TimCNT_T;
//==============================================================================
typedef union{
  struct{
    /* 0x00000001 */ uint32_t Prescaler: 16; //TIM_PSC_PSC: Prescaler Value
  };
  uint32_t Value;
}TimPSC_T;
//==============================================================================
typedef union{
  struct{
    /* 0x00000001 */ uint32_t Period: 32; //TIM_ARR_ARR: actual auto-reload Value
  };
  uint32_t Value;
}TimARR_T;
//==============================================================================
typedef union{
  struct{
    /* 0x00000001 */ uint32_t Repetition: 8; //TIM_RCR_REP: Repetition Counter Value
  };
  uint32_t Value;
}TimRCR_T;
//==============================================================================
typedef union{
  struct{
    /* 0x00000001 */ uint32_t Compare: 16; //TIM_CCR1_CCR1: RCapture/Compare 1 Value
  } Out;
  
  struct{
    /* 0x00000001 */ uint32_t Capture: 16; //TIM_CCR1_CCR1: RCapture/Compare 1 Value
  } In;
  uint32_t Value;
}TimCCR1_T;
//==============================================================================
typedef union{
  struct{
    /* 0x00000001 */ uint32_t Compare: 16; //TIM_CCR2_CCR2: RCapture/Compare 2 Value
  } Out;
  
  struct{
    /* 0x00000001 */ uint32_t Capture: 16; //TIM_CCR2_CCR2: RCapture/Compare 2 Value
  } In;
  uint32_t Value;
}TimCCR2_T;
//==============================================================================
typedef union{
  struct{
    /* 0x00000001 */ uint32_t Compare: 16; //TIM_CCR3_CCR3: RCapture/Compare 3 Value
  } Out;
  
  struct{
    /* 0x00000001 */ uint32_t Capture: 16; //TIM_CCR3_CCR3: RCapture/Compare 3 Value
  } In;
  uint32_t Value;
}TimCCR3_T;
//==============================================================================
typedef union{
  struct{
    /* 0x00000001 */ uint32_t Compare: 16; //TIM_CCR4_CCR4: RCapture/Compare 4 Value
  } Out;
  
  struct{
    /* 0x00000001 */ uint32_t Capture: 16; //TIM_CCR4_CCR4: RCapture/Compare 4 Value
  } In;
  uint32_t Value;
}TimCCR4_T;
//==============================================================================
typedef union{
  struct{
    /* 0x00000001 */ uint32_t Compare: 32; //TIM_CCR5_CCR5: RCapture/Compare 5 Value
  } Out;
  
  struct{
    /* 0x00000001 */ uint32_t Capture: 32; //TIM_CCR4_CCR4: RCapture/Compare 5 Value
  } In;
  
  struct{
    /* 0x00000001 */ uint32_t Free0_28: 1; //Free0_28
	
    /* 0x20000000 */ uint32_t GroupChannel5And1: 1; //TIM_CCR5_GC5C1: Group Channel 5 and Channel 1
    /* 0x40000000 */ uint32_t GroupChannel5And2: 1; //TIM_CCR5_GC5C2: Group Channel 5 and Channel 2
    /* 0x80000000 */ uint32_t GroupChannel5And3: 1; //TIM_CCR5_GC5C3: Group Channel 5 and Channel 3
  } Group;
  uint32_t Value;
}TimCCR5_T;
//==============================================================================
typedef union{
  struct{
    /* 0x00000001 */ uint32_t Compare: 16; //TIM_CCR4_CCR4: RCapture/Compare 4 Value
  } Out;
  
  struct{
    /* 0x00000001 */ uint32_t Capture: 16; //TIM_CCR4_CCR4: RCapture/Compare 4 Value
  } In;
  uint32_t Value;
}TimCCR6_T;
//==============================================================================
typedef union{
  struct{
    /* 0x00000001 */ uint32_t DeadTimeGenerator: 8; //TIM_BDTR_DTG: DTG[0:7] bits (Dead-Time Generator set-up)
    
    /* 0x00000100 */ uint32_t LockConfiguration: 2; //TIM_BDTR_LOCK: LOCK[1:0] bits (Lock Configuration)
    /* 0x00000400 */ uint32_t OffStateSelectionIdle : 1; //TIM_BDTR_OSSI: Off-State Selection for Idle mode
    /* 0x00000800 */ uint32_t OffStateSelectionRun: 1; //TIM_BDTR_OSSR: Off-State Selection for Run mode
    
    /* 0x00001000 */ uint32_t BreakEnable: 1; //TIM_BDTR_BKE: Break enable for Break
    /* 0x00002000 */ uint32_t BreakPolarity : 1; //TIM_BDTR_BKP: Break Polarity for Break
    /* 0x00004000 */ uint32_t AutomaticOutputEnable : 1; //TIM_BDTR_AOE: Automatic Output enable
    /* 0x00008000 */ uint32_t MainOutputEnable: 1; //TIM_BDTR_MOE: Main Output enable
	
    /* 0x00010000 */ uint32_t BreakFilter: 4; //TIM_BDTR_BKF: Break Filter for Break
    
    /* 0x00100000 */ uint32_t Break2Filter: 4; //TIM_BDTR_BK2F: Break Filter for Break2
    
    /* 0x01000000 */ uint32_t Break2Enable: 1; //TIM_BDTR_BK2E: Break enable for Break2
    /* 0x02000000 */ uint32_t Break2Polarity: 1; //TIM_BDTR_BK2P: Break Polarity for Break2
  };
  uint32_t Value;
}TimBDTR_T;
//==============================================================================
typedef union{
  struct{
    /* 0x00000001 */ uint32_t DMA_BaseAddress: 4; //TIM_DCR_DBA: DBA[4:0] bits (DMA Base Address)
    
    /* 0x00000010 */ uint32_t DMA_BurstLength: 4; //TIM_DCR_DBL: DBL[4:0] bits (DMA Burst Length)
  };
  uint32_t Value;
}TimDCR_T;
//==============================================================================
typedef union{
  struct{
    /* 0x00000001 */ uint32_t DMA_BurstAccesses: 16; //TIM_DMAR_DMAB: DMA register for burst accesses
  };
  uint32_t Value;
}TimDMAR_T;
//==============================================================================
typedef struct{
  volatile TimCR1_T CR1;                //TIM control register 1,                   Address offset: 0x00
  volatile TimCR2_T CR2;                //Reserved, 0x02
  
  volatile TimSMCR_T SMCR;              //TIM control register 2,                   Address offset: 0x04
  volatile TimDIER_T DIER;              //TIM slave mode control register,          Address offset: 0x08
  
  volatile TimSR_T SR;                  //TIM DMA/interrupt enable register,        Address offset: 0x0C
  volatile TimEGR_T EGR;                //TIM event generation register,            Address offset: 0x14
  
  volatile TimCCMR1_T CCMR1;            //TIM capture/compare mode register 1,      Address offset: 0x18
  volatile TimCCMR2_T CCMR2;            //TIM capture/compare mode register 2,      Address offset: 0x1C
  volatile TimCCER_T CCER;              //TIM capture/compare enable register,      Address offset: 0x20
  
  volatile uint32_t Counter;            //TIM counter register,                     Address offset: 0x24
  volatile uint32_t Prescaler;          //TIM prescaler,                            Address offset: 0x28
  volatile uint32_t Period;             //TIM auto-reload register,                 Address offset: 0x2C
  volatile uint32_t RepetitionCounter; 	//TIM repetition counter register,          Address offset: 0x30
  
  volatile uint32_t CaptureCompare1; 	//TIM capture/compare register 1,           Address offset: 0x34
  volatile uint32_t CaptureCompare2; 	//TIM capture/compare register 2,           Address offset: 0x38
  volatile uint32_t CaptureCompare3; 	//TIM capture/compare register 3,           Address offset: 0x3C
  volatile uint32_t CaptureCompare4; 	//TIM capture/compare register 4,           Address offset: 0x40
  
  volatile TimBDTR_T BDTR;              //TIM break and dead-time register,         Address offset: 0x44
  volatile TimDCR_T DCR;                //TIM DMA control register,                 Address offset: 0x48
  volatile uint32_t DMAR;               //TIM DMA address for full transfer,        Address offset: 0x4C
  
  volatile uint32_t RESERVED14;         //Reserved, 0x50
  
  volatile uint32_t CCMR3;              //TIM capture/compare mode register 3,      Address offset: 0x54
  volatile uint32_t CCR5;               //TIM capture/compare register5,            Address offset: 0x58
  volatile uint32_t CCR6;               //TIM capture/compare register6,            Address offset: 0x5C
  volatile uint32_t AF1;                //TIM alternate function option register 1, Address offset: 0x60
  volatile uint32_t AF2;                //TIM alternate function option register 2, Address offset: 0x64
  volatile uint32_t TISEL;              //TIM Input Selection register,             Address offset: 0x68
}TimStm32_T; //TIM_TypeDef
//==============================================================================
extern TimStm32_T *xTimer1;
extern TimStm32_T *xTimer2;
extern TimStm32_T *xTimer3;
extern TimStm32_T *xTimer4;
extern TimStm32_T *xTimer5;
extern TimStm32_T *xTimer6;
extern TimStm32_T *xTimer7;
extern TimStm32_T *xTimer8;
//extern TimStm32_T *xTimer9;
//extern TimStm32_T *xTimer10;
extern TimStm32_T *xTimer12;
extern TimStm32_T *xTimer13;
extern TimStm32_T *xTimer14;
extern TimStm32_T *xTimer15;
extern TimStm32_T *xTimer16;
extern TimStm32_T *xTimer17;
//==============================================================================
extern inline void PATTERN_TIMER_INIT();
//==============================================================================
#endif /* PATTERN_STM32H7XX_TIMER_H_ */

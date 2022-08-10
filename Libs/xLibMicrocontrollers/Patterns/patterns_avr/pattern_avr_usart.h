/*
 * pattern_avr_usart.h
 *
 * Created: 16.05.2019 14:29:38
 *  Author: rekuts
 */ 
//==============================================================================
#ifndef PATTERN_AVR_USART_H_
#define PATTERN_AVR_USART_H_
//==============================================================================
typedef union{
  struct{
    uint8_t Free: 3;
    uint8_t TxEnable: 1;
    uint8_t RxEnable: 1;
    uint8_t TxInterruptEnable: 1;
    uint8_t TxEndTransmitInterruptEnable: 1;
    uint8_t RxInterruptEnable: 1;
  };
  uint8_t Value;
}UsartRegB_T;
//==============================================================================
typedef union{
  struct{
    uint8_t MicroprocessorExchange: 1;
    uint8_t DoublingSpeed: 1;
    uint8_t ParityErrors: 1;
    uint8_t Overflowerror: 1;
    uint8_t CodingError: 1;
    uint8_t TxBufFree: 1;
    uint8_t TxComplite: 1;
    uint8_t RxComplite: 1;
  };
  uint8_t Value;
}UsartRegA_T;
//==============================================================================
#endif /* PATTERN_AVR_USART_H_ */

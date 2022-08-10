/*
 * xMacroces.h
 *
 * Created: 16.05.2019 14:29:38
 *  Author: rekuts
 */ 
//==============================================================================
#ifndef XMACROSES_H_
#define XMACROSES_H_
//==============================================================================
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
//==============================================================================
#ifdef macroces_stm32f4xx

#include "macroces_stm32f4xx/macroces_stm32f4xx_uart.h"

#endif
//==============================================================================
#define VA_ARGS_SHIFT(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, PN, ...) PN
#define VA_ARGS_COUNT(...) VA_ARGS_SHIFT(-1, ##__VA_ARGS__ ,9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
//==============================================================================
#define COPY_MEMORY(memory, object, size) memcpy((uint8_t*)memory, (uint8_t*)object, size)  
#define COPY_OBJECT(memory, object) memcpy((uint8_t*)memory, (uint8_t*)&object, sizeof(object))

#define CLEAR_MEMORY(object) memset(&object, 0, sizeof(object))
   
//inline static uint8_t* NEW_OBJECT(object, size)
//==============================================================================
/*
#define xRequestDef(name, size1, ...) \
xContent RequestContent_##name[VA_ARGS_COUNT(__VA_ARGS__)] = { { .obj = &#__VA_ARGS__, .size = sizeof(#__VA_ARGS__) } }; \
xObject RequestObjects_##name[VA_ARGS_COUNT(__VA_ARGS__)] = { &#__VA_ARGS__ }; \
uint8_t RequestObjectSize_##name[VA_ARGS_COUNT(__VA_ARGS__)] = { sizeof(#__VA_ARGS__) }; \
xRequestT Request_##name = { .Content = { 0 }, .Contents = RequestContent_##name }
*/
//==============================================================================
#endif /* XMACROSES_H_ */

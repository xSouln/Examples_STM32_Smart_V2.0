/*
 * BootTypes.h
 *
 * Created: 16.05.2019 14:29:38
 *  Author: rekuts
 */ 
//==============================================================================
#ifndef _BOOT_TYPES_H_
#define _BOOT_TYPES_H_
//==============================================================================
#include <stdint.h>
#include <stdbool.h>
//==============================================================================
#define BOOT_START_ADDRESS      0x08000000
#define BOOT_END_ADDRESS        0x08004000

#define APP_START_ADDRESS       0x080E0000
#define APP_END_ADDRESS         0x080FFFFF
//==============================================================================
#endif /* BOOT_TYPES_H_ */

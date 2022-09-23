//==============================================================================
#ifndef RGB_CUPS_CONFIG_H
#define RGB_CUPS_CONFIG_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
#include "WS2812/WS2812.h"
//==============================================================================
#define RGB_CUPS_PIXELS_COUNT_CUP1 8
#define RGB_CUPS_PIXELS_COUNT_CUP2 8
#define RGB_CUPS_PIXELS_COUNT_CUP3 8
#define RGB_CUPS_PIXELS_COUNT_CUP4 8

#define RGB_CUPS_SYNC_START_DATA_COUNT 50
#define RGB_CUPS_SYNC_END_DATA_COUNT 1

#define RGB_CUPS_DATA_BUFFER_SIZE_CUP1 (RGB_CUPS_PIXELS_COUNT_CUP1 * WS2812_BITS_IN_PIXEL)
#define RGB_CUPS_DATA_BUFFER_SIZE_CUP2 (RGB_CUPS_PIXELS_COUNT_CUP2 * WS2812_BITS_IN_PIXEL)
#define RGB_CUPS_DATA_BUFFER_SIZE_CUP3 (RGB_CUPS_PIXELS_COUNT_CUP2 * WS2812_BITS_IN_PIXEL)
#define RGB_CUPS_DATA_BUFFER_SIZE_CUP4 (RGB_CUPS_PIXELS_COUNT_CUP2 * WS2812_BITS_IN_PIXEL)
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif /* RGB_CUPS_CONFIG_H */

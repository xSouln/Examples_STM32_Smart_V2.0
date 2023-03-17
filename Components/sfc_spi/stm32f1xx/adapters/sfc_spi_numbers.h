//==============================================================================
//header:

#ifndef _SFC_SPI_NUMBERS_H_
#define _SFC_SPI_NUMBERS_H_
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif 
//==============================================================================
//includes:

#include "sfc_spi_config.h"
#include "spi.h"
//==============================================================================
//types:

typedef enum
{

#if	defined(SPI1) && defined(SFC_SPI_NUMBER1_ENABLE)
	SFC_SPI_NUMBER_1,
#endif

#if	defined(SPI2) && defined(SFC_SPI_NUMBER2_ENABLE)
	SFC_SPI_NUMBER_2,
#endif

#if	defined(SPI3) && defined(SFC_SPI_NUMBER3_ENABLE)
	SFC_SPI_NUMBER_3,
#endif

#if	defined(SPI4) && defined(SFC_SPI_NUMBER4_ENABLE)
	SFC_SPI_NUMBER_4,
#endif

#if	defined(SPI5) && defined(SFC_SPI_NUMBER5_ENABLE)
	SFC_SPI_NUMBER_5,
#endif

#if	defined(SPI6) && defined(SFC_SPI_NUMBER6_ENABLE)
	SFC_SPI_NUMBER_6,
#endif

#if	defined(SPI7) && defined(SFC_SPI_NUMBER7_ENABLE)
	SFC_SPI_NUMBER_7,
#endif

#if	defined(SPI8) && defined(SFC_SPI_NUMBER8_ENABLE)
	SFC_SPI_NUMBER_8,
#endif

#if	defined(SPI9) && defined(SFC_SPI_NUMBER9_ENABLE)
	SFC_SPI_NUMBER_9,
#endif

#if	defined(SPI10) && defined(SFC_SPI_NUMBER10_ENABLE)
	SFC_SPI_NUMBER_10,
#endif

#if	defined(SPI11) && defined(SFC_SPI_NUMBER11_ENABLE)
	SFC_SPI_NUMBER_11,
#endif

#if	defined(SPI12) && defined(SFC_SPI_NUMBER12_ENABLE)
	SFC_SPI_NUMBER_12,
#endif

	SFC_SPI_COUNT

} sfc_spi_number_t;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif //_SFC_SPI_CORE_H_

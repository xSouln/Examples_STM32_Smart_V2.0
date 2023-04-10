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

#if	defined(SPI1) && defined(SFC_SPI1_ENABLE)
	SFC_SPI1,
#endif

#if	defined(SPI2) && defined(SFC_SPI2_ENABLE)
	SFC_SPI2,
#endif

#if	defined(SPI3) && defined(SFC_SPI3_ENABLE)
	SFC_SPI3,
#endif

#if	defined(SPI4) && defined(SFC_SPI4_ENABLE)
	SFC_SPI4,
#endif

#if	defined(SPI5) && defined(SFC_SPI5_ENABLE)
	SFC_SPI5,
#endif

#if	defined(SPI6) && defined(SFC_SPI6_ENABLE)
	SFC_SPI6,
#endif

#if	defined(SPI7) && defined(SFC_SPI7_ENABLE)
	SFC_SPI7,
#endif

#if	defined(SPI8) && defined(SFC_SPI8_ENABLE)
	SFC_SPI8,
#endif

#if	defined(SPI9) && defined(SFC_SPI9_ENABLE)
	SFC_SPI9,
#endif

#if	defined(SPI10) && defined(SFC_SPI10_ENABLE)
	SFC_SPI10,
#endif

#if	defined(SPI11) && defined(SFC_SPI11_ENABLE)
	SFC_SPI11,
#endif

#if	defined(SPI12) && defined(SFC_SPI12_ENABLE)
	SFC_SPI12,
#endif

	SFC_SPI_COUNT

} sfc_spi_number_t;
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif //_SFC_SPI_CORE_H_

//==============================================================================
//includes:

#include "sfc_spi_core.h"
#include "sfc_spi/controls/sfc_spi.h"
//==============================================================================
//import:

//==============================================================================
//variables:

extern sfc_spi_handle_t sfc_spi_handles[];
//==============================================================================
//functions:

#if	defined(SPI1) && defined(SFC_SPI1_ENABLE)
void SPI1_IRQHandler(void)
{
	sfc_spi_handles[SFC_SPI1].irq(sfc_spi_handles[SFC_SPI1].spi);
}
#endif
//------------------------------------------------------------------------------
#if	defined(SPI4) && defined(SFC_SPI4_ENABLE)
void SPI4_IRQHandler(void)
{
	sfc_spi_handles[SFC_SPI_NUMBER_4].irq(sfc_spi_handles[SFC_SPI_NUMBER_4].spi);
}
#endif
//------------------------------------------------------------------------------
void sfc_spi_core_lock()
{
	__disable_irq();
}
//------------------------------------------------------------------------------
void sfc_spi_core_unlock()
{
	__enable_irq();
}
//------------------------------------------------------------------------------
void sfc_spi_core_clock_enable(sfc_spi_t* spi)
{
	sfc_spi_handle_t* handle = spi->handle;

	switch((int)handle->reg)
	{
#if	defined(SPI1) && defined(SFC_SPI1_ENABLE)
		case (int)SPI1:
			__HAL_RCC_SPI1_CLK_ENABLE();
			break;
#endif

#if	defined(SPI2) && defined(SFC_SPI2_ENABLE)
		case (int)SPI2:
			__HAL_RCC_SPI2_CLK_ENABLE();
			break;
#endif

#if	defined(SPI3) && defined(SFC_SPI3_ENABLE)
		case (int)SPI3:
			__HAL_RCC_SPI3_CLK_ENABLE();
			break;
#endif

#if	defined(SPI4) && defined(SFC_SPI4_ENABLE)
		case (int)SPI4:
			__HAL_RCC_SPI4_CLK_ENABLE();
			break;
#endif
	}
}
//------------------------------------------------------------------------------
void sfc_spi_core_clock_disable(sfc_spi_t* spi)
{
	sfc_spi_handle_t* handle = spi->handle;

	switch((int)handle->reg)
	{
#if	defined(SPI1) && defined(SFC_SPI1_ENABLE)
		case (int)SPI1:
			__HAL_RCC_SPI1_CLK_DISABLE();
			break;
#endif

#if	defined(SPI2) && defined(SFC_SPI2_ENABLE)
		case (int)SPI2:
			__HAL_RCC_SPI2_CLK_DISABLE();
			break;
#endif

#if	defined(SPI3) && defined(SFC_SPI3_ENABLE)
		case (int)SPI3:
			__HAL_RCC_SPI3_CLK_DISABLE();
			break;
#endif

#if	defined(SPI4) && defined(SFC_SPI4_ENABLE)
		case (int)SPI4:
			__HAL_RCC_SPI4_CLK_DISABLE();
			break;
#endif
	}
}
//------------------------------------------------------------------------------
sfc_spi_result_t sfc_spi_core_pin_mux_set(sfc_spi_t* spi, sfc_spi_pin_mux_t* mux)
{
	sfc_spi_result_t result = SFC_SPI_RESULT_OK;
	//sfc_spi_handle_t* handle = spi->handle;

	sfc_spi_core_lock();
	(void)mux;
	sfc_spi_core_unlock();

	return result;
}
//------------------------------------------------------------------------------
sfc_spi_result_t sfc_spi_core_pin_mux_dispose(sfc_spi_t* spi, sfc_spi_pin_mux_t* mux)
{
	sfc_spi_result_t result = SFC_SPI_RESULT_OK;
	//sfc_spi_handle_t* handle = spi->handle;

	sfc_spi_core_lock();
	(void)mux;
	sfc_spi_core_unlock();

	return result;
}
//------------------------------------------------------------------------------
void sfc_spi_core_interrupt_enable(sfc_spi_t* spi, uint8_t priority, uint8_t sub_priority)
{
	sfc_spi_handle_t* handle = spi->handle;

	HAL_NVIC_SetPriority(handle->IRQn, priority, sub_priority);
	HAL_NVIC_EnableIRQ(handle->IRQn);
}
//------------------------------------------------------------------------------
void sfc_spi_core_interrupt_disable(sfc_spi_t* spi)
{
	sfc_spi_handle_t* handle = spi->handle;

	HAL_NVIC_DisableIRQ(handle->IRQn);
}
//------------------------------------------------------------------------------
void sfc_spi_core_lock_bus(sfc_spi_t* spi)
{
	sfc_spi_handle_t* handle = spi->handle;

	sfc_spi_core_lock();

	if (handle->spi)
	{
		spi->state.bus_is_locked = true;
	}

	sfc_spi_core_unlock();
}
//------------------------------------------------------------------------------
void sfc_spi_core_unlock_bus(sfc_spi_t* spi)
{
	spi->state.bus_is_locked = true;
}
//------------------------------------------------------------------------------
//called after the transfer is completed
sfc_spi_result_t sfc_spi_core_give_bus(sfc_spi_t* spi)
{
	sfc_spi_handle_t* handle = spi->handle;

	sfc_spi_core_lock();

	if (!spi->state.bus_is_locked)
	{
		handle->spi = 0;
		spi->state.bus_is_own = false;
	}

	spi->state.is_transmitting = false;

	sfc_spi_core_unlock();

	return SFC_SPI_RESULT_OK;
}
//------------------------------------------------------------------------------
//binding "spi" to a handler for an asynchronous request
//upon completion of the asynchronous request, the sfc_spi_core_give_bus command should be called
sfc_spi_result_t sfc_spi_core_take_bus(sfc_spi_t* spi, uint32_t time_out)
{
	sfc_spi_handle_t* handle = spi->handle;
	uint32_t time_stamp = sfc_spi_get_time(spi);

	while (true)
	{
		if ((!handle->spi || handle->spi == spi) && !spi->state.is_transmitting)
		{
			sfc_spi_core_lock();

			if ((!handle->spi || handle->spi == spi) && !spi->state.is_transmitting)
			{
				break;
			}

			sfc_spi_core_unlock();
		}

		if (sfc_spi_get_time(spi) - time_stamp < time_out)
		{
			return SFC_SPI_RESULT_TIMEOUT;
		}
	}

	handle->spi = spi;
	handle->irq = spi->adapter->interface->irq;
	spi->state.bus_is_own = true;

	sfc_spi_core_unlock();

	return SFC_SPI_RESULT_OK;
}
//------------------------------------------------------------------------------
sfc_spi_result_t sfc_spi_core_bind_handle(sfc_spi_t* spi, sfc_spi_number_t number)
{
	spi->handle = sfc_spi_handles + number;

	return SFC_SPI_RESULT_OK;
}
//------------------------------------------------------------------------------
sfc_spi_handle_t sfc_spi_handles[] =
{
#if	defined(SPI1) && defined(SFC_SPI1_ENABLE)
	[SFC_SPI1] =
	{
		.reg = (REG_SPI_T*)SPI1,
		.IRQn = SPI1_IRQn,
	},
#endif

#if	defined(SPI2) && defined(SFC_SPI2_ENABLE)
	[SFC_SPI2] =
	{
		.reg = (REG_SPI_T*)SPI2,
		.IRQn = SPI2_IRQn,
	},
#endif

#if	defined(SPI3) && defined(SFC_SPI3_ENABLE)
	[SFC_SPI3] =
	{
		.reg = (REG_SPI_T*)SPI3,
		.IRQn = SPI3_IRQn,
	},
#endif

#if	defined(SPI4) && defined(SFC_SPI4_ENABLE)
	[SFC_SPI4] =
	{
		.reg = (REG_SPI_T*)SPI4,
		.IRQn = SPI4_IRQn,
	},
#endif
};
//==============================================================================

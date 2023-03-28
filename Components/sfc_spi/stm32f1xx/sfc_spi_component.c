//==============================================================================
//includes:

#include "sfc_spi_component.h"
#include "Components.h"

#include "sfc_spi/stm32f1xx/adapters/sfc_spi_adapter.h"
//==============================================================================
//defines:


//==============================================================================
//variables:

static uint32_t private_time_stamp1 = 0;

sfc_spi_t sfc_spi_wiz;
sfc_spi_t sfc_spi_ex2;
//==============================================================================
//import:


//==============================================================================
//functions:

static int sfc_spi_request_listener(sfc_spi_t* spi, sfc_spi_sys_request_selector selector, void* arg)
{
	switch((int)selector)
	{

		default: break;
	}

	return 0;
}
//------------------------------------------------------------------------------

static int sfc_spi_event_listener(sfc_spi_t* spi, sfc_spi_sys_event_selector selector, void* arg)
{
	switch ((uint8_t)selector)
	{
		default: return xResultRequestIsNotFound;
	}

	return xResultAccept;
}
//------------------------------------------------------------------------------
//component functions:

/**
 * @brief main handler
 */
void sfc_spi_component_handler()
{
	uint32_t time_stamp1 = ComponentsSysGetTime();

	if (time_stamp1 - private_time_stamp1 > 999)
	{
		private_time_stamp1 = time_stamp1;
	}
}
//==============================================================================
//initialization:

static sfc_spi_adapter_t sfc_spi_adapter_wiz =
{
	.spi_number = SFC_SPI_NUMBER_1,

	.cs_port = WIZ_CS_GPIO_Port,
	.cs_pin = WIZ_CS_Pin,

	.pin_mux =
	{
		.sck_port = WIZ_SCK_GPIO_Port,
		.sck_pin = WIZ_SCK_Pin,

		.miso_port = WIZ_MISO_GPIO_Port,
		.miso_pin = WIZ_MISO_Pin,

		.mosi_port = WIZ_MOSI_GPIO_Port,
		.mosi_pin = WIZ_MOSI_Pin
	}
};
//------------------------------------------------------------------------------

static sfc_spi_sys_interface_t sfc_sys_interface =
{
	.request_listener = (sfc_spi_sys_request_listener_t)sfc_spi_request_listener,
	.event_listener = (sfc_spi_sys_event_listener_t)sfc_spi_event_listener
};
//==============================================================================
//component initialization:

xResult sfc_spi_component_init(void* parent)
{
	sfc_spi_adapter_init(&sfc_spi_wiz, &sfc_spi_adapter_wiz);
	sfc_spi_init(&sfc_spi_wiz, parent, &sfc_sys_interface);

	sfc_spi_options_t options;
	options.clock_divider = SFC_SPI_CLOCK_DIVIDER_2;

	sfc_spi_set_options(&sfc_spi_wiz, &options);
  
	return 0;
}
//==============================================================================


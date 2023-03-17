//==============================================================================
//includes:

#include "sfc_spi_adapter.h"
#include "sfc_spi_core.h"
//==============================================================================
//defines:

#define SFC_SPI_ADAPTER_TYPE 0x3C
#define SFC_SPI_ADAPTER_SUBTYPE 0x00
#define SFC_SPI_ADAPTER_ID 0x3468
//==============================================================================
//types:


//==============================================================================
//variables:

static uint32_t private_temp;
//==============================================================================
//functions:

static void private_lock()
{
	sfc_spi_core_lock();
}
//------------------------------------------------------------------------------

static void private_unlock()
{
	sfc_spi_core_unlock();
}
//------------------------------------------------------------------------------
//
static void private_handler(sfc_spi_t* spi)
{

}
//------------------------------------------------------------------------------
static void private_irq(sfc_spi_t* spi)
{
	uint32_t rx_data;
	sfc_spi_handle_t* handle = spi->handle;
	REG_SPI_T* reg = (REG_SPI_T*)handle->reg;

	if (reg->Status.RxNotEmpty)
	{
		rx_data = reg->Data.Byte;

		if (handle->transfer.rx_data_size)
		{
			//revision is receive to convey the word and half-word
			*handle->transfer.rx_data = rx_data;

			handle->transfer.rx_data += handle->transfer.rx_data_increment;
			handle->transfer.rx_data_size -= handle->transfer.data_size_increment;
		}
	}

	if (reg->Control2.TxEmptyInterruptEnable && reg->Status.TxEmpty)
	{
		if (handle->transfer.tx_data_size)
		{
			//revision is transmit to convey the word and half-word
			reg->Data.Byte = *handle->transfer.tx_data;

			handle->transfer.tx_data += handle->transfer.tx_data_increment;
			handle->transfer.tx_data_size -= handle->transfer.data_size_increment;

			return;
		}
		else
		{
			reg->Control2.TxEmptyInterruptEnable = false;
		}
	}

	if (!handle->transfer.rx_data_size && !handle->transfer.tx_data_size)
	{
		//disabling the spi bus
		rx_data = handle->reg->Status.Value;
		reg->Control2.RxNotEmptyInterruptEnable = false;
		reg->Control2.TxEmptyInterruptEnable = false;

		spi->state.async_transmission_completed = true;

		//gives the bus, provided that the flag spi.state.bus_is_locked == false
		sfc_spi_core_give_bus(spi);
	}
}
//------------------------------------------------------------------------------
static sfc_spi_result_t private_request_listener(sfc_spi_t* spi, sfc_spi_adapter_request_selector_t selector, void* arg)
{
	sfc_spi_adapter_t* adapter = (sfc_spi_adapter_t*)spi->adapter;

	switch((uint8_t)selector)
	{
		case SFC_SPI_ADAPTER_REQUEST_APDATE_STATE:

			break;

		case SFC_SPI_ADAPTER_REQUEST_CHIP_SELECT:
			((GPIO_TypeDef*)adapter->cs_port)->ODR &= ~adapter->cs_pin;
			break;

		case SFC_SPI_ADAPTER_REQUEST_ABORT_TRANSFER:
			break;

		case SFC_SPI_ADAPTER_REQUEST_CHIP_DESELECT:
			((GPIO_TypeDef*)adapter->cs_port)->ODR |= adapter->cs_pin;
			break;

		case SFC_SPI_ADAPTER_REQUEST_APPLY_OPTIONS:
			break;

		case SFC_SPI_ADAPTER_REQUEST_LOCK:
			private_lock();
			break;

		case SFC_SPI_ADAPTER_REQUEST_UNLOCK:
			private_unlock();
			break;

		default: return SFC_SPI_RESULT_NOT_SUPORTED;
	}

	return SFC_SPI_RESULT_OK;
}
//------------------------------------------------------------------------------
static int private_event_listener(sfc_spi_t* spi, sfc_spi_adapter_event_selector_t selector, void* arg)
{
	return 0;
}
//------------------------------------------------------------------------------
static int private_transfer(sfc_spi_t* spi, sfc_spi_transfer_t* request)
{
	sfc_spi_handle_t* handle = spi->handle;
	REG_SPI_T* reg = (REG_SPI_T*)handle->reg;

	//getting access to use the bus before the end of the transfer
	if (sfc_spi_core_take_bus(spi, 1000) != SFC_SPI_RESULT_OK)
	{
		return 0;
	}

	//initializing the fifo buffer
	uint8_t* rx_data = request->rx_data;
	uint8_t rx_data_increment = sizeof(uint8_t);
	uint8_t* tx_data = request->tx_data;
	uint8_t tx_data_increment = sizeof(uint8_t);
	int data_size = request->data_size;
	uint8_t data_size_increment = sizeof(uint8_t);
	uint32_t transfer_time_stamp = sfc_spi_get_time(spi);

	//if there is no receive buffer, we set dummy values
	if (!rx_data)
	{
		rx_data = (uint8_t*)&private_temp;
		rx_data_increment = 0;
	}

	//if there is no transmit buffer, we set dummy values
	if (!tx_data)
	{
		tx_data = (uint8_t*)&private_temp;
		tx_data_increment = 0;
	}

	//set flags
	spi->transfer_result = SFC_SPI_RESULT_NO_ERROR;
	spi->state.is_transmitting = true;

	//preparation of spi. the order of inclusion is important
	private_temp = reg->Data.Word;
	handle->reg->Status.Value = 0;
	reg->Control1.SpiEnable = true;

	while(data_size)
	{
		while(!reg->Status.TxEmpty){ };
		reg->Data.Byte = *tx_data;

		while(reg->Status.Busy){ };

		while(!reg->Status.RxNotEmpty){ };
		*rx_data = reg->Data.Byte;

		tx_data += tx_data_increment;
		rx_data += rx_data_increment;

		data_size -= data_size_increment;

		if (sfc_spi_get_time(spi) - transfer_time_stamp > request->timeout)
		{
			spi->transfer_result = SFC_SPI_RESULT_TIME_OUT;
			break;
		}
	}

	private_temp = reg->Data.Word;
	handle->reg->Status.Value = 0;
	reg->Control1.SpiEnable = true;

	//gives the bus, provided that the flag spi.state.bus_is_locked == false
	sfc_spi_core_give_bus(spi);

	return SFC_SPI_RESULT_OK;
}
//------------------------------------------------------------------------------
static sfc_spi_result_t private_transfer_async(sfc_spi_t* spi, sfc_spi_transfer_t* request)
{
	sfc_spi_handle_t* handle = spi->handle;
	REG_SPI_T* reg = (REG_SPI_T*)handle->reg;

	//getting access to use the bus before the end of the transfer
	if (sfc_spi_core_take_bus(spi, 1000) != SFC_SPI_RESULT_OK)
	{
		return SFC_SPI_RESULT_TIMEOUT;
	}

	//initializing the fifo buffer
	handle->transfer.rx_data = request->rx_data;
	handle->transfer.rx_data_size = request->data_size;
	handle->transfer.rx_data_increment = sizeof(uint8_t);
	handle->transfer.tx_data = request->tx_data;
	handle->transfer.tx_data_size = request->data_size;
	handle->transfer.tx_data_increment = sizeof(uint8_t);
	handle->transfer.data_size_increment = sizeof(uint8_t);
	handle->transfer.timeout = request->timeout;

	//if there is no receive buffer, we set dummy values
	if (!handle->transfer.rx_data)
	{
		handle->transfer.rx_data = (uint8_t*)&private_temp;
		handle->transfer.rx_data_increment = 0;
	}

	//if there is no transmit buffer, we set dummy values
	if (!handle->transfer.tx_data)
	{
		handle->transfer.tx_data = (uint8_t*)&private_temp;
		handle->transfer.tx_data_increment = 0;
	}

	//set flags
	spi->transfer_result = SFC_SPI_RESULT_NO_ERROR;
	spi->state.async_transmission_completed = false;
	spi->state.is_transmitting = true;

	//preparation of spi. the order of inclusion is important
	private_temp = handle->reg->Data.Word;
	handle->reg->Status.Value = 0;
	reg->Control1.SpiEnable = true;

	reg->Control2.RxNotEmptyInterruptEnable = true;
	reg->Control2.TxEmptyInterruptEnable = true;

	return SFC_SPI_RESULT_OK;
}
//------------------------------------------------------------------------------
static sfc_spi_result_t private_transfer_async_dma(sfc_spi_t* spi, sfc_spi_transfer_t* request)
{
	return SFC_SPI_RESULT_NOT_SUPORTED;
}
//------------------------------------------------------------------------------
static sfc_spi_result_t private_set_options(sfc_spi_t* spi, sfc_spi_options_t* request)
{
	//sfc_spi_adapter_t* adapter = (sfc_spi_adapter_t*)spi->adapter;

	return SFC_SPI_RESULT_NOT_SUPORTED;
}
//==============================================================================
static const sfc_spi_adapter_interface_t adapter_interface =
{
	.handler = (sfc_spi_handler_t)private_handler,
	.irq = (sfc_spi_irq_t)private_irq,

	.request_listener = (sfc_spi_request_listener_t)private_request_listener,
	.event_listener = (sfc_spi_event_listener_t)private_event_listener,

	.transfer = (sfc_spi_transfer_action_t)private_transfer,
	.transfer_async = (sfc_spi_transfer_action_async_t)private_transfer_async,
	.transfer_async_dma = (sfc_spi_transfer_action_async_t)private_transfer_async_dma,

	.set_options = (sfc_spi_set_options_t)private_set_options
};
//==============================================================================
//initialization:

sfc_spi_result_t sfc_spi_adapter_init(sfc_spi_t* spi, sfc_spi_adapter_t* adapter)
{
	//description of the object
	spi->adapter = (sfc_spi_adapter_base_t*)adapter;
	spi->adapter->object.type = SFC_SPI_ADAPTER_TYPE;
	spi->adapter->object.sub_type = SFC_SPI_ADAPTER_SUBTYPE;
	spi->adapter->object.identifier = SFC_SPI_ADAPTER_ID;

	//linking objects
	spi->adapter->object.parent = spi;
	spi->adapter->interface = (sfc_spi_adapter_interface_t*)&adapter_interface;

	//binding the spi bus to an instance
	sfc_spi_core_bind_handle(spi, adapter->spi_number);

	sfc_spi_core_clock_enable(spi);

	sfc_spi_core_pin_mux_set(spi, &adapter->pin_mux);

	//enabling interrupts
	sfc_spi_core_interrupt_enable(spi, 5, 0);

	return SFC_SPI_RESULT_OK;
}
//==============================================================================

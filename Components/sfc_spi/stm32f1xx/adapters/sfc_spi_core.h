//==============================================================================
//header:

#ifndef _SFC_SPI_CORE_H_
#define _SFC_SPI_CORE_H_
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif 
//==============================================================================
//includes:

#include "sfc_spi_registers.h"
#include "sfc_spi/controls/sfc_spi_types.h"
#include "sfc_spi_numbers.h"
//==============================================================================
//types:

typedef struct
{
	uint8_t* tx_data;
	uint8_t* rx_data;

	int tx_data_size;
	int rx_data_size;

	struct
	{
		uint16_t tx_data_increment : 4;
		uint16_t rx_data_increment : 4;
		uint16_t data_size_increment : 4;
	};

	uint32_t timeout;

	void* content;

} sfc_spi_transfer_fifo_t;
//------------------------------------------------------------------------------
//interface for working with the target spi
typedef struct
{
	void (*sfc_spi_core_lock)();
	void (*sfc_spi_core_unlock)();

	void (*sfc_spi_core_interrupt_enable)(sfc_spi_t* spi, uint8_t priority, uint8_t sub_priority);
	void (*sfc_spi_core_interrupt_disable)(sfc_spi_t* spi);

	void (*clock_enable)(sfc_spi_t* spi);
	void (*clock_disable)(sfc_spi_t* spi);

	sfc_spi_result_t (*pin_mux_set)(sfc_spi_t* spi, sfc_spi_pin_mux_t* mux);
	void (*pin_mux_dispose)(sfc_spi_t* spi, sfc_spi_pin_mux_t* mux);

} sfc_spi_core_interface_t;
//------------------------------------------------------------------------------

// handle for async operations
typedef struct
{
	//binding spi reg
	REG_SPI_T* reg;

	//interface for working with the target spi
	//const sfc_spi_core_interface_t* interface;

	const uint32_t IRQn;

	//the spi that caused the asynchronous transfer operation
	sfc_spi_t* spi;

	//taken out separately to optimize the function call
	sfc_spi_irq_t irq;

	sfc_spi_transfer_fifo_t transfer;

	sfc_spi_pin_mux_t* total_pin_mux;

	//the time of the start of the transfer
	uint32_t transfer_time_stamp;

	void* content;

} sfc_spi_handle_t;
//==============================================================================
//export:

//==============================================================================
//functions:

void sfc_spi_core_lock();
void sfc_spi_core_unlock();

void sfc_spi_core_lock_bus(sfc_spi_t* spi);
void sfc_spi_core_unlock_bus(sfc_spi_t* spi);

void sfc_spi_core_clock_enable(sfc_spi_t* spi);
void sfc_spi_core_clock_disable(sfc_spi_t* spi);

void sfc_spi_core_interrupt_enable(sfc_spi_t* spi, uint8_t priority, uint8_t sub_priority);
void sfc_spi_core_interrupt_disable(sfc_spi_t* spi);

sfc_spi_result_t sfc_spi_core_take_bus(sfc_spi_t* spi, uint32_t time_out);
sfc_spi_result_t sfc_spi_core_give_bus(sfc_spi_t* spi);

sfc_spi_result_t sfc_spi_core_pin_mux_set(sfc_spi_t* spi, sfc_spi_pin_mux_t* mux);
sfc_spi_result_t sfc_spi_core_pin_mux_dispose(sfc_spi_t* spi, sfc_spi_pin_mux_t* mux);

sfc_spi_result_t sfc_spi_core_bind_handle(sfc_spi_t* spi, sfc_spi_number_t number);
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif //_SFC_SPI_CORE_H_

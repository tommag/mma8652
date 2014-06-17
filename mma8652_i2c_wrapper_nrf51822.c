/* MMA8652 C library - small 3-axis digital accelerometer from Freescale
 * 
 * This file contains the hardware dependent functions for the I2C module
 * Hardware functions for the nRF51822 MCU from Nordic, for a specific application
 * Provided as reference only
 * 
 * 2014 - Tom Magnier : tom@tmagnier.fr
 */

#include "mma8652.h"
#include "debug.h" //Contains the debug_log macro
#include "twi_master.h"

bool mma8652_i2c_init(void)
{
	return twi_master_init();
}

bool mma8652_i2c_register_write(uint8_t register_address, uint8_t value)
{
	uint8_t write_address = MMA8652_I2C_ADDR << 1;
	uint8_t write_buffer[2];
	
	//Transmit register address first, then value to write
	write_buffer[0] = register_address;
	write_buffer[1] = value;
	
	return twi_master_transfer(write_address, write_buffer, 2, TWI_ISSUE_STOP);
}

bool mma8652_i2c_register_read(uint8_t first_reg_address, uint8_t * p_destination, uint8_t num_bytes)
{
	uint8_t write_address = MMA8652_I2C_ADDR << 1;
	uint8_t read_address = MMA8652_I2C_ADDR << 1 | TWI_READ_BIT;
	bool success;
	
	//Send first register address
	success = twi_master_transfer(write_address, &first_reg_address, 1, TWI_DONT_ISSUE_STOP);
	if(!success)
		return false;
	
	//Read multiple registers in a row
	success = twi_master_transfer(read_address, p_destination, num_bytes, TWI_ISSUE_STOP);
	
	return success;
}

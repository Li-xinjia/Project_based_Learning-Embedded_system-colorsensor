/*
 * my_i2c.h
 *
 *  Created on: 2013/10/04
 */

#ifndef MY_I2C_H_
#define MY_I2C_H_

#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_i2c.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"

//*****************************************************************************
//
// Set the address for slave module. This is a 7-bit address sent in the
// following format:
//                      [A6:A5:A4:A3:A2:A1:A0:RS]
//
// A zero in the R/S position of the first byte means that the master
// transmits (sends) data to the selected slave, and a one in this position
// means that the master receives data from the slave.
//
//*****************************************************************************

//*****************************************************************************
// initialize I2C
//*****************************************************************************
extern inline void initI2C(uint32_t base);
//*****************************************************************************
// write 8 bit data on I2C
//*****************************************************************************
extern inline void writeByteI2C(uint32_t base,uint8_t slave_address,uint8_t data);
//*****************************************************************************
// write multiple data on I2C
//*****************************************************************************
extern inline void writeDataI2C(uint32_t base,uint8_t slave_address,uint8_t *data,uint8_t length);
//*****************************************************************************
// read 8 bit data on I2C
//*****************************************************************************
extern inline uint8_t readByteI2C(uint32_t base, uint8_t slave_address);
//*****************************************************************************
// read multiple data on I2C
//*****************************************************************************
extern inline uint8_t readDataI2C(uint32_t base, uint8_t slave_address, uint8_t *data, uint8_t length);

#endif

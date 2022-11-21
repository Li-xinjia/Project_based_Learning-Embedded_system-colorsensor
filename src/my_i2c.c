/*
 * my_i2c.c
 *
 *  Created on: Oct 10, 2020
 *      Author: omzn
 */

#include "my_i2c.h"

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
inline void initI2C(uint32_t base) {
  I2CMasterInitExpClk(base, SysCtlClockGet(),false);
  while(I2CMasterBusBusy(base));
}

//*****************************************************************************
// write 8 bit data on I2C
//*****************************************************************************
inline void writeByteI2C(uint32_t base,uint8_t slave_address,uint8_t data) {
  // write mode
  I2CMasterSlaveAddrSet(base, slave_address, false);
  while(I2CMasterBusy(base));//I2CMasterBusBusy(base)
  I2CMasterDataPut(base, data);
  I2CMasterControl(base, I2C_MASTER_CMD_SINGLE_SEND);
  while(I2CMasterBusy(base));
}

//*****************************************************************************
// write multiple data on I2C
//*****************************************************************************
inline void writeDataI2C(uint32_t base,uint8_t slave_address,uint8_t *data,uint8_t length) {
  uint8_t i;
  // write mode
  I2CMasterSlaveAddrSet(base, slave_address, false);
  while(I2CMasterBusy(base));//I2CMasterBusBusy(base)
  // Send data
  for(i = 0;i < length;i++) {
    I2CMasterDataPut(base, data[i]);
    if (i == 0) {
      I2CMasterControl(base, I2C_MASTER_CMD_BURST_SEND_START);
    } else if (i == length-1) {
      I2CMasterControl(base, I2C_MASTER_CMD_BURST_SEND_FINISH);
    } else {
      I2CMasterControl(base, I2C_MASTER_CMD_BURST_SEND_CONT);
    }
    while(I2CMasterBusy(base));
  }
}

//*****************************************************************************
// read 8 bit data on I2C
//*****************************************************************************
inline uint8_t readByteI2C(uint32_t base, uint8_t slave_address) {
  uint32_t dataRx;
  //set read mode
  I2CMasterSlaveAddrSet(base, slave_address, true);
  //read the data from the slave
  I2CMasterControl(base, I2C_MASTER_CMD_SINGLE_RECEIVE);
  while(I2CMasterBusy(base));
  dataRx = I2CMasterDataGet(base);
  return (uint8_t)dataRx;
}

//*****************************************************************************
// read multiple data on I2C
//*****************************************************************************
inline uint8_t readDataI2C(uint32_t base, uint8_t slave_address, uint8_t *data, uint8_t length) {
  uint32_t dataRx;
  uint8_t i;
  //set read mode
  I2CMasterSlaveAddrSet(base, slave_address, true);
  //read the data from the slave
  while(I2CMasterBusy(base));
  for (i=0;i<length;i++) {
    if (i == 0) {
      I2CMasterControl(base, I2C_MASTER_CMD_BURST_RECEIVE_START);
    } else if (i == length-1) {
      I2CMasterControl(base, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    } else {
      I2CMasterControl(base, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
    }
    dataRx = I2CMasterDataGet(base);
    data[i] = (uint8_t)dataRx;
    while(I2CMasterBusy(base));
  }
  return i+1;
}


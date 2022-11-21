/*
 * colorSensor_TCS3472.c
 *
 *  Created on: Oct 12, 2020
 *      Author: omzn
 */

#include "colorSensor_TCS3472.h"

//*****************************************************************************
// Read 8 bit COMMAND function.
//*****************************************************************************
uint8_t read8ColorSensor(uint8_t reg_name) {
    writeByteI2C(I2C3_BASE, TCS3472_SLAVE_ADDRESS, reg_name | TYPE_REPEAT);
    return readByteI2C(I2C3_BASE, TCS3472_SLAVE_ADDRESS);
}

//*****************************************************************************
// Read 16 bit COMMAND function.
//*****************************************************************************
uint16_t read16ColorSensor(uint8_t reg_name) {
    uint16_t retval;
    retval = (uint16_t)read8ColorSensor(reg_name);
    retval |= (uint16_t)read8ColorSensor(reg_name+1) << 8;
    return retval;
}

//*****************************************************************************
// Read 16 bit x 4 registers function.
//*****************************************************************************
void read64ColorSensor(uint8_t reg_name, uint16_t *result) {
    int i;
    for (i = 0; i < 4; i++) {
        result[i] = read16ColorSensor(reg_name + i*2);
    }
}

//*****************************************************************************
// Write 8 bit COMMAND function.
//*****************************************************************************
void write8ColorSensor(uint8_t reg_name, uint8_t data) {
    uint8_t tx[2];
    tx[0] = reg_name | TYPE_REPEAT;
    tx[1] = data;
    writeDataI2C(I2C3_BASE, TCS3472_SLAVE_ADDRESS, tx, 2);
}

//*****************************************************************************
// Clear INT function.
//*****************************************************************************
void clearIntColorSensor() {
    writeByteI2C(I2C3_BASE, TCS3472_SLAVE_ADDRESS, INT_CLEAR);
}

//*****************************************************************************
// set interrupt threshold function.
//*****************************************************************************
void setIntThresholdColorSensor(uint16_t low_th, uint16_t high_th) {
    //set persistence filter (over 3)
    write8ColorSensor(PERS_REG, 0x03);
    //set interrupt low threshold
    write8ColorSensor(AILTL_REG, low_th & 0x00FF);
    write8ColorSensor(AILTH_REG, low_th >> 8);
    //set interrupt high threshold
    write8ColorSensor(AIHTL_REG, high_th & 0x00FF);
    write8ColorSensor(AIHTH_REG, high_th >> 8);
    // Enable CS Interrupt
    write8ColorSensor(ENABLE_REG, ENABLE_PON | ENABLE_AIEN | ENABLE_AEN); //AEN=1, PON=1,AIEN=1
}

//*****************************************************************************
// Initialize function.
//*****************************************************************************
void initColorSensor(tcs34725IntegrationTime_t it,tcs34725Gain_t ga) {
    // color sensor initial setting
    write8ColorSensor(ATIME_REG, it);   //RGBC Time = 154ms
    write8ColorSensor(CONFIG_REG, 0x00);  //WLONG=0
    write8ColorSensor(WTIME_REG, 0xFF);   //Wait Time = 2.4ms if WLONG=0
    write8ColorSensor(CONTROL_REG, ga); //16x gain

    write8ColorSensor(ENABLE_REG, ENABLE_PON);  //PON=1
    delay_ms(3);
    write8ColorSensor(ENABLE_REG, ENABLE_PON | ENABLE_AEN); //AEN=1, PON=1

}

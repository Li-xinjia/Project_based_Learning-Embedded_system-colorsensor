/*
 * lcd_SB1602.c
 *
 *  Created on: Oct 10, 2020
 *      Author: omzn
 */

#include "lcd_SB1602.h"

//*****************************************************************************
// initialize
//*****************************************************************************
void initLCD(void) {
    uint8_t lcd_init_data1[] = {SB1602_COMMAND_BURST, 0x38, 0x39, 0x14, 0x78, 0x5E, 0x6C};//setting commands for LCD init
    uint8_t lcd_init_data2[] = {SB1602_COMMAND_BURST, 0x38, 0x0C, 0x01};//setting commands for LCD display
    uint8_t lcd_init_data3[] = {SB1602_COMMAND_BURST, 0x10, 0x02};//setting commands for LCD display
    // LCD setting
    delay_ms(40);//40ms
    writeDataI2C(I2C3_BASE, SB1602_SLAVE_ADDRESS, lcd_init_data1, 7);
    delay_ms(210);//210ms
    writeDataI2C(I2C3_BASE, SB1602_SLAVE_ADDRESS, lcd_init_data2, 3);
    delay_ms(20);//20ms
    writeDataI2C(I2C3_BASE, SB1602_SLAVE_ADDRESS, lcd_init_data3, 2);
    delay_ms(20);//20ms
}

//*****************************************************************************
// command LCD
//*****************************************************************************
inline void commandLCD(uint8_t cmd) {
    static uint8_t command[2];
    command[0] = SB1602_COMMAND_SINGLE;
    command[1] = cmd;
    writeDataI2C(I2C3_BASE, SB1602_SLAVE_ADDRESS, command, 2);
}

//*****************************************************************************
// set address LCD
//*****************************************************************************
inline void setAddressLCD(uint8_t x, uint8_t y) {
    if (x > 63 || y > 1) return;
    uint8_t command[] = {SB1602_COMMAND_SINGLE, 0x80 | (y * 0x40 + x)};
    writeDataI2C(I2C3_BASE, SB1602_SLAVE_ADDRESS, command, 2);
}

//*****************************************************************************
// write text LCD
//*****************************************************************************
inline void writeTextLCD(uint8_t *text, uint8_t length) {
    I2CMasterSlaveAddrSet(I2C3_BASE, SB1602_SLAVE_ADDRESS, false);
    while (I2CMasterBusy(I2C3_BASE));//I2CMasterBusBusy(base)
    I2CMasterDataPut(I2C3_BASE, SB1602_DATA_BURST);
    I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while (I2CMasterBusy(I2C3_BASE));//I2CMasterBusBusy(base)

    for (int i = 0; i < length; i++) {
        I2CMasterDataPut(I2C3_BASE, text[i]);
        if (i == length - 1) {
            I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        } else {
            I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
        }
        while (I2CMasterBusy(I2C3_BASE));
    }
}

inline void clearLCD(void) {
    uint8_t command[] = {SB1602_COMMAND_BURST, 0x01};
    writeDataI2C(I2C3_BASE, SB1602_SLAVE_ADDRESS, command, 2);
}


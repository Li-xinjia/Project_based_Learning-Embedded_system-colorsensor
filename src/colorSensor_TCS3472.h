/*
 * color_sensor_TCS3472.h
 *
 *  Created on: 2015/01/15
 */

#ifndef COLORSENSOR_TCS3472_H_
#define COLORSENSOR_TCS3472_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/i2c.h"
#include "my_util.h"
#include "my_i2c.h"

#define TCS3472_SLAVE_ADDRESS          (0x29)

//*****************************************************************************
// Register
//*****************************************************************************
#define TCS_COMMAND_BIT (0x80)
#define TYPE_REPEAT (0x00)
#define TYPE_AUTO   (0x20)
#define TYPE_SP     (0x60)
#define INT_CLEAR   (TCS_COMMAND_BIT | TYPE_SP |0x6 )

#define ENABLE_AIEN      (0x10)    /* RGBC Interrupt Enable */
#define ENABLE_WEN       (0x08)    /* Wait enable - Writing 1 activates the wait timer */
#define ENABLE_AEN       (0x02)    /* RGBC Enable - Writing 1 actives the ADC, 0 disables it */
#define ENABLE_PON       (0x01)    /* Power on - Writing 1 activates the internal oscillator, 0 disables it */

#define ENABLE_REG  (TCS_COMMAND_BIT | 0x00)
#define ATIME_REG   (TCS_COMMAND_BIT | 0x01)
#define WTIME_REG   (TCS_COMMAND_BIT | 0x03)
#define AILTL_REG   (TCS_COMMAND_BIT | 0x04)
#define AILTH_REG   (TCS_COMMAND_BIT | 0x05)
#define AIHTL_REG   (TCS_COMMAND_BIT | 0x06)
#define AIHTH_REG   (TCS_COMMAND_BIT | 0x07)
#define PERS_REG    (TCS_COMMAND_BIT | 0x0C)
#define CONFIG_REG  (TCS_COMMAND_BIT | 0x0D)
#define CONTROL_REG (TCS_COMMAND_BIT | 0x0F)
#define ID_REG      (TCS_COMMAND_BIT | 0x12)
#define STATUS_REG  (TCS_COMMAND_BIT | 0x13)
#define CDATAL_REG  (TCS_COMMAND_BIT | 0x14)
#define CDATAH_REG  (TCS_COMMAND_BIT | 0x15)
#define RDATAL_REG  (TCS_COMMAND_BIT | 0x16)
#define RDATAH_REG  (TCS_COMMAND_BIT | 0x17)
#define GDATAL_REG  (TCS_COMMAND_BIT | 0x18)
#define GDATAH_REG  (TCS_COMMAND_BIT | 0x19)
#define BDATAL_REG  (TCS_COMMAND_BIT | 0x1A)
#define BDATAH_REG  (TCS_COMMAND_BIT | 0x1B)

typedef enum {
	INTEGRATIONTIME_2_4MS = 0xFF, /**<  2.4ms - 1 cycle    - Max Count: 1024  0x0500*/
	INTEGRATIONTIME_24MS = 0xF6, /**<  24ms  - 10 cycles  - Max Count: 10240 0x2800*/
	INTEGRATIONTIME_50MS = 0xEB, /**<  50ms  - 20 cycles  - Max Count: 20480 0x5000*/
	INTEGRATIONTIME_101MS = 0xD5, /**<  101ms - 42 cycles  - Max Count: 43008 */
	INTEGRATIONTIME_154MS = 0xC0, /**<  154ms - 64 cycles  - Max Count: 65535 */
	INTEGRATIONTIME_700MS = 0x00 /**<  700ms - 256 cycles - Max Count: 65535 */
} tcs34725IntegrationTime_t;

typedef enum {
	GAIN_1X = 0x00, /**<  No gain  */
	GAIN_4X = 0x01, /**<  4x gain  */
	GAIN_16X = 0x02, /**<  16x gain */
	GAIN_60X = 0x03 /**<  60x gain */
} tcs34725Gain_t;

//*****************************************************************************
// Read 8 bit COMMAND function.
//*****************************************************************************
uint8_t read8ColorSensor(uint8_t reg_name);
//*****************************************************************************
// Read 16 bit COMMAND function.
//*****************************************************************************
uint16_t read16ColorSensor(uint8_t reg_name);
//*****************************************************************************
// Read 16 bit x 4 registers function.
//*****************************************************************************
void read64ColorSensor(uint8_t reg_name, uint16_t *result);
//*****************************************************************************
// Write 8 bit COMMAND function.
//*****************************************************************************
void write8ColorSensor(uint8_t reg_name, uint8_t data);
//*****************************************************************************
// Clear INT function.
//*****************************************************************************
void clearIntColorSensor();
//*****************************************************************************
// set interrupt threshold function.
//*****************************************************************************
void setIntThresholdColorSensor(uint16_t low_th, uint16_t high_th);
//*****************************************************************************
// Initialize function.
//*****************************************************************************
void initColorSensor(tcs34725IntegrationTime_t it,tcs34725Gain_t ga);
#endif /* SENSOR_TCS3472_H_ */

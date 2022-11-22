/*
 * my_util.h
 *    by @omzn 2020/10/20
 */

#ifndef MY_UTIL_H_
#define MY_UTIL_H_

#include <stdbool.h>
#include <stdint.h>

#include "driverlib/sysctl.h"

#define DIGITS 16
#define SYSTICKS_PER_SEC 32

#define LED_RED    GPIO_PIN_1
#define LED_BLUE   GPIO_PIN_2
#define LED_GREEN  GPIO_PIN_3
#define LED_CYAN   GPIO_PIN_1|GPIO_PIN_2
#define LED_YELLOW GPIO_PIN_1|GPIO_PIN_3
#define LED_WHITE  GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
#define LED_ALL    GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3

#define INT_LEFT_BUTTON  GPIO_INT_PIN_4
#define INT_RIGHT_BUTTON GPIO_INT_PIN_0
#define INT_ALL_BUTTONS  (INT_LEFT_BUTTON | INT_RIGHT_BUTTON)

#define enableSW1PinInt()     GPIOIntEnable(GPIO_PORTF_BASE,GPIO_INT_PIN_4)
#define disableSW1PinInt()    GPIOIntDisable(GPIO_PORTF_BASE,GPIO_INT_PIN_4)
#define clearSW1PinInt()      GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_4)

#define enableCSPinInt()  GPIOIntEnable(GPIO_PORTB_BASE, GPIO_INT_PIN_0)
#define disableCSPinInt() GPIOIntDisable(GPIO_PORTB_BASE, GPIO_INT_PIN_0)
#define clearCSPinInt()   GPIOIntClear(GPIO_PORTB_BASE,GPIO_INT_PIN_0)

#define enableREPinInt()  GPIOIntEnable(GPIO_PORTD_BASE,GPIO_INT_PIN_6)
#define disableREPinInt() GPIOIntDisable(GPIO_PORTD_BASE,GPIO_INT_PIN_6)
#define clearREPinInt()   GPIOIntClear(GPIO_PORTD_BASE,GPIO_PIN_6)

#define NONE 0;

#define RED 1
#define RED_VALUE_R //TODO: set value;
#define RED_VALUE_G //TODO: set value;
#define RED_VALUE_B //TODO: set value;
#define RED_RANG //TODO: set value;

#define GREEN 2
#define GREEN_VALUE_R //TODO: set value;
#define GREEN_VALUE_G //TODO: set value;
#define GREEN_VALUE_B //TODO: set value;
#define GREEN_RANG //TODO: set value;

#define BLUE 3
#define BLUE_VALUE_R //TODO: set value;
#define BLUE_VALUE_G //TODO: set value;
#define BLUE_VALUE_B //TODO: set value;
#define BLUR_VALUE //TODO: set value;
#define BLUE_RANG //TODO: set value;

#define YELLOW 4
#define YELLOW_VALUE_R //TODO: set value;
#define YELLOW_VALUE_G //TODO: set value;
#define YELLOW_VALUE_B //TODO: set value;
#define YELLOW_VALUE //TODO: set value;
#define YELLOW_RANG //TODO: set value;

//*****************************************************************************
// Interrupt handlers
//*****************************************************************************

void SystickIntHandler(void);
void SW1PinIntHandler(void);
void CSPinIntHandler(void);
void REPinIntHandler(void);

//*****************************************************************************
// Utility functions
//*****************************************************************************

extern inline void delay_ms(uint32_t ms);
uint8_t *itoh(uint32_t num,uint8_t length);
uint8_t *itoa(int32_t num,uint8_t length);

#endif /* MY_UTIL_H_ */

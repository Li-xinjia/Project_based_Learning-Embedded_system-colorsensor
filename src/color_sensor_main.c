/*
  Color sensor
    @omzn  2020/10/20
*/

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "inc/hw_i2c.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_qei.h"
#include "inc/hw_timer.h"
#include "inc/hw_types.h"
#include "driverlib/qei.h"
#include "utils/uartstdio.c" //strong-arm method
#include "utils/uartstdio.h"

#include "periphConf.h" //generated by PinMux

#include "my_i2c.h"
#include "my_util.h"

#include "buzzer.h"
#include "colorSensor_TCS3472.h"
#include "lcd_SB1602.h"

//*****************************************************************************
//
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//
//*****************************************************************************
void initConsole(void) {
    // Enable GPIO port A which is used for UART0 pins.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    // Enable UART0 so that we can configure the clock.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    // Select the alternate (UART) function for these pins.
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // Initialize the UART for console I/O.
    UARTStdioConfig(0, 9600, 16000000);
}

void PB1PinIntHandler(void) {
    GPIOIntDisable(GPIO_PORTB_BASE, GPIO_PIN_0);
    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0);

    static uint16_t data_red;
    static uint16_t data_blue;
    static uint16_t data_green;
    static uint16_t data_clear;

    setAddressLCD(0, 0);
    writeTextLCD("R:", 2);
    data_red = read16ColorSensor(RDATAL_REG);
    setAddressLCD(2, 0);
    writeTextLCD(itoh(data_red, 4), 4);

    setAddressLCD(7, 0);
    writeTextLCD("B:", 2);
    data_blue = read16ColorSensor(BDATAL_REG);
    setAddressLCD(9, 0);
    writeTextLCD(itoh(data_blue, 4), 4);

    setAddressLCD(0, 1);
    writeTextLCD("G:", 2);
    data_green = read16ColorSensor(GDATAL_REG);
    setAddressLCD(2, 1);
    writeTextLCD(itoh(data_green, 4), 4);

    setAddressLCD(7, 1);
    writeTextLCD("C:", 2);
    data_clear = read16ColorSensor(CDATAL_REG);
    setAddressLCD(9, 1);
    writeTextLCD(itoh(data_clear, 4), 4);

//    UARTprintf("PB1 interrupt\n");
    clearIntColorSensor();

    delay_ms(100);
    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0);
}

void initInterruptPins(void) {
    // You can write your own code here.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_0,
                     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0);

    GPIOIntRegister(GPIO_PORTB_BASE, PB1PinIntHandler);
    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_0,
                   GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0);

    UARTprintf("Interrupt pins initiate over\n");
}

//*****************************************************************************
// Event handers
//*****************************************************************************

void SysTickIntHandler(void) {
    static uint32_t led_color = LED_ALL;
    static uint32_t tick_count = 0;
    if (tick_count % 16 == 0) {
        led_color = ~led_color;
        GPIOPinWrite(GPIO_PORTF_BASE, LED_CYAN, led_color);
        tick_count = 0;
    }
    tick_count++;
}


int main(void) {
    // Set the clocking to run directly from the crystal.
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
    // Set up ports hardware (see periphConf.c)
    PortFunctionInit();

    // Initialize console
    initConsole();
    UARTprintf("Color sensor example\n");

    // Set up interrupts (you can specify GPIO interrupt initialization here)
    initInterruptPins();

    // Initialize buzzer
    initBuzzer();

    // Initialize two I2C Masters
    initI2C(I2C3_BASE);
    // Initialize LCD module
    initLCD();
    // Initialize color sensor module
    initColorSensor(INTEGRATIONTIME_154MS, GAIN_16X);
    clearIntColorSensor();

    SysTickPeriodSet(SysCtlClockGet() / SYSTICKS_PER_SEC);
    SysTickEnable();
    SysTickIntRegister(SysTickIntHandler);
    SysTickIntEnable();

//    setAddressLCD(0, 0);
//    writeTextLCD("CLEAR DATA:", 11);


    while (1) {
        // Warning: you do not leave the contents here.
//        data_clear = read16ColorSensor(CDATAL_REG);
//        setAddressLCD(11, 0);
//        writeTextLCD(itoh(data_clear, 4), 4);
//        delay_ms(100);
    }
}

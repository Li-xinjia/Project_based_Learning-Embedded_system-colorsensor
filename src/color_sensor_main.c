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
#include "rotaryEncoder.h"

//*****************************************************************************
//
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//
//*****************************************************************************

struct Statue {
    uint8_t selectColor;
    uint8_t nowColor;
    uint8_t rTimes;
    uint8_t gTimes;
    uint8_t bTimes;
    uint8_t yTimes;
} statue;


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

    setAddressLCD(0, 0);
    switch (statue.selectColor) {
        case RED:
            writeTextLCD("RED   ", 6);
            break;
        case GREEN:
            writeTextLCD("GREEN ", 6);
            break;
        case BLUE:
            writeTextLCD("BLUE  ", 6);
            break;
        case YELLOW:
            writeTextLCD("YELLOW", 6);
            break;
    }

    setAddressLCD(0, 1);
    switch (statue.nowColor) {
        case RED:
            writeTextLCD("RED   ", 6);
            break;
        case GREEN:
            writeTextLCD("GREEN ", 6);
            break;
        case BLUE:
            writeTextLCD("BLUE  ", 6);
            break;
        case YELLOW:
            writeTextLCD("YELLOW", 6);
            break;
    }

    setAddressLCD(10, 0);
    writeTextLCD(itoa(statue.rTimes, 2), 2);

    setAddressLCD(14, 0);
    writeTextLCD(itoa(statue.yTimes, 2), 2);

    setAddressLCD(10, 1);
    writeTextLCD(itoa(statue.gTimes, 2), 2);

    setAddressLCD(14, 1);
    writeTextLCD(itoa(statue.bTimes, 2), 2);

    tick_count++;
}


void PB1PinIntHandler(void) {
    GPIOIntDisable(GPIO_PORTB_BASE, GPIO_PIN_0);
    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0);

    static uint16_t data_red;
    static uint16_t data_blue;
    static uint16_t data_green;
    static uint16_t data_clear;

    data_red = read16ColorSensor(RDATAL_REG);
    data_blue = read16ColorSensor(BDATAL_REG);
    data_green = read16ColorSensor(GDATAL_REG);
    data_clear = read16ColorSensor(CDATAL_REG);

    if (data_red > RED_VALUE_R - RED_RANG && data_red < RED_VALUE_R + RED_RANG) {
        UARTprintf("1\n");
        if (data_blue > RED_VALUE_B - RED_RANG && data_blue < RED_VALUE_B + RED_RANG) {
            UARTprintf("2\n");
            if (data_green > RED_VALUE_G - RED_RANG && data_red < RED_VALUE_G + RED_RANG) {
                UARTprintf("3\n");
                statue.nowColor = RED;
                statue.rTimes += 1;
                delay_ms(1000);
                clearIntColorSensor();
                GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0);
                return;
            }
        }
    }

    if (data_red > BLUE_VALUE_R - BLUE_RANG && data_red < BLUE_VALUE_R + BLUE_RANG) {
        if (data_blue > BLUE_VALUE_B - BLUE_RANG && data_blue < BLUE_VALUE_B + BLUE_RANG) {
            if (data_green > BLUE_VALUE_G - BLUE_RANG && data_red < BLUE_VALUE_G + BLUE_RANG) {
                statue.nowColor = BLUE;
                statue.bTimes += 1;
                delay_ms(1000);
                clearIntColorSensor();
                GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0);
                return;
            }
        }
    }

    if (data_red > GREEN_VALUE_R - GREEN_RANG && data_red < GREEN_VALUE_R + GREEN_RANG) {
        if (data_blue > GREEN_VALUE_B - GREEN_RANG && data_blue < GREEN_VALUE_B + GREEN_RANG) {
            if (data_green > GREEN_VALUE_G - GREEN_RANG && data_red < GREEN_VALUE_G + GREEN_RANG) {
                statue.nowColor = GREEN;
                statue.gTimes += 1;
                delay_ms(1000);
                clearIntColorSensor();
                GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0);
                return;
            }
        }
    }

    if (data_red > YELLOW_VALUE_R - YELLOW_RANG && data_red < YELLOW_VALUE_R + YELLOW_RANG) {
        if (data_blue > YELLOW_VALUE_B - YELLOW_RANG && data_blue < YELLOW_VALUE_B + YELLOW_RANG) {
            if (data_green > YELLOW_VALUE_G - YELLOW_RANG && data_red < YELLOW_VALUE_G + YELLOW_RANG) {
                statue.nowColor = YELLOW;
                statue.yTimes += 1;
                delay_ms(1000);
                clearIntColorSensor();
                GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0);
                return;
            }
        }
    }

    UARTprintf("%04x %04x %04x\n", data_red, data_blue, data_green);

    statue.nowColor = NONE;
    clearIntColorSensor();

    delay_ms(100);
    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0);
}

void SW1PinIntHandler(void) {
    GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);

    UARTprintf("SW1 pushed\n");

    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);
}

void REPinIntHandler(void) {
    // Start Interrupt
    static uint32_t position;

    GPIOIntDisable(GPIO_PORTD_BASE, GPIO_PIN_6);
    GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_6);

    position = QEIPositionGet(QEI0_BASE);

    UARTprintf("Color:%d\n", statue.selectColor);
    UARTprintf("%d %d\n", QEIPositionGet(QEI0_BASE));

    if (position >= 3 && QEIDirectionGet(QEI0_BASE) == 1) {
        QEIPositionSet(QEI0_BASE, 0);
        switch (statue.selectColor) {
            case RED:
                statue.selectColor = YELLOW;
                break;
            case YELLOW:
                statue.selectColor = BLUE;
                break;
            case BLUE:
                statue.selectColor = GREEN;
                break;
            case GREEN:
                statue.selectColor = RED;
                break;
        }
    } else if (position <= 94 && QEIDirectionGet(QEI0_BASE) == -1) {
        QEIPositionSet(QEI0_BASE, 0);
        switch (statue.selectColor) {
            case RED:
                statue.selectColor = GREEN;
                break;
            case GREEN:
                statue.selectColor = BLUE;
                break;
            case BLUE:
                statue.selectColor = YELLOW;
                break;
            case YELLOW:
                statue.selectColor = RED;
                break;
        }
    }
    GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_6);
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

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    // Set SW1 as an interrupt
    GPIOIntRegister(GPIO_PORTF_BASE, SW1PinIntHandler);
    // Make pins 4 falling-edge triggered interrupts.
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_INT_PIN_4, GPIO_FALLING_EDGE);
    // PF4 pull-up
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);


    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    // Watch QEI as an interrupt
    GPIOIntRegister(GPIO_PORTD_BASE, REPinIntHandler);
    // Make pins PD6 falling-edge triggered interrupts.
    GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_INT_PIN_6, GPIO_BOTH_EDGES);
    // (PD6 is physically pulled-up)

    GPIOIntEnable(GPIO_PORTD_BASE, GPIO_INT_PIN_6);

    UARTprintf("Interrupt pins initiate over\n");
}


int main(void) {
    // Set the clocking to run directly from the crystal.
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    // Initialize statue
    statue.nowColor = NONE;
    statue.selectColor = RED;
    statue.rTimes = 0;
    statue.gTimes = 0;
    statue.bTimes = 0;
    statue.yTimes = 0;

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
    clearLCD();

    // Initialize color sensor module
    initColorSensor(INTEGRATIONTIME_154MS, GAIN_16X);
    clearIntColorSensor();

    initRotaryEncoder();

    SysTickPeriodSet(SysCtlClockGet() / SYSTICKS_PER_SEC);
    SysTickEnable();
    SysTickIntRegister(SysTickIntHandler);
    SysTickIntEnable();


    while (1) {
    }
}

/*
 * buzzer.c
 *
 *  Created on: Oct 10, 2020
 *      Author: omzn
 */

#include "buzzer.h"

void initBuzzer(void) {
  SysCtlPWMClockSet(SYSCTL_PWMDIV_16);//1MHz

  // M0PWM3 on PB5 for Buzzer
  // Configure the PWM0 to count up/down without synchronization.
  PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN |
                  PWM_GEN_MODE_NO_SYNC);
  // Set PWM period (maximum period:2^16)
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, O4C); //20ms*PWMClock=20000
  // Set the PWM pulse width.
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, O4C >> 1);
  // PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7,1500);
  // Enable the PWM0 Bit6 (PC4) output signal.
  PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, false);
  // Enable the PWM0 Bit7 (PC5) output signal.
  // PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
  // Enable the PWM generator block.
  PWMGenEnable(PWM0_BASE, PWM_GEN_1);
}

void toneBuzzer(int tone) {

}

void restBuzzer() {

}



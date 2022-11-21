/*
 * rotaryEncoder.c
 *
 *  Created on: Oct 10, 2020
 *      Author: omzn
 */

#include "rotaryEncoder.h"

//*****************************************************************************
// initialize
//*****************************************************************************
void initRotaryEncoder(void) {
  // Setting up QEI for a rotary encoder
  QEIConfigure(QEI0_BASE,
           QEI_CONFIG_CAPTURE_A_B|
           QEI_CONFIG_RESET_IDX|
           QEI_CONFIG_QUADRATURE|
           QEI_CONFIG_NO_SWAP,
           24*4-1);

  // For debouncing
  HWREG(QEI0_BASE + QEI_O_CTL) |= QEI_CTL_FILTCNT_M + QEI_CTL_FILTEN;
  // Enable the quadrature encoder.
  QEIEnable(QEI0_BASE);
  QEIPositionSet(QEI0_BASE,0);
}

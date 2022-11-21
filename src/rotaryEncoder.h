/*
 * rotaryEncoder.h
 *
 *  Created on: 2014/01/15 by omzn
 */

#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_qei.h"
#include "driverlib/qei.h"

//*****************************************************************************
// utility functions
//*****************************************************************************
#define setPositionRotaryEncoder(pos) QEIPositionSet(QEI0_BASE,pos)
#define getPositionRotaryEncoder() QEIPositionGet(QEI0_BASE)
#define getDirectionRotaryEncoder() QEIDirectionGet(QEI0_BASE)

void initRotaryEncoder(void);

#endif /* ROTARYENCODER_H_ */

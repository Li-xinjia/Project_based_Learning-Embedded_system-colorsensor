/*
 *  buzzer.h
 *  Created on: 2020/10/10 by omzn
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/timer.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

// ドがC ド#はCP O4はオクターブ
#define O4C  3820
#define O4CP 3610
#define O4D  3410
#define O4DP 3210
#define O4E  3030
#define O4F  2860
#define O4FP 2700
#define O4G  2550
#define O4GP 2410
#define O4A  2270
#define O4AP 2150
#define O4B  2020
#define O5C  (O4C  >> 1)
#define O5CP (O4CP >> 1)
#define O5D  (O4D  >> 1)
#define O5DP (O4DP >> 1)
#define O5E  (O4E  >> 1)
#define O5F  (O4F  >> 1)
#define O5FP (O4FP >> 1)
#define O5G  (O4G  >> 1)
#define O5GP (O4GP >> 1)
#define O5A  (O4A  >> 1)
#define O5AP (O4AP >> 1)
#define O5B  (O4B  >> 1)
#define O6C  (O4C  >> 2)
#define O6CP (O4CP >> 2)
#define O6D  (O4D  >> 2)
#define O6DP (O4DP >> 2)
#define O6E  (O4E  >> 2)
#define O6F  (O4F  >> 2)
#define O6FP (O4FP >> 2)
#define O6G  (O4G  >> 2)
#define O6GP (O4GP >> 2)
#define O6A  (O4A  >> 2)
#define O6AP (O4AP >> 2)
#define O6B  (O4B  >> 2)
#define O7C  (O4C  >> 3)
#define RR 0

#define L1  1
#define L2  2
#define L4  4
#define L8  8
#define L12 12
#define L16 16
#define L24 24
#define L48 48

void toneBuzzer(int tone);
void restBuzzer();
void initBuzzer();

#endif /* BUZZER_H_ */

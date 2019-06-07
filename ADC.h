/* 
 * File:   A_to_D_Converter.h
 * Author: Michael
 *
 * Created on 29 février 2016, 10:33
 */

#ifndef A_TO_D_CONVERTER_H
#define	A_TO_D_CONVERTER_H

//Pre-Processor Directives:
#include <xc.h>
#include "system.h"

//Functions and Variables with Global Scope:
extern int Potentiometer;

void ADC_Init(void);
void __attribute__((__interrupt__)) _ADCInterrupt(void);

#endif	/* A_TO_D_CONVERTER_H */


/* 
 * File:   DisplayRoutines.h
 * Author: Michael
 *
 * Created on 29 février 2016, 10:30
 */

#ifndef DISPLAYROUTINES_H
#define	DISPLAYROUTINES_H

//Pre-Processor Directives:
#include <xc.h>
#include "system.h"

//Declaration to Link External Functions & Variables:
extern  int     TempSensor;
extern  int     Potentiometer;

//Functions and Variables with Global Scope:
void UpdateDisplayBuffer(void);
void ADCResult2Decimal(unsigned int ADRES);

#endif	/* DISPLAYROUTINES_H */


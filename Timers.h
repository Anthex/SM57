/* 
 * File:   Timers.h
 * Author: Michael
 *
 * Created on 29 février 2016, 10:19
 */

#ifndef TIMERS_H
#define	TIMERS_H

//Pre-Processor Directives:
#include <xc.h>
#include "System.h"

//Functions and Variables with Global Scope:
void Timer1_Init(void);
void Timer2_Init(void);
void __attribute__((__interrupt__)) _T1Interrupt(void);

#endif	/* TIMERS_H */


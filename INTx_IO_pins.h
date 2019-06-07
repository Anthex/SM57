/* 
 * File:   INTx_IO_pins.h
 * Author: Michael
 *
 * Created on 29 février 2016, 10:18
 */

#ifndef INTX_IO_PINS_H
#define	INTX_IO_PINS_H

//Pre-Processor Directives:
#include <xc.h>
#include "system.h"


//Functions and Variables with Global Scope:
void INTx_IO_Init(void);
void __attribute__((__interrupt__)) _INT0Interrupt(void);
void __attribute__((__interrupt__)) _INT1Interrupt(void);

#endif	/* INTX_IO_PINS_H */


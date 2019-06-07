/* 
 * File:   UART.h
 * Author: Michael
 *
 * Created on 29 février 2016, 13:42
 */

#ifndef UART_H
#define	UART_H

//Pre-Processor Directives:
#include <xc.h>
#include "system.h"

#define BAUDRATE        9600                    //Desired Baud Rate
#define BRGVAL          ((FCY/BAUDRATE)/16)-1   //Formula for U1BRG register
                                                //from dsPIC30F Family
                                                //Reference Manual

//Declaration to Link External Functions & Variables:
extern  unsigned char DisplayData[];

//Functions and Variables with Global Scope:
void UART_Init (void);
void putch(char data);
void WriteUART_to_RS232(void);
void __attribute__((__interrupt__)) _U1TXInterrupt(void);


#endif	/* UART_H */


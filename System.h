

#ifndef SYSTEM_H
#define	SYSTEM_H

#include <xc.h>

//Defines for System Clock Timing -
//For oscillator configuration XT x PLL8 mode,
//Device Throughput in MIPS = Fcy = 7372800*8/4 = ~14.74 MIPS
//Instruction Cycle time = Tcy = 1/(Fcy) = ~68 nanoseconds
#define XTFREQ          10000000         //On-board Crystal frequency
#define PLLMODE         8               //On-chip PLL setting
#define FCY             XTFREQ*PLLMODE/4        //Instruction Cycle Frequency

//Defines that equate Switches on board to specific interrupt pins on device
#define Switch_S5       PORTEbits.RE8   //Switch S5 is connected to RE8
#define Switch_S6       PORTDbits.RD0   //Switch S6 is connected to RD0

#define LED_PER 500


#endif	/* SYSTEM_H */




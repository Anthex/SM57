#include "Timers.h"

//Timer1_Init() sets up Timer1 to count up to the maximum 16-bit value, 0xFFFF,
//and interrupt the CPU. ISR processing is enabled for Timer 1.
void Timer1_Init(void)
{
        T1CON = 0x0000;         //Timer1 set up to count on instruction cycle
                                //edge with 1:1 prescaler applied initially.
        PR1 = 19999;		//Period Register, PR1, set 19999 -> 1ms period
        IFS0bits.T1IF = 0;      //Clear the Timer1 Interrupt Flag
        IEC0bits.T1IE = 1;      //Enable Timer1 Interrupt Service Routine
        T1CONbits.TON=1;        //Start Timer 1
}





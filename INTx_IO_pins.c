#include "INTx_IO_pins.h"

//Functions:
//INTx_IO_Init() sets up the INT0 and INT1 pins connected to the
//switches S5 and S6, on the dsPICDEM2 board. It also sets port
//pins, RB0 and RB1, as output pins to drive LEDs, D3 and D4.
void INTx_IO_Init(void)
{
        //Set up Switches S5 and S6 as external interrupt input pins
        //S5 connected to INT0
        //S6 connected to INT1
        INTCON2 = 0x0003;       //Setup INT0-1 pins to interupt on falling edge
        IFS0bits.INT0IF = 0;    //Reset INT0 interrupt flag
        IEC0bits.INT0IE = 1;    //Enable INT0 Interrupt Service Routine
        IFS1bits.INT1IF = 0;    //Reset INT1 interrupt flag
        IEC1bits.INT1IE = 1;    //Enable INT1 Interrupt Service Routine

        //Set up port pins RB0 and RB1 to drive the LEDs, D3 and D4
        //RB0 connected to D3
        //RB1 connected to D4
        LATBbits.LATB0 = 0;     //Clear Latch bit for RB0 port pin
        TRISBbits.TRISB0 = 0;   //Set the RB0 pin direction to be an output
        LATBbits.LATB1 = 0;     //Clear Latch bit for RB1 port pin
        TRISBbits.TRISB1 = 0;   //Set the RB1 pin direction to be an output
}

//_INT0Interrupt() is the INT0 interrupt service routine (ISR).
void _ISR __attribute__((no_auto_psv)) _INT0Interrupt(void)
{
    // Do what you want here ... (send CAN frame, toggle LED ...)
    IFS0bits.INT0IF = 0;    //Clear the INT0 interrupt flag or else
                            //the CPU will keep vectoring back to the ISR
}

//_INT1Interrupt() is the INT1 interrupt service routine (ISR).
void _ISR __attribute__((no_auto_psv)) _INT1Interrupt(void)
{
        // Do what you want here ...
        IFS1bits.INT1IF = 0;    //Clear the INT0 interrupt flag or else
                                //the CPU will keep vectoring back to the ISR
}


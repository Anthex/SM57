#include "UART.h"

//UART_Init() sets up the UART for a 8-bit data, No Parity, 1 Stop bit
//at 9600 baud with transmitter interrupts enabled
void UART_Init (void)
{
        U1MODE = 0x0000;        //Clear UART2 registers
        U1STA = 0x0000;
        U1MODEbits.ALTIO = 1;   //Enable U1ATX and U1ARX instead of
                                //U1TX and U1RX pins

        U1MODEbits.UARTEN = 1;  //Enable UART1 module
        U1BRG = BRGVAL;         //Load UART1 Baud Rate Generator

        IFS0bits.U1RXIF = 0;    //Clear UART1 Receiver Interrupt Flag
        IFS0bits.U1TXIF = 0;    //Clear UART1 Transmitter Interrupt Flag
        IEC0bits.U1RXIE = 0;    //Disable UART1 Receiver ISR
        IEC0bits.U1TXIE = 0;    //Enable UART1 Transmitter ISR
        U1STAbits.UTXISEL = 1;  //Setup UART1 transmitter to interrupt
                                //when a character is transferred to the
                                //Transmit Shift register and as result,
                                //the transmit buffer becomes empty.

        U1STAbits.UTXEN = 1;    //Enable UART1 transmitter
}

// This function is used to send one character from UART to PC
// This function is used by printf() function to send formatted string
void putch(char data)
{
    while(!IFS0bits.U1TXIF);
    U1TXREG = data;
}






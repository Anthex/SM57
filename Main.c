/*
; © 2005  Microchip Technology Inc.
;
; Microchip Technology Inc. (“Microchip”) licenses this software to you
; solely for use with Microchip dsPIC® digital signal controller
; products.  The Make and Programmsoftware is owned by Microchip and is protected under
; applicable copyright laws.  All rights reserved.
;
; SOFTWARE IS PROVIDED “AS IS.”  MICROCHIP EXPRESSLY DISCLAIMS ANY
; WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
; LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
; PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
; BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
; DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
; PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
; BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
; ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
*/


#include <xc.h>
#include <p30f4011.h>

#include "INTx_IO_pins.h"
#include "Timers.h"
#include "DisplayRoutines.h"
//#include "SPI_for_LCD.h"
#include "ADC.h"
#include "UART.h"
#include "can.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"


// FOSC
#pragma config FPR = XT_PLL8            // Primary Oscillator Mode (XT w/PLL 8x)
#pragma config FOS = PRI                // Oscillator Source (Internal Fast RC)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_OFF         // POR Timer Value (Timer Disabled)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config LPOL = PWMxL_ACT_HI      // Low-side PWM Output Polarity (Active High)
#pragma config HPOL = PWMxH_ACT_HI      // High-side PWM Output Polarity (Active High)
#pragma config PWMPIN = RST_IOPIN       // PWM Output Pin Reset (Control with PORT/TRIS regs)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

#define FEUAVDROIT  0x10522111
#define COMMODOCLIGNO 0x10005110    
#define FEUARRDROIT 0x10532110
#define FEUARRGAUCHE 0x10532112
#define TRIPCOMODO 0x10005100


uint16_t ledTick;
char cligno =0;
char clignoState =0;

CANMSG messageReception;



int main (void)
{
    ADPCFG = 0xFFFF;        //After reset all port pins multiplexed
			    //with the A/D converter are configred analog.
			    //We will reconfigure them to be digital
			    //by writing all 1's to the ADPCFG register.
			    //Note: All dsPIC registers are memory mapped.
			    //The address of ADPCFG and other registers
			    //are defined in the device linker script
			    //in your project.


    //Function UART_Init() available in file, UART.c
    UART_Init();            //Initialize the UART module to communicate
			    //with the COM port on the Host PC via an
			    //RS232 cable and the DB9 connector.

    //Function ADC_Init() available in file, A_to_D_Converter.c
    ADC_Init();             //Initialize the A/D converter to convert
			    //signals from the Temperature Sensor and the
			    //Potentiometer.

    //Function INTx_IO_Init() available in file, INTx_IO_pins.c
    INTx_IO_Init();         //Initialize the External interrupt pins and
			    //some I/O pins to accept input from the
			    //switches, S5 and S6 and drive the LEDs, D3
			    //and D4.

    //Function Timer1_Init() & Timer2_Init() available in file, Timers.c
    Timer1_Init();          //Initialize Timer1 to provide 1ms time base


    /*** Exemple de formatage de chaîne de caractère pour affichage sur terminal PC ***/
    /*uint8_t var = 2;
    //printf("\n\rValeur de var : %d", var);*/
    /**************************************/

    ledTick = LED_PER;
  
    
    initCAN();
    
    messageReception.ID = 0;
    messageReception.len = 0x08;
    messageReception.data[0] = 0;
    messageReception.data[1] = 0;
    messageReception.data[2] = 0;
    messageReception.data[3] = 0;
    messageReception.data[4] = 0;
    messageReception.data[5] = 0;
    messageReception.data[6] = 0;
    messageReception.data[7] = 0;

    
    while (1)               //Main Loop of Code Executes forever
    {
        if(!ledTick)
        {   
            
            //sendCANFrame(&messageReception);
            ledTick = LED_PER;
            if (cligno || clignoState){
                clignoState =~ clignoState;
                clignotantArrGauche(clignoState);
                clignotantArrDroit(clignoState);
            }
        }
    }
    return 0;
}

//_T1Interrupt() is the Timer1 Interrupt Service Routine
void _ISR __attribute__((no_auto_psv)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0;                      // Clear Timer1 Interrupt Flag
    if(ledTick)ledTick--;			// Decrement LED tick counter	
}

void  _ISR __attribute__((interrupt, no_auto_psv)) _C1Interrupt(void)
{
  LATBbits.LATB1 = 1;
  //printf("Interruption declenchee\r\n"); 
  IFS1bits.C1IF = 0; //Clear interrupt flag
    if(C1INTFbits.RX0IF) 
      { 
        //printf("Message sur RX0\r\n");  
        C1INTFbits.RX0IF = 0; //clear interrupt
		
		// RECEPTION MESSAGE ICI
		getCANFrame(&messageReception, 0);
		//sendCANFrame(&messageReception);
        treatMsg(&messageReception);
        
      }
    if(C1INTFbits.RX1IF) 
      { 
        //printf("Message sur RX1\r\n"); 
        C1INTFbits.RX1IF = 0; //clear interrupt
		
		// RECEPTION MESSAGE ICI
        
		getCANFrame(&messageReception, 1);
		//sendCANFrame(&messageReception);
        treatMsg(&messageReception);
      }
    
    
    //printf("C1INTF : , %d \r\n", C1INTFbits.TXBO);
    //C1INTF = 0;
}

void treatMsg(CANMSG *messageReception){    
    switch(messageReception->ID){
        case COMMODOCLIGNO:
            switch(messageReception->data[0]){
                case 0xc:
                    cligno = 1;
                    clignotantArrDroit(1);
                    clignotantArrGauche(1);
                    break;
                default:
                    cligno = 0;
                    clignotantArrGauche(0);
                    clignotantArrDroit(0);
                    break;
            }
            break;
        case TRIPCOMODO:
            switch(messageReception->data[0]){
                case 0xBA:
                    feuStopArrDroit(0);
                    break;
                default:
                    feuStopArrDroit(1);
                    break;
            }
            break;
        default:
            break;
    }
}

void clignotantArrGauche(char state){
    while(C1TX0CONbits.TXREQ); //on attend que le buffer soit vide
    CANMSG MsgToSend;
    MsgToSend.ID = FEUARRGAUCHE;
    MsgToSend.len = 1;
    MsgToSend.data[0] = 0x04 * state;
    LATBbits.LATB1 = state;
    sendCANFrame(&MsgToSend);
}

void clignotantArrDroit(char state){
    while(C1TX0CONbits.TXREQ);
    CANMSG MsgToSend;
    MsgToSend.ID = FEUARRDROIT;
    MsgToSend.len = 1;
    MsgToSend.data[0] = state;
    LATBbits.LATB1 = state;
    sendCANFrame(&MsgToSend);
}


void feuStopArrDroit(char state){
    CANMSG MsgToSend;
    MsgToSend.ID = FEUARRGAUCHE;
    MsgToSend.len = 1;
    MsgToSend.data[0] = state; //0x01
    LATBbits.LATB1 = state;
    sendCANFrame(&MsgToSend);
}
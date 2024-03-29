#include "ADC.h"

int TempSensor = 0;
int Potentiometer = 0;

//Functions:

//ADC_Init() is used to configure A/D to scan and convert 2 input channels
//per interrupt. The A/D is set up for a total sampling rate of 8KHz
//or 4KHz per channel. The internal counter in the A/D is used to provide
//Acquisition time delay. The input pins being scanned are AN2 and AN3.
//AN2 and AN3 are connected to the Temperature Sensor and the Potentiometer
//on the dsPICDEM2 board.
void ADC_Init(void)
{
        //ADCON1 Register
        //Set up A/D for Automatic Sampling, Auto-Convert
        //All other bits to their default state
        ADCON1bits.SSRC = 7;
        ADCON1bits.ASAM = 1;

        //ADCON2 Register
        //Set up A/D for interrupting after 2 samples get filled in the buffer
        //Also, enable Channel scanning
        //All other bits to their default state
        ADCON2bits.SMPI = 1;
        ADCON2bits.CSCNA = 1;

        //ADCON3 Register
        //Set up Acquisition time (Tacq) for 31 Tad periods
        //where, Tad = A/D conversion clock time.
        //Set up Tad period to be 20.5 Tcy (Tcy = instruction cycle time)
        //Given that each conversion takes 14*Tad (=Tconv) periods,
        //Total Sample Time = Acquisition Time + Conversion Time
        // = (31 + 14)*Tad = 45*Tad periods
        // = 45 * 20.5 * Tcy = 922.5*Tcy periods
        //At 7.3728 MIPS, Tcy = 135 ns = Instruction Cycle Time
        //So Tsamp = Tacq + Tconv = 45*Tad(in this example)= 125.1 microseconds
        //So Fsamp = Sampling Rate ~= 8 KHz
        //All other bits to their default state
        ADCON3bits.SAMC = 31;
        ADCON3bits.ADCS = 40;

        //ADCHS Register
        //When Channel scanning is enabled (ADCON2bits.CSCNA=1)
        //AND Alternate mux sampling is disabled (ADCON2bits.ALTS=0)
        //then ADCHS is a "don't care"
        ADCHS = 0x0000;

        //ADCSSL Register
        //Scan channels AN2, AN3 fas part of scanning sequence
        ADCSSL = 0x000C;

        //ADPCFG Register
        //Set up channels AN2, AN3 as analog inputs and leave rest as digital
        //Recall that we configured all A/D pins as digital when code execution
        //entered main() out of reset
        ADPCFGbits.PCFG2 = 0;
        ADPCFGbits.PCFG3 = 0;

        //Clear the A/D interrupt flag bit
        IFS0bits.ADIF = 0;

        //Set the A/D interrupt enable bit
        IEC0bits.ADIE = 1;

        //Turn on the A/D converter
        //This is typically done after configuring other registers
        ADCON1bits.ADON = 1;

}

//_ADCInterrupt() is the A/D interrupt service routine (ISR).
//The routine must have global scope in order to be an ISR.
//The ISR name is chosen from the device linker script.
void _ISR __attribute__((no_auto_psv)) _ADCInterrupt(void)
{
        //Copy the A/D conversion results from ADCBUFn to variables-
        //"Potentiometer" and "TempSensor".
        //Since ADCON2bits.SMPI = 1, only the first two (i.e. SMPI+1) ADCBUFn
        //locations are used by the module to store conversion results
        Potentiometer = ADCBUF0;
        TempSensor = ADCBUF1;

        //Clear the A/D Interrupt flag bit or else the CPU will
        //keep vectoring back to the ISR
        IFS0bits.ADIF = 0;

}



#include <p30f4011.h>

#include "can.h"
#include "math.h"

/****** Définitions des fonctions CAN *****/


void initCAN(void)
{   
    //mise en mode config
    C1CTRLbits.REQOP = 4;
    while(C1CTRLbits.REQOP != 4);
    
    //configuration pins
    
    TRISFbits.TRISF0 = 1;      //C1RX TRISF pin 0 mise en input
    TRISFbits.TRISF1 = 0;      //C1TX TRISF pin 1 mise en output   
    
    
    //init module
    C1CFG1bits.BRP = 1;       // 10 Meg clock 
    C1CFG1bits.SJW = 2;       // Synchronized jump width time is 2Tq. 
    C1CFG2bits.PRSEG = 6;     // Propagation time segment = 6Tq. 
    C1CFG2bits.SEG1PH = 5;    // Phase buffer Segment 1 = 5Tq. 
    C1CFG2bits.SAM = 0;       // Bus is sampled once. 
    C1CFG2bits.SEG2PHTS = 1;  // PhaseSeg2 is freely programmable. 
    C1CFG2bits.SEG2PH = 5;    // Phase buffer Segment 2 = 5Tq.
    
    C1CTRLbits.ABAT = 1;        // Can clock is Fcy = 10MHz
    C1CTRLbits.CANCKS = 1;

            
    
    //activation des interruptions
    C1INTF = 0;
    C1INTEbits.RX0IE = 1;
    C1INTEbits.RX1IE = 1;
    C1INTEbits.TX0IE = 1;
    C1INTEbits.ERRIE = 1;
    C1INTE = 0x00FF;
    
    //CAN interrupts flags
    C1INTFbits.RX0IF = 0;
    C1INTFbits.RXB1IF = 0;
    C1INTFbits.RX1IF = 0;    
    C1INTFbits.RXB0IF = 0;
    
    IFS1bits.C1IF = 0;
    IEC1bits.C1IE = 1;
    IPC6bits.C1IP = 7;
    
    
    //filtres
    C1RXF0SIDbits.EXIDE = 0;
    C1RXF1SIDbits.EXIDE = 0;
    C1RXF2SIDbits.EXIDE = 0;
    C1RXF3SIDbits.EXIDE = 0;
    C1RXF4SIDbits.EXIDE = 0;
    C1RXF5SIDbits.EXIDE = 0;

    //masques      
    C1RXM0SID = 0x0000;
    C1RXM1SID = 0x0000;
         
    C1RXM0SIDbits.MIDE = 0;
    C1RXM1SIDbits.MIDE = 0;    
                
    //mise en mode normal
    C1CTRLbits.REQOP = 0;
    while(C1CTRLbits.REQOP != 0);
      

}

void sendCANFrame(CANMSG *msg)
{   
    if (C1TX0CONbits.TXREQ == 0)
    {   
        C1TX0SIDbits.TXIDE = 1;
            
        // id poids fort (standard)
        C1TX0SIDbits.SID10_6 = ((msg->ID)>>24) & 0x1F;
        C1TX0SIDbits.SID5_0 = ((msg->ID)>>18) & 0x3F;
        
        //id poids faible (extended)
        C1TX0EIDbits.EID17_14 = ((msg->ID)>>14) & 0x0F;
        C1TX0EIDbits.EID13_6 = ((msg->ID) >>6) & 0xFF;
        C1TX0DLCbits.EID5_0 = ((msg->ID)) & 0x3F;            
        

        //data
        C1TX0B1 = ((msg->data[1])<<8) + (msg->data[0]); 
        C1TX0B2 = ((msg->data[3])<<8) + (msg->data[2]);
        C1TX0B3 = ((msg->data[5])<<8) + (msg->data[4]);
        C1TX0B4 = ((msg->data[7])<<8) + (msg->data[6]);
       
        
        
        //data length
        C1TX0DLCbits.DLC = msg->len;
        C1TX0DLCbits.TXRTR = 0;
        C1TX0DLCbits.TXRB1 = 0;
        C1TX0DLCbits.TXRB0 = 0;
        
        //beginning transmission
        C1TX0CONbits.TXREQ = 1;
    }    
    
    
    
    
        
    
}

int getCANFrame(CANMSG *message, int buffer)
{
    //printf("lecture frame \r\n");
    if (buffer == 0){ 
    if(C1RX0CONbits.RXFUL)
    {   
        //printf("lecture buffer 0 \r\n");
        
        if (C1RX0SIDbits.RXIDE == 0)
        {
            message->ID = (C1RX0SID >> 2);
        }
        
        else
        {
           message->ID = (unsigned long long)C1RX0SIDbits.SID * 262144 + (unsigned long long)C1RX0EID * 64 + (unsigned long long)C1RX0DLCbits.EID5_0;
        
        }
        //récupération data
        message->data[0] = C1RX0B1;
        message->data[1] = C1RX0B1>>8;
        message->data[2] = C1RX0B2;
        message->data[3] = C1RX0B2>>8;
        message->data[4] = C1RX0B3;
        message->data[5] = C1RX0B3>>8;
        message->data[6] = C1RX0B4;
        message->data[7] = C1RX0B4>>8;
        
        //récupération length
        message->len = C1RX0DLCbits.DLC;
        C1INTFbits.RX0IF = 0;
        C1RX0CONbits.RXFUL = 0;
        
    }
    }
    
    else if (buffer==1)
        
    {
        if(C1RX1CONbits.RXFUL)
    {   
        if (C1RX1SIDbits.RXIDE == 0)
        {
            message->ID =   (C1RX1SID >> 2);
        }
        
        else
        {
            message->ID = (unsigned long long)C1RX1SIDbits.SID * 262144 + (unsigned long long)C1RX1EID * 64 + (unsigned long long)C1RX1DLCbits.EID5_0;
        }
        //récupération data
        message->data[0] = C1RX1B1;
        message->data[1] = C1RX1B1>>8;
        message->data[2] = C1RX1B2;
        message->data[3] = C1RX1B2>>8;
        message->data[4] = C1RX1B3;
        message->data[5] = C1RX1B3>>8;
        message->data[6] = C1RX1B4;
        message->data[7] = C1RX1B4>>8;
        
        //récupération length
        message->len = C1RX1DLCbits.DLC;
   
        C1INTFbits.RX1IF = 0;
        C1RX1CONbits.RXFUL = 0;
        }
    }
    /*
    printf("SID : %#010x - %d\r\n", C1RX0SIDbits.SID,C1RX0SIDbits.SID);
    printf("EID : %#010x - %d\r\n", C1RX0EID,C1RX0EID);
    printf("DLC : %#010x - %d\r\n", C1RX0DLCbits.EID5_0,C1RX0DLCbits.EID5_0);
    printf("DATA : \r\n%#010x\r\n", message->data[0]);
    printf("%#010x\r\n", message->data[1]);
    printf("%#010x\r\n", message->data[2]);
    printf("%#010x\r\n", message->data[3]);
    printf("%#010x\r\n", message->data[4]);
    printf("%#010x\r\n", message->data[5]);
    printf("%#010x\r\n", message->data[6]);
    printf("%#010x\r\n", message->data[7]);
    printf("ID message : %llu\r\n", message->ID);
    */
    return 0;
}  


/* 
 * File:   can.h
 * Author: 
 *
 * Created on 28 mars 2017, 17:00
 */

#ifndef CAN_H
#define	CAN_H
#include <xc.h>

/**** Prototypes fonctions CAN ****/


#endif	/* CAN_H */




typedef struct{unsigned long long ID; uint8_t len; uint8_t data[8];} CANMSG;

void initCAN(void);
void sendCANFrame(CANMSG *msg);
//int getCANFrame(CANMSG *message, int buffer);
int getCANFrame(CANMSG *message, int buffer);


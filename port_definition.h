/*
Define the hardware port of the mbed.
*/

#ifndef PORT_DEFINITION_H
#define PORT_DEFINITION_H

#include "mbed.h"

extern Serial PC_serial; //these three extern parameters will be define in the "port_definition.cpp"
extern Serial XBEE_serial;
extern Serial GPS_serial;

extern DigitalOut PC_led; //for PC rx/tx
extern DigitalOut XBEE_led; //for XBEE rx/tx
extern DigitalOut GPS_led; //for GPS rx/tx

extern void init_PORT();

#endif
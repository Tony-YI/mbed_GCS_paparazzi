/*
This header file include all the constant value used in the GCS.
*/

#ifndef CONSTANT_VALUE_H
#define CONSTANT_VALUE_H

//maximum length of unsigned char buffer
#define MAX_LENGTH 512

//maximun length of command line
#define CMDLINE_LENGTH 3

//address size of xbee module xbee_pro_s2b
#define ADDR_SIZE 8 //8 bytes = 64 bits

//the baudrate of XBEE_serial port
#define PC_serial_baud 9600
#define PCTX USBTX
#define PCRX USBRX

//the baudrate of XBEE_serial port
#define XBEE_serial_baud 57600
#define XBEETX p9
#define XBEERX p10

//the baudrate of GPS_serial port
#define GPS_serial_baud 38400
#define GPSTX p13
#define GPSRX p14

//the ac_id of GCS is 0
#define GCS_ac_id 0

#endif
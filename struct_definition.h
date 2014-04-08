/*
In this file, we define most of the structurs that we will use in the whole program.
*/

#ifndef STRUCT_DEFINITION_H
#define STRUCT_DEFINITION_H

#include "mbed.h"
#include "constant_value.h"
#include "message_xbee_pro_s2b.h"
//#include "message_struct_definition.h"

struct GPS_UART //struct for communication with GPS
{
    uint8_t uart_id;
    Serial *GPS_serial; //serial port that connceted to the GPS module
    struct Message_Gps_Data *gps_data; //a data structure that will send to the aircraft from the GCS
    unsigned char buf[MAX_LENGTH];
    unsigned char data[MAX_LENGTH];
    int in;
    int out;
    unsigned char state, lstate, code, id, chk1, chk2, ck1, ck2;
    unsigned int length, idx, cnt;
    bool GpsReady;
    bool CheckOk;
    bool InfoOk;
};

struct XBEE_UART //struct for communication with XBEE module
{
    uint8_t uart_id;
    Serial *XBEE_serial; //serial port that connected to the XBEE module
    unsigned char buf[MAX_LENGTH]; //store the API frame
    unsigned char data[MAX_LENGTH]; //store the RF data
    unsigned int in; //rx
    unsigned int out; //retrive
    uint8_t state; //for Xbee_Pares_Message state machine
    uint8_t xbee_cs; //for check sum
    uint16_t length; //the length get from the API message
    uint16_t payload_length; //record how many bytes have been received
    bool CheckOk; //check checksum
    bool InfoOk; //message is ready
};

struct AC //struct to identify an aircraft
{
    uint8_t ac_id;
    unsigned char address[ADDR_SIZE];
};// can ignore __attribute__((packet));

struct COMMAND_LINE //struct of command line typed in by the user
{
    //support maximum 16-bit
    int command_id; //0 means wrong command line
    int ac_nb; //0 means not a number, we define -1 means all aircrafts
};// can ignore __attribute__((packet));

#endif
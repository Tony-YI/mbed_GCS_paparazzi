/*
This is the implementation of "prot_definition.h"
*/

#include "constant_value.h"
#include "port_definition.h"

Serial PC_serial(PCTX, PCRX);
Serial XBEE_serial(XBEETX, XBEERX);
Serial GPS_serial(GPSTX, GPSRX);

DigitalOut PC_led(LED1);
DigitalOut XBEE_led(LED2);
DigitalOut GPS_led(LED3);

void init_PORT() //used in main() function
{
    PC_serial.baud(PC_serial_baud);
    XBEE_serial.baud(XBEE_serial_baud);
    GPS_serial.baud(GPS_serial_baud);
}
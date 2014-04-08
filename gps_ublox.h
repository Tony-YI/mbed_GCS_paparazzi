/*
This file is for us to read data from the GPS with ublox protocol.
*/

#ifndef GPS_UBLOX_H
#define GPS_UBLOX_H

#include "struct_definition.h"
#include "message_struct_definition.h"
#include "port_definition.h"
#include "constant_value.h"

#define  SYNC1       0xB5
#define  SYNC2       0x62
#define  SOL_MSG     0x06

#define INT_32_ADDR(X,gps_uart)    *(int32_t *)(&(gps_uart->data[X]))
#define UINT_32_ADDR(X,gps_uart)   *(uint32_t *)(&(gps_uart->data[X]))
#define INT_8_ADDR(X,gps_uart)     *(int8_t *)(&(gps_uart->data[X]))
#define UINT_8_ADDR(X,gps_uart)    *(uint8_t *)(&(gps_uart->data[X]))

#define INT_32(X,gps_uart)    *(int32_t *)(&(gps_uart.data[X]))
#define UINT_32(X,gps_uart)   *(uint32_t *)(&(gps_uart.data[X]))
#define INT_8(X,gps_uart)     *(int8_t *)(&(gps_uart.data[X]))
#define UINT_8(X,gps_uart)    *(uint8_t *)(&(gps_uart.data[X]))

extern struct Message_Gps_Data message_gps_data_GCS;

extern struct GPS_UART gps_uart;

extern void Gps_Send_Command(unsigned char len, uint8_t data[], struct GPS_UART * gps_uart);

extern void Gps_Enable_Message(unsigned char id, bool enable, struct GPS_UART *gps_uart, int rate = 1);

extern void Gps_Parse_Message(struct GPS_UART *gps_uart);

extern void Gps_Rx_Handler();

extern void init_GPS();

extern void test_GPS(); //for debugging

#endif // #ifdef ublox_GPS.h
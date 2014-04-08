/*
This file define the message format of data sending and receving from the xbee_pro_s2b module.

tx_xbee_pro_s2b-message: ABCDxxxxxxxE
    A XBEE_PRO_S2B_START (0x7E)
    B LENGTH_MSB (D->D)
    C LENGTH_LSB
    D XBEE_PRO_S2B_PAYLOAD
      0 FRAME_TYPE_TX (0x10)                   / FRAME_TYPE_RX (0x90)
      1 FRAME_ID (0x00)                        / 0x00(NO ACK) 0x01(ACK)
      2 64-BIT DESITINATION ADDRESS            /0x000000000000FFFF is broadcast, 0x0000000000000000 is coordinator
        0 ADDRESS MSB
        1
        2
        3
        4
        5
        6
        7 ADDRESS LSB
      3 16-BIT DESITINATION NETWORK ADDRESS    /0xFFFE if you don't know the specific number
        0 MSB
        1 LSB
      4 BROADCAST RADIUS (0x00)                /0x00 is the maximum radius
      5 TX_OPTIONS (0x00)                         /for more information, please refer to the digi manual
      6 PPRZ_DATA(RF_DATA)
        0 SENDER_ID                            /ac_id
        1 MSG_ID                               /refer to paparazzi "message.xml" file
        2 MSG_PAYLOAD
        . DATA (messages.xml)
    E XBEE_CHECKSUM (0xFF - sum[D->D])
    
rx_xbee_pro_s2b-message: ABCDxxxxxxxE
    A XBEE_PRO_S2B_START (0x7E)
    B LENGTH_MSB (D->D)
    C LENGTH_LSB
    D XBEE_PRO_S2B_PAYLOAD
      0 FRAME_TYPE_RX (0x90)                   / FRAME_TYPE_TX (0x10)
      1 64-BIT SOURCE ADDRESS                  /0xFFFFFFFFFFFFFFFF if sender is unknown
        0 ADDRESS MSB
        1
        2
        3
        4
        5
        6
        7 ADDRESS LSB
      2 16-BIT NETWORK ADDRESS                 / 16-bit address of sender
        0 MSB
        1 LSB
      3 RX_OPTIONS                             /0x01, 0x02, 0x20, 0x40
      5 PPRZ_DATA(RF_DATA)
        0 SENDER_ID                            /ac_id
        1 MSG_ID                               /refer to paparazzi "message.xml" file
        2 MSG_PAYLOAD
        . DATA (messages.xml)
    E XBEE_CHECKSUM (0xFF - sum[D->D])
*/

#ifndef MESSAGE_XBEE_PRO_S2B_H
#define MESSAGE_XBEE_PRO_S2B_H

#include "struct_definition.h"
#include "constant_value.h"
#include "port_definition.h"

#define XBEE_START 0x7E
#define FRAME_TYPE_TX 0x10
#define FRAME_TYPE_RX 0x90
#define FRAME_ID 0x00
#define DESITINATION_NET_WOEK_ADDRESS_MSB 0xFF
#define DESITINATION_NET_WOEK_ADDRESS_LSB 0xFE
#define BROADCAST_RADIUS 0x00
#define TX_OPTIONS 0x00
#define TX_HEADER_LENGTH 16 //start with MSG_PAYLOAD
#define RX_HEADER_LENGTH 14 //start with MSG_PAYLOAD
#define TX_OFFSET 14 //start with PPRZ_DATA
#define RX_OFFSET 12 //start with PPRZ_DATA

#define Xbee_Put_1_Byte(_data){\
    XBEE_serial.putc(_data);\
}//without checksum
#define Xbee_Put_Uint8(_data, _cs){\
    Xbee_Put_1_Byte(_data);\
    _cs += _data;\
}//with checksum
#define Xbee_Put_1_Byte_By_Address(_data, _cs){\
    Xbee_Put_Uint8(*(const uint8_t*) _data, _cs);\
}//with checksum
#define Xbee_Put_2_Byte_By_Address(_data, _cs){\
    Xbee_Put_Uint8(*(const uint8_t*) _data, _cs);\
    Xbee_Put_Uint8(*(((const uint8_t *) _data) + 1), _cs);\
}//with checksum
#define Xbee_Put_4_Byte_By_Address(_data, _cs){\
   Xbee_Put_2_Byte_By_Address(((const uint16_t *) _data), _cs);\
   Xbee_Put_2_Byte_By_Address((((const uint16_t *) _data) + 1), _cs);\
}//with checksum

extern void Xbee_Put_Header(struct AC *ac, uint16_t length, uint8_t *cs, uint8_t message_id);

extern void Xbee_Put_Tailer(uint8_t cs);

extern void Xbee_Rx_Handler();

extern void init_XBEE();

extern void Xbee_Parse_API(struct XBEE_UART* xbee_uart);

extern void Xbee_Parse_Message(struct XBEE_UART* xbee_uart);

extern void test_XBEE(); //for debugging

#endif
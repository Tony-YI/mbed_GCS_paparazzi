/*
This is a header file for us to input the 64-bit serial number of the xbee_pro_s2b
module on the aircraft.

In the main() function, we can use "init AC()" to define an aircraft with ac_id and 
xbee_pro_s2b module address.

Every specific aircraft should has a unique ac_id and xbee_pro_s2b_address.
*/

#ifndef ADDRESS_XBEE_PRO_S2B_H
#define ADDRESS_XBEE_PRO_S2B_H

#include "constant_value.h"
#include "struct_definition.h"

//AC is struct AC, defined in "struct_definition.h"
#define Set_AC(_AC, _ac_id, _address_high_32, _address_low_32){\
    _AC.ac_id = (uint8_t)_ac_id;\
    _AC.address[0] = (uint8_t)(_address_high_32 >> 24);\
    _AC.address[1] = (uint8_t)(_address_high_32 >> 16);\
    _AC.address[2] = (uint8_t)(_address_high_32 >> 8);\
    _AC.address[3] = (uint8_t)_address_high_32;\
    _AC.address[4] = (uint8_t)(_address_low_32 >> 24);\
    _AC.address[5] = (uint8_t)(_address_low_32 >> 16);\
    _AC.address[6] = (uint8_t)(_address_low_32 >> 8);\
    _AC.address[7] = (uint8_t)_address_low_32;\
}

#endif
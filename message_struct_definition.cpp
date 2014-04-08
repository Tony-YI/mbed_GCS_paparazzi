/*
Implementation of "message_struct_definition.h"
*/

#include "message_struct_definition.h"

void Send_Message_Set_Home_Point_Here(struct Message_Set_Home_Point_Here *message_set_home_point_here, struct AC *ac)
{
}

//go to block "Holding Point" in the flight_plane
void Send_Message_Holding_Point(struct Message_Holding_Point *message_holding_point, struct AC *ac)
{
    uint8_t xbee_cs = 0;
    uint16_t length = TX_HEADER_LENGTH + sizeof(struct Message_Holding_Point);
    
    __disable_irq();
    Xbee_Put_Header(ac, length, &xbee_cs, (uint8_t)Message_Holding_Point_ID);
    Xbee_Put_Uint8(Message_Holding_Point_BLOCK_ID, xbee_cs);
    PC_serial.printf("%.2x", Message_Holding_Point_BLOCK_ID);
    Xbee_Put_Uint8(ac->ac_id, xbee_cs);
    PC_serial.printf("%.2x", ac->ac_id);
    Xbee_Put_Tailer(xbee_cs);
   __enable_irq();
}

//go to block "Start Engine" in the flight_plane
void Send_Message_Start_Engine(struct Message_Start_Engine *message_start_engine, struct AC *ac)
{
    uint8_t xbee_cs = 0;
    uint16_t length = TX_HEADER_LENGTH + sizeof(struct Message_Start_Engine);
    
    __disable_irq();
    Xbee_Put_Header(ac, length, &xbee_cs, (uint8_t)Message_Start_Engine_ID);
    Xbee_Put_Uint8(Message_Start_Engine_BLOCK_ID, xbee_cs);
    PC_serial.printf("%.2x", Message_Start_Engine_BLOCK_ID);
    Xbee_Put_Uint8(ac->ac_id, xbee_cs);
    PC_serial.printf("%.2x", ac->ac_id);
    Xbee_Put_Tailer(xbee_cs);
   __enable_irq();
}

//go to block "TakeOff" in the flight_plane
void Send_Message_Take_Off(struct Message_Take_Off *message_take_off, struct AC *ac)
{
    uint8_t xbee_cs = 0;
    uint16_t length = TX_HEADER_LENGTH + sizeof(struct Message_Take_Off);
    
    __disable_irq();
    Xbee_Put_Header(ac, length, &xbee_cs, (uint8_t)Message_Take_Off_ID);
    Xbee_Put_Uint8(Message_Take_Off_BLOCK_ID, xbee_cs);
    PC_serial.printf("%.2x", Message_Take_Off_BLOCK_ID);
    Xbee_Put_Uint8(ac->ac_id, xbee_cs);
    PC_serial.printf("%.2x", ac->ac_id);
    Xbee_Put_Tailer(xbee_cs);
   __enable_irq();
}

void Send_Message_Land_Here(struct Message_Land_Here *message_land_here, struct AC *ac)
{
}

void Send_Message_Land_Home(struct Message_Land_Home *message_land_home, struct AC *ac)
{
}

void Send_Message_Gps_Data(struct Message_Gps_Data *message_gps_data, struct AC *ac)
{   
    uint8_t xbee_cs = 0;
    uint16_t length = TX_HEADER_LENGTH + sizeof(struct Message_Gps_Data);
    
   //PC_serial.printf("Sending Msg\r\n");
   __disable_irq(); //disable interupt when sending data
   Xbee_Put_Header(ac, length, &xbee_cs, (uint8_t)Message_Gps_Data_ID); //defined in "message_xbee_pro_s2b.h"
   
   //Message content Start//
   Xbee_Put_Uint8(message_gps_data->ac_id, xbee_cs);
   Xbee_Put_Uint8(message_gps_data->GPS_FIX, xbee_cs);
   Xbee_Put_4_Byte_By_Address(&message_gps_data->ecef_pos_cm_x, xbee_cs);
   Xbee_Put_4_Byte_By_Address(&message_gps_data->ecef_pos_cm_y, xbee_cs);
   Xbee_Put_4_Byte_By_Address(&message_gps_data->ecef_pos_cm_z, xbee_cs);
   //Message content End//
      
   Xbee_Put_Tailer(xbee_cs);
   __enable_irq();
   //PC_serial.printf("Sending Msg Done\r\n");
}
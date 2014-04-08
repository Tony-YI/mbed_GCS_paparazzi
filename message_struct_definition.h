/*
This file records the data structurs of the message that used to communicate between
the aircraft and the GCS.

More information about the messages, please refer to the paparazzi's "message.xml" file.

One message example is:
struct Message_..
{
    uint8_t ac_id;
    .
    .
    .
    .
    .
    .
}__attribute__((packed));
*/

#ifndef MESSAGE_STRUCT_DEFINITION_H
#define MESSAGE_STRUCT_DEFINITION_H

#include "constant_value.h"
#include "message_xbee_pro_s2b.h"
#include "struct_definition.h"

/**************** Messages from aircraft to GCS *********************/

/********************************************************************/



/**************** Messages from GCS to aircraft *********************/
//#define Message_Set_Home_Point_Here_ID //pleasse refer to the paparazzi "message.xml" file
struct Message_Set_Home_Point_Here //set the currently location as the homepoint, refer to the paparazzi/con/flight_plane/flight_plane.xml
{
    uint8_t ac_id;
}__attribute__((packed));
extern void Send_Message_Set_Home_Point_Here(struct Message_Set_Home_Point_Here *message_set_home_point_here, struct AC *ac); //must add "static inline"

#define Message_Holding_Point_ID 5 //refer to paparazzi, var, dl_protocol.h
#define Message_Holding_Point_BLOCK_ID 2 //refer to /paparazzi/var/MY_AC/generated/flight_plan.h
struct Message_Holding_Point
{
    uint8_t block_id; //refer to the flight_plane.xml
    uint8_t ac_id;
}__attribute__((packed));
extern void Send_Message_Holding_Point(struct Message_Holding_Point *message_holding_point, struct AC *ac);

#define Message_Start_Engine_ID 5 //refer to paparazzi, var, dl_protocol.h
#define Message_Start_Engine_BLOCK_ID 3 //refer to /paparazzi/var/MY_AC/generated/flight_plan.h
struct Message_Start_Engine
{
    uint8_t block_id; //refer to the flight_plane.xml
    uint8_t ac_id;
}__attribute__((packed));
extern void Send_Message_Start_Engine(struct Message_Start_Engine *message_start_engine, struct AC *ac);

#define Message_Take_Off_ID 5 //refer to paparazzi, var, dl_protocol.h
#define Message_Take_Off_BLOCK_ID 4 //refer to /paparazzi/var/MY_AC/generated/flight_plan.h
struct Message_Take_Off
{
    uint8_t block_id; //refer to the flight_plane.xml
    uint8_t ac_id;
}__attribute__((packed));
extern void Send_Message_Take_Off(struct Message_Take_Off *message_take_off, struct AC *ac);

//#define Message_Land_Here_ID
struct Message_Land_Here
{
    uint8_t ac_id;
}__attribute__((packed));
extern void Send_Message_Land_Here(struct Message_Land_Here *message_land_here, struct AC *ac);

//#define Message_Land_Home_ID
struct Message_Land_Home
{
    uint8_t ac_id;
}__attribute__((packed));
extern void Send_Message_Land_Home(struct Message_Land_Home *message_land_home, struct AC *ac);
/********************************************************************/



/**************** Messages of both directions ***********************/
#define Message_Gps_Data_ID 14 //paparazzi "message.xml", class "datalink", "message HOVER_POINT_CHANGE"
struct Message_Gps_Data
{
    uint8_t ac_id;
    uint8_t GPS_FIX;
    uint32_t iTOW;
    uint32_t ecef_pos_cm_x;
    uint32_t ecef_pos_cm_y;
    uint32_t ecef_pos_cm_z;
    uint32_t accuracy_3D;
}__attribute__((packed)); //very important line
extern void Send_Message_Gps_Data(struct Message_Gps_Data *message_gps_data, struct AC *ac);
/********************************************************************/

#endif
/*
Define some functions that may be used in the main() function.
*/

#ifndef GCS_H
#define GCS_H

#include "address_xbee_pro_s2b.h"
#include "struct_definition.h"
#include "message_struct_definition.h"
#include "constant_value.h"

extern struct COMMAND_LINE cmdLine;
extern void init_GCS();
extern void Reset_Cmdline();
extern void Gcs_Parse_Cmdline();
extern void tset_GCS();

//add more AC as you like
/* More AC */
#define AC_NUM 2
extern struct AC ac[AC_NUM + 1]; //consider the GCS as a dummy ac with ac_id = 0
extern uint32_t MSB[AC_NUM + 1];
extern uint32_t LSB[AC_NUM + 1];
/* End of AC */

//init the ACs
extern void init_AC();

//functions
extern void Set_Home_Point_Here(struct AC* ac);
extern void Take_Off(struct AC* ac);
extern void Land_Here(struct AC* ac);
extern void Land_Home(struct AC* ac);
extern void Kill(struct AC* ac);

#endif
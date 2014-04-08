#include "mbed.h"
#include "address_xbee_pro_s2b.h"
#include "message_xbee_pro_s2b.h"
#include "gps_ublox.h"
#include "gcs.h"

int main()
{
    init_PORT();
    init_AC();
    init_GPS();
    init_XBEE();
    init_GCS();
    
    while(1)
    {
        switch(cmdLine.command_id)
        {
            case 0: //command line is wrong
                PC_serial.printf("\r\nWelcome to Ting-GCS:");
                Gcs_Parse_Cmdline();
                break;
            case 1:
                tset_GCS();
                Reset_Cmdline();
                break;
            case 2:
                tset_GCS();
                Reset_Cmdline();
                break;
            case 3:
                tset_GCS();
                Reset_Cmdline();
                break;
            case 4:
                tset_GCS();
                Reset_Cmdline();
                break;
            default:
                tset_GCS();
                Reset_Cmdline();
                break;
        }
    }
}
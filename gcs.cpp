/*
Implementation of "gcs.h"
*/

#include "gcs.h"

struct COMMAND_LINE cmdLine;
void init_GCS()
{
    memset(&cmdLine, 0 , sizeof(cmdLine));
}
void Reset_Cmdline()
{
    init_GCS();
}
void tset_GCS()
{
    PC_serial.printf("command_id: %d, ac_nb(-1 is all): %d\r\n", cmdLine.command_id, cmdLine.ac_nb);
}

void Gcs_Parse_Cmdline()
{
    char buf[CMDLINE_LENGTH + 1]; //1 for '\0'
    uint8_t state = 0;
    uint8_t i = 0;
    uint8_t l = 0;
    char cc;
    
    if(state == 0) //waitting command
    {
        state++;
        for(i = 0; i < CMDLINE_LENGTH; i++)
        {
            cc = PC_serial.getc();
            if(cc == '\r' || cc == ' ')
            {
                if(i == 0)
                {  
                    state = 2;
                }
                i = CMDLINE_LENGTH;
            }
            else
            {   
                PC_serial.putc(cc);
                buf[l] = cc;
                l++;
            }
        }
        if(i >= CMDLINE_LENGTH)
        {
            PC_serial.putc(' ');
        }
        buf[l] = '\0';
        cmdLine.command_id = atoi(buf); // 0 means not a command
        l = 0;
    }
    if(state == 1) //waitting ac_nb, 0 represent all aircraft
    {
        for(i = 0; i < CMDLINE_LENGTH; i++)
        {
            cc = PC_serial.getc();
            if(cc == '\r' || cc == ' ')
            {
                if(i == 0)
                {   
                    state = 3;
                }
                i = CMDLINE_LENGTH;
            }
            else
            {   
                PC_serial.putc(cc);
                buf[l] = cc;
                l++;
            }
        }
        buf[l] = '\0';
        cmdLine.ac_nb = atoi(buf); //0 means not a number, we define -1 means all aircrafts
        PC_serial.printf("\r\n");
    }
    if(state == 2)
    {
        PC_serial.printf("\r\nERROR 2\r\n");
        cmdLine.command_id = 0;
        cmdLine.ac_nb = 0;
    }
    if(state == 3)
    {
        PC_serial.printf("\r\nERROR 3\r\n");
        cmdLine.command_id = 0;
        cmdLine.ac_nb = 0;
    }
}

//add more AC as you like
/* More AC */
struct AC ac[AC_NUM + 1];
uint32_t MSB[AC_NUM + 1];
uint32_t LSB[AC_NUM + 1];
/* End of AC */

void init_AC()
{
    memset(ac, 0, sizeof(struct AC) * (AC_NUM + 1));
    memset(MSB, 0, sizeof(uint32_t) * (AC_NUM + 1));
    memset(LSB, 0, sizeof(uint32_t) * (AC_NUM + 1));
    
    //GCS has ac_id = 0, ADDR=0x******** 0x********
    MSB[0] = 0x00000000;  LSB[0] = 0x00000000;
    MSB[1] = 0x0013A200;  LSB[1] = 0x409C27B4;
    MSB[2] = 0x0013A200;  LSB[2] = 0x409C278A;
    int nb;
    for(nb = 0; nb < AC_NUM + 1; nb++)
    {
        //Set_AC(ac_1, 1, 0x0013A200, 0x409C27B4);
        //Set_AC(ac_2, 2, 0x0013A200, 0x409C278A);
        Set_AC(ac[nb], nb, MSB[nb], LSB[nb]);
    }
}

void Set_Home_Point_Here(struct AC* ac)
{
}

void Take_Off(struct AC* ac)
{
    struct Message_Start_Engine message_start_engine;//no need to modefy the connent
    struct Message_Holding_Point message_holding_point;//no need to modefy the connent
    struct Message_Take_Off message_take_off;//no need to modefy the connent
    
    Send_Message_Holding_Point(&message_holding_point, ac);
    Send_Message_Start_Engine(&message_start_engine, ac);
    Send_Message_Take_Off(&message_take_off, ac);
}

void Land_Here(struct AC* ac)
{
}

void Land_Home(struct AC* ac)
{
}

void Kill(struct AC* ac)
{
}
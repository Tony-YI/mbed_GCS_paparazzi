/*
Implementation of "gps_ublox.h"
*/

#include "gps_ublox.h"

struct Message_Gps_Data message_gps_data_GCS;
struct GPS_UART gps_uart;

void Gps_Send_Command(unsigned char len, uint8_t data[], struct GPS_UART * gps_uart)
{
   unsigned char chk1 = 0, chk2 = 0;

   gps_uart->GPS_serial->putc(SYNC1);
   gps_uart->GPS_serial->putc(SYNC2);
   for(unsigned char ii = 0; ii < len; ii++)
   {
       gps_uart->GPS_serial->putc(data[ii]);
       gps_uart->chk1 += data[ii];
       gps_uart->chk2 += chk1;
   }
   gps_uart->GPS_serial->putc(chk1);
   gps_uart->GPS_serial->putc(chk2);
}

void Gps_Enable_Message(unsigned char id, bool enable, struct GPS_UART * gps_uart, int rate)
{
   if (!enable) rate = 0;
   uint8_t cmdBuf[] = {
       0x06,   // class CFG
       0x01,   // id MSG -> CFG-MSG
       8,      // length, for config message rates
       0x01,   // class,
       id,     // id,
       0x0,    // target 0 rate (DDC/I2C)
       rate,   // target 1 rate (UART1)
       0x0,    // target 2 rate (UART2)
       0x0,    // target 3 rate (USB)
       0x0,    // target 4 rate (SPI)
       0x0,    // target 5 rate (reserved)
   };
   Gps_Send_Command(sizeof(cmdBuf), cmdBuf, gps_uart);
}

void Gps_Parse_Message(struct GPS_UART* gps_uart)
{
   if (gps_uart->in != gps_uart->out) //this means there is data in the buff of gps_uart
       {
           unsigned char cc = gps_uart->buf[gps_uart->out++];
           gps_uart->out &= (MAX_LENGTH-1);

           switch (gps_uart->state)
           {
               case 0:    // wait for sync 1 (0xB5)
                   gps_uart->ck1 = gps_uart->ck2 = 0;
                   gps_uart->CheckOk = false;
                   gps_uart->InfoOk = false;
                   if (cc == SYNC1)
                       gps_uart->state++;
                   else
                   {   
                      #if CHECK_SYN1
                      PC_serial.printf("UART %d NOT SYNC1\r\n",gps_uart->uart_id);
                      #endif
                   }
                   break;
               case 1:    // wait for sync 2 (0x62)
                   if (cc == SYNC2)
                       gps_uart->state++;
                   else
                   {
                       gps_uart->state = 0;
                       #if CHECK_SYN2
                       PC_serial.printf("UART %d NOT SYNC2\r\n", gps_uart->uart_id);
                       #endif
                   }
                   break;
               case 2:    // wait for class code
                   gps_uart->code = cc;
                   gps_uart->ck1 += cc;
                   gps_uart->ck2 += gps_uart->ck1;
                   gps_uart->state++;
                   break;
               case 3:    // wait for Id
                   gps_uart->id = cc;
                   gps_uart->ck1 += cc;
                   gps_uart->ck2 += gps_uart->ck1;
                   gps_uart->state++;
                   break;
               case 4:    // wait for length uint8_t 1
                   gps_uart->length = cc;
                   gps_uart->ck1 += cc;
                   gps_uart->ck2 += gps_uart->ck1;
                   gps_uart->state++;
                   break;
               case 5:    // wait for length uint8_t 2
                   gps_uart->length |= (unsigned int) cc << 8;
                   gps_uart->ck1 += cc;
                   gps_uart->ck2 += gps_uart->ck1;
                   gps_uart->idx = 0;
                   gps_uart->state++;
                   if (gps_uart->length > MAX_LENGTH)
                   {
                       gps_uart->state= 0;
                       #if USE_SCREEN
                       PC_serial.printf("Message too long\r\n");
                       #endif
                   }
                   break;
               case 6:    // wait for <length> payload uint8_ts
                   gps_uart->data[gps_uart->idx++] = cc;
                   gps_uart->ck1 += cc;
                   gps_uart->ck2 += gps_uart->ck1;
                   if (gps_uart->idx >= gps_uart->length)
                   {
                       gps_uart->state++;
                   }
                   break;
               case 7:    // wait for checksum 1
                   gps_uart->chk1 = cc;
                   gps_uart->state++;
                   break;
               case 8: {  // wait for checksum 2
                   gps_uart->chk2 = cc;
                   gps_uart->CheckOk = gps_uart->ck1 == gps_uart->chk1  &&  gps_uart->ck2 == gps_uart->chk2;
                   if (!gps_uart->CheckOk)
                   {
                       //GPS_led = 0;//led off when get a invalid message from GPS
                       PC_serial.printf("Uart %d Check sum incorrect\r\n", gps_uart->uart_id);
                   }
                   else//get a valid message from GPS
                   {
                       //GPS_led = 1;//led on when get a valid message from GPS
                       switch (gps_uart->code)
                       {
                           case 0x01:      // NAV-
                               switch (gps_uart->id)
                               {
                                   case SOL_MSG:  // NAV-SOL // we need this, ecef_pos_x, ecef_pos_y, ecef_pos_z, GPS_FIX, the paparazzi uses this type of gps message
                                       //GPS_led = (UINT_8(10, gps_uart) == 0x03);//get a GPS 3D-FIX
                                       #if USE_SCREEN
                                       PC_serial.printf("GPS%d GPS_FIX %d X:%d Y:%d Z:%d ACCUR:%d\r\n",gps_uart->uart_id,UINT_32(10,gps_uart),UINT_32(12,gps_uart),UINT_32(16,gps_uart),UINT_32(20,gps_uart),UINT_32(24,gps_uart));
                                       #endif
                                       
                                       #if USE_USB
                                       PC_serial.printf("GPS%d %d %d %d %d %d\r\n",gps_uart->uart_id,UINT_32(10,gps_uart),UINT_32(12,gps_uart),UINT_32(16,gps_uart),UINT_32(20,gps_uart),UINT_32(24,gps_uart));
                                       #endif
                                       /*
                                       PC_serial.printf("UART %d GPS_FIX = %x\r\n", gps_uart->uart_id ,UINT_8(10,gps_uart) );
                                       PC_serial.printf("UART %d ECEF_POS_X = %d\r\n", gps_uart->uart_id ,INT_32(12,gps_uart) );//in cm
                                       PC_serial.printf("UART %d ECEF_POS_Y = %d\r\n", gps_uart->uart_id ,INT_32(16,gps_uart) );//in cm
                                       PC_serial.printf("UART %d ECEF_POS_Z = %d\r\n", gps_uart->uart_id ,INT_32(20,gps_uart) );//in cm
                                       PC_serial.printf("UART %d 3D_POS_ACCURACY = %d\r\n\r\n",gps_uart->uart_id,(uint32_t)INT_32(24,gps_uart));//Accuray
                                       */
                                       
                                       gps_uart->gps_data->iTOW = UINT_32_ADDR(0, gps_uart);
                                       gps_uart->gps_data->GPS_FIX = UINT_8_ADDR(10, gps_uart);
                                       gps_uart->gps_data->ecef_pos_cm_x = INT_32_ADDR(12, gps_uart);
                                       gps_uart->gps_data->ecef_pos_cm_y = INT_32_ADDR(16, gps_uart);
                                       gps_uart->gps_data->ecef_pos_cm_z = INT_32_ADDR(20, gps_uart);
                                       gps_uart->gps_data->accuracy_3D = INT_32_ADDR(24, gps_uart);
                                       gps_uart->InfoOk = 1;
                                       break;
                                   default:
                                       break;
                               }
                               break;

                           default:
                               break;
                       }
                   }

                   gps_uart->state = 0;
                   break;
               }

               default:
                   break;
           }
       }
}

void Gps_Rx_Handler()
{   
   //PC_serial.printf("ENTER RX_HANDLER\r\n");
   __disable_irq();//disable interupt when receiving data from GPS_UART
   gps_uart.buf[gps_uart.in++] = gps_uart.GPS_serial->getc();
   gps_uart.in &= (MAX_LENGTH-1);
   __enable_irq();
   //PC_serial.printf("EXIT RX_HANDLER\r\n");
   //PC_serial.printf("GPS1 GOT SOMETHING\r\n");
   //GPS_led = !GPS_led;
}

void init_GPS()
{
    memset(&message_gps_data_GCS, 0, sizeof(struct Message_Gps_Data));
    memset(&gps_uart, 0, sizeof(struct GPS_UART));
    message_gps_data_GCS.ac_id = GCS_ac_id;
    gps_uart.GPS_serial = &GPS_serial; //attach the serial port to this struct
    gps_uart.gps_data = &message_gps_data_GCS; //attach the Message_Gps_Data struct into the gps_uart
    GPS_serial.attach(&Gps_Rx_Handler); //attach the rx handler to the serial port
    
    //configure GPS
    uint8_t cmdbuf[40];
    for (int i=0; i < 38; i++)
    {
        cmdbuf[i] = 0;
    }
    cmdbuf[0] = 0x06; //NAV-CFG5
    cmdbuf[1] = 0x24; //NAV-CFG5
    cmdbuf[2] = 0x00; //X2 bitmask
    cmdbuf[3] = 0x01; //bitmask: dynamic model
    cmdbuf[4] = 0x04; //U1 automotive dyn model
    Gps_Send_Command(38, cmdbuf, &gps_uart);
    
    // Modify these to control which messages are sent from module
    Gps_Enable_Message(SOL_MSG, true, &gps_uart); //Enable soluton messages
}

void test_GPS() //for debugging
{
    Gps_Parse_Message(&gps_uart);
    if(gps_uart.InfoOk)
    {
        gps_uart.InfoOk = false;
        PC_serial.printf("UART %d GPS_FIX = %x\r\n", gps_uart.uart_id, UINT_8(10, gps_uart) );
        PC_serial.printf("UART %d ECEF_POS_X = %d\r\n", gps_uart.uart_id, INT_32(12, gps_uart) );//in cm
        PC_serial.printf("UART %d ECEF_POS_Y = %d\r\n", gps_uart.uart_id, INT_32(16, gps_uart) );//in cm
        PC_serial.printf("UART %d ECEF_POS_Z = %d\r\n", gps_uart.uart_id, INT_32(20, gps_uart) );//in cm
        PC_serial.printf("UART %d 3D_POS_ACCURACY = %d\r\n\r\n", gps_uart.uart_id, (uint32_t)INT_32(24, gps_uart));//Accuray
    }
    return;
}
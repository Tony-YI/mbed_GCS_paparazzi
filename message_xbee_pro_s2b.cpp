/*
Implementation of "message_xbee_pro_s2b.h"
*/

#include "message_xbee_pro_s2b.h"

void Xbee_Put_Header(struct AC *ac, uint16_t length, uint8_t *cs, uint8_t message_id)
{
    Xbee_Put_1_Byte(XBEE_START);
    PC_serial.printf("%.2x", XBEE_START);
    Xbee_Put_1_Byte(length >> 8);
    PC_serial.printf("%.2x", length >> 8);
    Xbee_Put_1_Byte(length & 0xFF);
    PC_serial.printf("%.2x", length);
    Xbee_Put_Uint8(FRAME_TYPE_TX, *cs);
    PC_serial.printf("%.2x", FRAME_TYPE_TX);
    Xbee_Put_Uint8(FRAME_ID, *cs);
    PC_serial.printf("%.2x", FRAME_ID);
    Xbee_Put_1_Byte_By_Address(&(ac->address[0]), *cs);
    PC_serial.printf("%.2x", ac->address[0]);
    Xbee_Put_1_Byte_By_Address(&(ac->address[1]), *cs);
    PC_serial.printf("%.2x", ac->address[1]);
    Xbee_Put_1_Byte_By_Address(&(ac->address[2]), *cs);
    PC_serial.printf("%.2x", ac->address[2]);
    Xbee_Put_1_Byte_By_Address(&(ac->address[3]), *cs);
    PC_serial.printf("%.2x", ac->address[3]);
    Xbee_Put_1_Byte_By_Address(&(ac->address[4]), *cs);
    PC_serial.printf("%.2x", ac->address[4]);
    Xbee_Put_1_Byte_By_Address(&(ac->address[5]), *cs);
    PC_serial.printf("%.2x", ac->address[5]);
    Xbee_Put_1_Byte_By_Address(&(ac->address[6]), *cs);
    PC_serial.printf("%.2x", ac->address[6]);
    Xbee_Put_1_Byte_By_Address(&(ac->address[7]), *cs);
    PC_serial.printf("%.2x", ac->address[7]);
    Xbee_Put_Uint8(DESITINATION_NET_WOEK_ADDRESS_MSB, *cs);
    PC_serial.printf("%.2x", DESITINATION_NET_WOEK_ADDRESS_MSB);
    Xbee_Put_Uint8(DESITINATION_NET_WOEK_ADDRESS_LSB, *cs);
    PC_serial.printf("%.2x", DESITINATION_NET_WOEK_ADDRESS_LSB);
    Xbee_Put_Uint8(BROADCAST_RADIUS, *cs);
    PC_serial.printf("%.2x", BROADCAST_RADIUS);
    Xbee_Put_Uint8(TX_OPTIONS, *cs);
    PC_serial.printf("%.2x", TX_OPTIONS);
    Xbee_Put_Uint8(ac->ac_id, *cs);
    PC_serial.printf("%.2x", ac->ac_id);
    Xbee_Put_Uint8(message_id, *cs);
    PC_serial.printf("%.2x", message_id);
}

void Xbee_Put_Tailer(uint8_t cs)
{
    uint8_t t_cs = 0xFF - cs;
    Xbee_Put_1_Byte(t_cs);
    PC_serial.printf("%.2x", t_cs);
}

struct XBEE_UART xbee_uart;

void Xbee_Rx_Handler()
{
    //PC_serial.printf("ENTER RX_HANDLER\r\n");
    __disable_irq();//disable interupt when receiving data from XBEE_UART
    xbee_uart.buf[xbee_uart.in++] = xbee_uart.XBEE_serial->getc();
    xbee_uart.in &= (MAX_LENGTH-1);
    __enable_irq();
   //PC_serial.printf("EXIT RX_HANDLER\r\n");
   //PC_serial.printf("XBEE GOT SOMETHING\r\n");
   //XBEE_led = !XBEE_led;
}
void init_XBEE()
{
    memset(&xbee_uart, 0, sizeof(struct XBEE_UART));
    xbee_uart.XBEE_serial = &XBEE_serial; //attach the serial port to this struct
    XBEE_serial.attach(&Xbee_Rx_Handler); //attach the rx handler to the serial port
}

void Xbee_Parse_API(struct XBEE_UART* xbee_uart)
{
    if(xbee_uart->in != xbee_uart->out) //got some data
    {
        unsigned char cc = xbee_uart->buf[xbee_uart->out++];
        xbee_uart->out &= (MAX_LENGTH - 1);
        
        switch(xbee_uart->state)
        {
            case 0: //wait for xbee API START 0x7e
            {
                xbee_uart->xbee_cs = 0;
                xbee_uart->CheckOk = false;
                xbee_uart->InfoOk = false;
                if(cc == XBEE_START)
                {
                    xbee_uart->state++; //enter next state
                }
                else
                {
                    //PC_serial.printf("ERROR\r\n");
                }
                break;
            }
            case 1: //wait for xbee API LENGTH MSB
            {
                xbee_uart->length = cc << 8;
                xbee_uart->state++;
                break;
            }
            case 2: //wait for xbee API LENGTH LSB
            {
                xbee_uart->length |= cc;
                xbee_uart->state++;
                xbee_uart->payload_length = 0;
                xbee_uart->xbee_cs = 0;
                break;
            }
            case 3: //wait for xbee API payload, 1+8+2+1 + ac_id + msg_id + PPRZ_payload
            {
                xbee_uart->data[xbee_uart->payload_length] = cc;
                xbee_uart->payload_length++;
                xbee_uart->xbee_cs += cc;
                if(xbee_uart->payload_length >= xbee_uart->length)
                {
                    xbee_uart->state++;
                }
                break;
            }
            case 4: //check the checksum
            {
                if(cc + xbee_uart->xbee_cs == 0xFF) //checksum ok
                {
                    xbee_uart->CheckOk = true;
                    xbee_uart->InfoOk = true;
                    xbee_uart->state = 0;
                }
                break;
            }
            default:
            {
                xbee_uart->state = 0;
                break;
            }
        }
    }
    return;
}

void Xbee_Parse_Message(struct XBEE_UART* xbee_uart)
{
    switch(xbee_uart->data[0])
    {
        case FRAME_TYPE_RX:
        {
            if(xbee_uart->InfoOk == true)
            {
                xbee_uart->InfoOk = false;
                uint8_t ac_id = xbee_uart->data[RX_OFFSET];
                uint8_t msg_id = xbee_uart->data[RX_OFFSET + 1];
                PC_serial.printf("ac_id=%d, msg_id=%d \r\n", ac_id, msg_id);
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return;
}

void test_XBEE()
{
    Xbee_Parse_API(&xbee_uart);
    Xbee_Parse_Message(&xbee_uart);
}
#include "handler.h"

void reverseByte(BYTE* byte)
{
    BYTE temp = 0x00;
    if(*byte & 0x01) temp |= 0x80;
    if(*byte & 0x02) temp |= 0x40;
    if(*byte & 0x04) temp |= 0x20;
    if(*byte & 0x08) temp |= 0x10;
    if(*byte & 0x10) temp |= 0x08;
    if(*byte & 0x20) temp |= 0x04;
    if(*byte & 0x40) temp |= 0x02;
    if(*byte & 0x80) temp |= 0x01;

    *byte = temp;

}

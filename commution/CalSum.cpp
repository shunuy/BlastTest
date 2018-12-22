#include "stdafx.h"

unsigned char CalSum(unsigned char*puchMsg,unsigned short usDataLen)
{
    unsigned char uchLRC=0 ;
    unsigned short uIndex =0;
    
    while(usDataLen--)
    {
        uchLRC+=*puchMsg++;
    }
    return uchLRC;
}
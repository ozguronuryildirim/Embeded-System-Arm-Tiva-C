/*
 * lcd.c
 *
 *  Created on: 9 Kas 2022
 *      Author: onur_
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/gpio.c"
#include "inc/hw_gpio.h"
#include "lcd.h"




void lcdcmd(unsigned char c)
{

    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0xf0) );
    GPIOPinWrite(LCDPORT, RS, 0x00);
    GPIOPinWrite(LCDPORT, E, 0x02);
    SysCtlDelay(10);
    GPIOPinWrite(LCDPORT, E, 0x00);
    SysCtlDelay(10000);
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0x0f) << 4 );
    GPIOPinWrite(LCDPORT, RS, 0x00);
    GPIOPinWrite(LCDPORT, E, 0x02);
    SysCtlDelay(10);
    GPIOPinWrite(LCDPORT, E, 0x00);
    SysCtlDelay(10000);

}


void lcdinit(unsigned char DL, unsigned char N, unsigned char F)
{
    //DL 4/8
    //N 1=1 satır, 2=2 satır
    //F 7 => 5x7, 10 => 5x10

     SysCtlPeripheralEnable(LCDPORTENABLE);
     GPIOPinTypeGPIOOutput(LCDPORT, 0xFF);
     SysCtlDelay(50000);
     GPIOPinWrite(LCDPORT, RS,  0x00 );
     GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
     GPIOPinWrite(LCDPORT, E, 0x02);
     SysCtlDelay(10);
     GPIOPinWrite(LCDPORT, E, 0x00);
     SysCtlDelay(50000);
     GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
     GPIOPinWrite(LCDPORT, E, 0x02);
     SysCtlDelay(10);
     GPIOPinWrite(LCDPORT, E, 0x00);
     SysCtlDelay(50000);
     GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
     GPIOPinWrite(LCDPORT, E, 0x02);
     SysCtlDelay(10);
     GPIOPinWrite(LCDPORT, E, 0x00);
     SysCtlDelay(50000);
     int DLNF = 0x20;
     if (DL==8)
     {
         DLNF = DLNF | 0x10;
     }
     if (N==1)
     {
         DLNF = DLNF | 0x08;
     }
     if (F==10)
     {
         DLNF = DLNF | 0x02;
     }
     GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  DLNF );
     GPIOPinWrite(LCDPORT, E, 0x02);
     SysCtlDelay(10);
     GPIOPinWrite(LCDPORT, E, 0x00);
     SysCtlDelay(50000);
     lcdcmd(0x0F);
     lcdclr();
}


void lcdchar(unsigned char d)
{

     GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0xf0) );
     GPIOPinWrite(LCDPORT, RS, 0x01);
     GPIOPinWrite(LCDPORT, E, 0x02);
     SysCtlDelay(10);
     GPIOPinWrite(LCDPORT, E, 0x00);
     SysCtlDelay(10000);
     GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0x0f) << 4 );
     GPIOPinWrite(LCDPORT, RS, 0x01);
     GPIOPinWrite(LCDPORT, E, 0x02);
     SysCtlDelay(10);
     GPIOPinWrite(LCDPORT, E, 0x00);
     SysCtlDelay(10000);

}

void lcdclr(void)
{
    lcdcmd(0x01);
}

void lcdgo(char x, char y)
{

    if (y==0) {
        lcdcmd(0x80 + (x % 16));
        return;
    }
    lcdcmd(0xC0 + (x % 16));

}

void lcdnum(int e)
{

    switch(e)
    {
    case 0:
                lcdchar('0');
                break;
    case 1:
                lcdchar('1');
                break;
    case 2:
                lcdchar('2');
                break;
    case 3:
                lcdchar('3');
                break;
    case 4:
                lcdchar('4');
                break;
    case 5:
                lcdchar('5');
                break;
    case 6:
                lcdchar('6');
                break;
    case 7:
                lcdchar('7');
                break;
    case 8:
                lcdchar('8');
                break;
    case 9:
                lcdchar('9');
                break;
    }

}










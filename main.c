#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.c"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "inc/hw_uart.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "lcd.h"

    int sa_h = 1;
    int sa_l = 2;
    int dk_h = 4;
    int dk_l = 0;
    int sn_h = 1;
    int sn_l = 0;
    uint32_t gelendeger[4];
    long ort;
    int temp;
    int temp_h;
    int temp_l;
    int i;
    int x;
    char uartveri;
    char uarttext[24];
    int uarttime[24];

void timerkesmefonk(void){
    ADCProcessorTrigger(ADC0_BASE, 2);
    if(sn_l < 9)
    {
        sn_l = sn_l + 1;
    }
    else if(sn_h < 5)
    {
        sn_h = sn_h + 1;
        sn_l = 0;
    }
    else if (dk_l < 9)
    {

        sn_h = 0;
        sn_l = 0;
        dk_l = dk_l + 1;
    }
    else if (dk_h < 5)
    {
        dk_h = dk_h + 1;
        sn_h = 0;
        sn_l = 0;
        dk_l = 0;
    }
    else if (sa_l < 9)
    {
        dk_h = 0;
        dk_l = 0;
        sn_h = 0;
        sn_l = 0;
        if(sa_h == 2)
        {
                if (sa_l < 3)
                {
                   sa_l = sa_l + 1;
                }
                else
                {
                    sa_h = 0;
                    sa_l = 0;
                }
        }
        else
            {
            sa_l = sa_l + 1;
            }
    }
    else if (sa_h < 2)
    {
            sa_h = sa_h + 1;
            dk_h = 0;
            dk_l = 0;
            sn_h = 0;
            sn_l = 0;
            sa_l = 0;
    }
    while(!ADCIntStatus(ADC0_BASE, 2, false)){}
    ADCSequenceDataGet(ADC0_BASE, 2, gelendeger);
    ort=(gelendeger[0]+gelendeger[1]+gelendeger[2]+gelendeger[3])/4;
    temp=((4096*ort*0.70)/4096)/10;
    temp_l = temp % 10;
    temp_h = (temp - temp_l)/10;
    ADCIntClear(ADC0_BASE, 2);
    UARTCharPut(UART0_BASE, 'T');
    UARTCharPut(UART0_BASE, temp_h + 48);
    UARTCharPut(UART0_BASE, temp_l + 48);
//    UARTCharPut(UART0_BASE, 15);
    lcdgo(0,0);
    lcdnum(sa_h);
    lcdnum(sa_l);
    lcdchar(':');
    lcdnum(dk_h);
    lcdnum(dk_l);
    lcdchar(':');
    lcdnum(sn_h);
    lcdnum(sn_l);
    lcdgo(0,1);
    lcdchar('T');
    lcdchar('E');
    lcdchar('M');
    lcdchar('P');
    lcdchar(':');
    lcdnum(temp_h);
    lcdnum(temp_l);
    lcdchar(0xDF);
    lcdchar('C');
   TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}
int main(void)
{
    lcdinit(4,2,7);
    lcdclr();
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); // 40mhz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, 14);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0|GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    UARTEnable(UART0_BASE);
    ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);// seq 2 kullanilacak
    ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 3, ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 2);
    IntMasterEnable();
    IntEnable(INT_TIMER0A);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A,SysCtlClockGet()-1);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_A, timerkesmefonk);
    TimerEnable(TIMER0_BASE, TIMER_A);

    while(1)
    {
        if (UARTCharsAvail(UART0_BASE))
        {
            TimerDisable(TIMER0_BASE, TIMER_A);
            uartveri = UARTCharGet(UART0_BASE);
            if (uartveri == '$')
            {
                while (i < 8)
                {
                    while(!UARTCharsAvail(UART0_BASE));
                    uartveri = UARTCharGet(UART0_BASE);
                    uarttime[i] = uartveri - 48;
                    i++;
                }
                if(uarttime[1] != 0)
                {
                    sa_h = uarttime[0];
                    sa_l = uarttime[1];
                    dk_h = uarttime[3];
                    dk_l = uarttime[4];
                    sn_h = uarttime[6];
                    sn_l = uarttime[7];
                    uarttime[0] = 0;
                    uarttime[1] = 0;
                    uarttime[3] = 0;
                    uarttime[4] = 0;
                    uarttime[6] = 0;
                    uarttime[7] = 0;
                }
            }
            else if (uartveri == '#')
            {
                while (i < 6)
                {
                    while(!UARTCharsAvail(UART0_BASE));
                    uartveri = UARTCharGet(UART0_BASE);
                    uarttext[i] = uartveri;
                    i++;
                }
                for (x = 0; x < 6; x++)
                {
                    lcdgo(10 + x , 1);
                    lcdchar(uarttext[x]);
                    if (uarttext[x + 1] == '#')
                        {
                            break;
                        }
                }
            }
            else if (uartveri == '&')
            {
                while(UARTBusy(UART0_BASE));
                UARTCharPut(UART0_BASE, '&');
                UARTCharPut(UART0_BASE, sn_l);
                UARTCharPut(UART0_BASE, sn_h);
                UARTCharPut(UART0_BASE, dk_l);
                UARTCharPut(UART0_BASE, dk_h);
                UARTCharPut(UART0_BASE, sa_l);
                UARTCharPut(UART0_BASE, sa_h);
            }
            i = 0;
            TimerEnable(TIMER0_BASE, TIMER_A);
        }
    }
}

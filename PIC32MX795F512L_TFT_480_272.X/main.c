// PIC32MX795F512L Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <plib.h>

#include "delay.h"
#include "display_ssd1963.h"
#include "color.h"

#pragma config FNOSC     = PRIPLL            // Oscillator Selection
#pragma config FPLLIDIV  = DIV_2             // PLL Input Divider (PIC32 Starter Kit: use divide by 2 only)
#pragma config FPLLMUL   = MUL_20            // PLL Multiplier
#pragma config FPLLODIV  = DIV_1             // PLL Output Divider
#pragma config FPBDIV    = DIV_1             // Peripheral Clock divisor
#pragma config FWDTEN    = OFF               // Watchdog Timer
#pragma config WDTPS     = PS32768           // Watchdog Timer Postscale
#pragma config FCKSM     = CSDCMD            // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC  = OFF               // CLKO Enable
#pragma config POSCMOD   = HS                // Primary Oscillator
#pragma config IESO      = OFF               // Internal/External Switch-over
#pragma config FSOSCEN   = OFF               // Secondary Oscillator Enable
#pragma config CP        = OFF               // Code Protect
#pragma config BWP       = OFF               // Boot Flash Write Protect
#pragma config PWP       = OFF               // Program Flash Write Protect
#pragma config ICESEL    = ICS_PGx2          // ICE/ICD Comm Channel Select
#pragma config DEBUG     = OFF               // Debugger Disabled for Starter Kit
#pragma config FSRSSEL   = PRIORITY_7

#define SystemFrequency		80000000L

#define VERSION "1.0"

int main()
{
    DDPCONbits.JTAGEN = 0;
    
    SYSTEMConfig( SystemFrequency , SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE );

    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    TRISF = 0;
    TRISG = 0;

    LATA = 0;
    LATB = 0;
    LATC = 0;
    LATD = 0;
    LATE = 0;
    LATF = 0;
    LATG = 0;

    TFT_Init_SSD1963_8bit(480,272);

    INTEnable(INT_SOURCE_DMA(DMA_CHANNEL0), INT_ENABLED);
    INTEnableSystemMultiVectoredInt( );

    TFT_Fill(White);

    while(1)
    {
        TFT_Fill(Blue);

        msleep(1000);

        TFT_Fill(White);

        msleep(1000);
    }
    
    return 0;
}
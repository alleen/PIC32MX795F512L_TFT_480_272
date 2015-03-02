#include <xc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <plib.h>

#define CCLK   (80000000L)    // system clock
#define PBCLK   (CCLK / 2)    // peripheral bus clock
#define SAMPLE_RATE       5000
#define CCLK_US  (PBCLK/1000000L)   //used for microsecond delay

void usleep (unsigned int t){
      WriteCoreTimer(0);
      while(ReadCoreTimer() < t*CCLK_US);
}

void msleep(unsigned long long t)
{
    unsigned int i;

    for(i=0; i<t; i++)
        usleep(1000);
}

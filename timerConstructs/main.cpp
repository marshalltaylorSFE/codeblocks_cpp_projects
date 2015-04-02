//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  March 21, 2015
//
//**********************************************************************//

//HOW TO OPERATE
//  Make TimerClass objects for each thing that needs periodic service
//  pass the interval of the period in ticks
//  Set MAXINTERVAL to the max foreseen interval of any TimerClass
//  Set MAXTIMER to overflow number in the header.  MAXTIMER + MAXINTERVAL
//    cannot exceed variable size.

//Includes
#include "timerModule.h"
#include "stdio.h"
#include "stdint.h"

//Globals
TimerClass msTimerA( 500 );
uint16_t msTicks = 0;

#define MAXINTERVAL 2000


int main( void )
{
    //Simulates ISR
    //In arduino land, should be able to call TimerClass::flagStatus() whenever, will return overflow.
    for( uint64_t i = 0; i < 22000; i = i + 1 )
    {
        uint32_t returnVar = 0;
        if(msTicks >= ( MAXTIMER + MAXINTERVAL ))
        {
            returnVar = msTicks - MAXTIMER;

        }
        else
        {
            returnVar = msTicks + 1;
        }
        msTicks = returnVar;


        msTimerA.update(msTicks);

        if(msTimerA.flagStatus() == PENDING)
        {
            printf("msTimerA has expired.  Internal ticks: %d\n", msTicks);
        }

    }

    printf("Unit test has somehow exited the loop!  Program end.\n");

}

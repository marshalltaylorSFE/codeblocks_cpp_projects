//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Beer target:  Marshall Taylor
//  Debited:  March 21, 2015
//
//**********************************************************************//

//Includes
#include "effect_bendvelope.h"
#include "stdio.h"
#include "conio.h"
#include <windows.h>
#include "stdint.h"


// for use of the bitmap_image library
//#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include "graphWrapper.h"
#include "timeKeeper32.h"
#include "timerModule32.h"

using namespace std;
#define ENVTICKRATEMS 5

//Globals
uint32_t MAXTIMER = 60000000;
uint32_t MAXINTERVAL = 2000000;

void gotoxy ( int column, int line );
uint8_t noQuit = 1;
//Globals

AudioEffectBendvelope bendvelope;
GraphWrapper outputGraph;

int main( void )
{

    TimerClass32 envTimer( 1000 );
    TimerClass32 debugTimer( 1000000 );

	//Setting envelope
	bendvelope.attack( 200, 0 );// 0 to 255 for length, -128 to 127
	bendvelope.decay( 255, 100 );// 0 to 255 for length, -128 to 127
	bendvelope.sustain( 150 );// 0 to 255 for level
	bendvelope.release( 30, 0 );// 0 to 255 for length, -128 to 127
	bendvelope.setAttackHold( 10 );

    long usTicks = 0;
    long lastService = 0;

    noQuit = 1;

    //Make the delays
    TimeKeeper32 testTK;

    outputGraph.open();

    GridParams grid1 = outputGraph.makeGrid(6,6,1011,261,10,16);
    GridParams grid2 = outputGraph.makeGrid(6,278,1011,374,10,10);
    GridParams grid3 = outputGraph.makeGrid(6,390,1011,645,10,16);
    GridParams grid4 = outputGraph.makeGrid(6,663,1011,759,10,10);

    grid1.setColor(255,0,0);
    grid2.setColor(0,127,0);
    grid3.setColor(200,20,200);
    grid4.setColor(255,127,0);

    while( usTicks < 2000000 )
    {
        if( usTicks == 10000 )
        {
            bendvelope.noteOn();
        }
        if( usTicks == 600000 )
        {
            bendvelope.noteOff();
        }
        if( usTicks == 650000 )
        {
            bendvelope.noteOn();
        }
       	envTimer.update(usTicks);
        debugTimer.update(usTicks);

        if(envTimer.flagStatus() == PENDING)
        {
            bendvelope.tick(1000);

            outputGraph.plotPoint((usTicks / 1000 / 2),bendvelope.amp, grid1);
            //outputGraph.plotPoint((usTicks / 1000 / 2),bendvelope.positionLHS, grid1);
            outputGraph.plotPoint((usTicks / 1000 / 2),bendvelope.noteState * 50, grid2);
            outputGraph.plotPoint((usTicks / 1000 / 2),bendvelope.state * 75, grid4);

        }
        if(debugTimer.flagStatus() == PENDING)
        {
        }
        //Print out test table
        int32_t tempSample;
        if(usTicks < 256)
        {
            tempSample = bendvelope.testTable.getSample(usTicks);
            outputGraph.plotPoint((usTicks),tempSample, grid3);
        }
        //Print out test table
        if((usTicks < 512)&&(usTicks >= 256 ))
        {
            tempSample = bendvelope.attackTable.getSample(usTicks - 256);
            outputGraph.plotPoint((usTicks),tempSample, grid3);
        }
        //Print out test table
        if((usTicks < 768)&&(usTicks >= 512 ))
        {
            tempSample = bendvelope.decayTable.getSample(usTicks - 512);
            outputGraph.plotPoint((usTicks),tempSample, grid3);
        }
        //Print out test table
        if((usTicks < 1024)&&(usTicks >= 768 ))
        {
            tempSample = bendvelope.releaseTable.getSample(usTicks - 768);
            outputGraph.plotPoint((usTicks),tempSample, grid3);
        }
        usTicks++;

    }

    outputGraph.close();


}
void gotoxy ( int column, int line )
{
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(
        GetStdHandle( STD_OUTPUT_HANDLE ),
        coord
    );
}

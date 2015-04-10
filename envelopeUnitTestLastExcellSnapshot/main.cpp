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
#include "envelope.h"
#include "stdio.h"
#include "conio.h"
#include <windows.h>
#include "stdint.h"
// basic file operations
#include <iostream>
#include <fstream>
using namespace std;
#define ENVTICKRATEMS 10

void gotoxy ( int column, int line );
uint8_t noQuit = 1;
//Globals

Envelope myEnvelope;

int main( void )
{

    myEnvelope.setSustain( 200 );
    myEnvelope.setAttack( 255, -80 );
    myEnvelope.setDecay( 255, -20 );
    myEnvelope.setRelease( 150, -20 );

    long msTicks = 0;
    long lastService = 0;

    //Help
    gotoxy(0,0);
    printf("Press 1 to trigger on, press space to trigger off.\n");
    printf("\nSeconds: \nAmplitude:");

    while(noQuit)
    {

        msTicks = msTicks + ENVTICKRATEMS;
        Sleep(ENVTICKRATEMS);
        //This is made to tick every 10 ms
        if( msTicks > (lastService + ENVTICKRATEMS))
        {
            lastService = lastService + ENVTICKRATEMS;
            myEnvelope.tick( ENVTICKRATEMS );

            //printf("%d", msTicks);
            //printf(", %d\n", myEnvelope.amp);
        }

//        //Present the output here
        gotoxy( 11, 2 );
        printf("%d", (msTicks / 1000));
        gotoxy(11, 3);
        printf("%03d", myEnvelope.amp);  //myEnvelope.amp is the output, 0 to 127



        //This section checks and operates the gate function
        if(_kbhit() == 1)
        {
            char inChar;
            inChar = _getch();
            if( inChar == '1')
            {
                myEnvelope.setNoteOn();
            }
            if( inChar == ' ')
            {
                myEnvelope.setNoteOff();
            }
            if( inChar == 't')
            {
                myEnvelope.tick(10);
            }
            if( inChar == 'q')
            {
                noQuit = 0;
            }
        }

    }
    //Has quit.  Now write a file
    msTicks = 0;
    lastService = 0;
    noQuit = 1;
    //Make the delays
    TimeKeeper testTK;

    FILE * pFile;

    pFile = fopen ("example.txt","w");
    uint8_t noteOnServiced = 0;
    uint8_t noteOnServiced2 = 0;
    uint8_t noteOffServiced = 0;
    uint8_t noteOffServiced2 = 0;
    uint8_t noteOnShadowServiced = 0;
    uint16_t lastTestTK = 0;
    uint8_t lastNoteState = 0;

    while(noQuit)
    {

        msTicks = msTicks + ENVTICKRATEMS;
        testTK.mIncrement( ENVTICKRATEMS );
        //Sleep(ENVTICKRATEMS);
        //This is made to tick every 10 ms
        if( msTicks > (lastService + ENVTICKRATEMS))
        {
            lastService = lastService + ENVTICKRATEMS;
            myEnvelope.tick( ENVTICKRATEMS );

            //printf("%d", msTicks);
            //printf(", %d\n", myEnvelope.amp);
        }

//        if( testTK.mGet() > (lastTestTK + 200))
//        {
//            lastTestTK += 200;
//            if( lastNoteState == 0)
//            {
//                myEnvelope.setNoteOn();
//                lastNoteState = 1;
//            }
//            else
//            {
//                myEnvelope.setNoteOff();
//                lastNoteState = 0;
//            }
//
//        }
        //If greater than... note on
        if( (noteOnServiced == 0) && (testTK.mGet() > 100 ) )
        {
            myEnvelope.setNoteOn();
            noteOnServiced = 1;
        }
        //If greater than... note off
        if( (noteOffServiced == 0) && (testTK.mGet() > 500 ) )
        {
            //myEnvelope.setNoteOff();
            noteOffServiced = 1;
        }
        //If greater than... note on
        if( (noteOnServiced2 == 0) && (testTK.mGet() > 1000 ) )
        {
            //myEnvelope.setNoteOn();
            noteOnServiced2 = 1;
        }
        //If greater than... note off
        if( (noteOffServiced2 == 0) && (testTK.mGet() > 3000 ) )
        {
            myEnvelope.setNoteOff();
            noteOffServiced2 = 1;
        }        //If greater than... note on
        if( (noteOnShadowServiced == 0) && (testTK.mGet() > 7000 ) )
        {
            myEnvelope.setNoteOn();
            noteOnShadowServiced = 1;
        }
        //If greater than... quit
        if( testTK.mGet() > 10000 )
        {
            noQuit = 0;
        }
//        //Present the output here

        fprintf(pFile, "%d\t", (msTicks));
        fprintf(pFile, "%03d\t", myEnvelope.amp);  //myEnvelope.amp is the output, 0 to 127
        fprintf(pFile, "%03d\n", myEnvelope.shadowAmp);  //myEnvelope.amp is the output, 0 to 127



    }
    fclose (pFile);



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

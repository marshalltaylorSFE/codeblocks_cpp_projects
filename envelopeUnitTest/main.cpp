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

#define ENVTICKRATEMS 10

void gotoxy ( int column, int line );
uint8_t noQuit = 1;
//Globals

Envelope myEnvelope;

int main( void )
{
    myEnvelope.setSustain( 127 );
    myEnvelope.setAttack( 255, -60 );
    myEnvelope.setDecay( 255, 60 );
    myEnvelope.setRelease( 255, 60 );

    long msTicks = 0;
    long lastService = 0;

    //Help
    gotoxy(0,0);
    printf("Press 1 to trigger on, press space to trigger off.\n");
    printf("\nSeconds: \nAmplitude:");

    while(noQuit)
    {

        msTicks = msTicks + 10;
        Sleep(10);
        //This is made to tick every 10 ms
        if( msTicks > (lastService + 10))
        {
            lastService = lastService + 10;
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

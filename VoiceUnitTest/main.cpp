//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Beer target:  Marshall Taylor
//  Debited:  July 9, 2015
//
//**********************************************************************//

//C++ Includes
#include "stdio.h"
#include "conio.h"
#include <windows.h>
#include "stdint.h"

//void gotoxy ( int column, int line );

//SynthVoice Includes
#include "SynthVoice.h"

//Globals


int main( void )
{
    SynthVoice mySynthVoiceA;
    SynthVoice mySynthVoiceB;
    mySynthVoiceA.init();
    mySynthVoiceA.buildTable();
    mySynthVoiceB.init();
    mySynthVoiceB.buildTable();
    mySynthVoiceA.setFreq(80);
    mySynthVoiceB.setFreq(85);

    for( int i = 0; i < 200; i++)
    {
        printf("%d,%d\n", i, ((mySynthVoiceA.tick()+mySynthVoiceB.tick())/2));
    }
    mySynthVoiceA.setFreq(100);
    mySynthVoiceB.setFreq(89);

    for( int i = 200; i < 300; i++)
    {
        printf("%d,%d\n", i, ((mySynthVoiceA.tick()+mySynthVoiceB.tick())/2));
    }


//    myEnvelope.setAttack(1);
//    myEnvelope.setDecay(1);
//    myEnvelope.setSustain(64);
//    myEnvelope.setRelease(1);
//    long msTicks = 0;
//    long lastService = 0;
//
//    while(1)
//    {
//
//        msTicks = msTicks + 1;
//        Sleep(1);
//        if( msTicks > (lastService + 10))
//        {
//            lastService = msTicks - 10;
//            myEnvelope.tick();
//        }
//        gotoxy( 5, 4 );
//        printf("%d", (msTicks / 1000));
//        gotoxy(5, 5);
//        printf("%03d", myEnvelope.amp);
//
//        if(_kbhit() == 1)
//        {
//            char inChar;
//            inChar = _getch();
//            if( inChar == '1')
//            {
//                myEnvelope.setNoteOn();
//            }
//            if( inChar == '0')
//            {
//                myEnvelope.setNoteOff();
//            }
//            if( inChar == 't')
//            {
//                myEnvelope.tick();
//            }
//        }
//
//    }
//

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

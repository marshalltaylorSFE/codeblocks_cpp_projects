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

// for use of the bitmap_image library
//#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include "bitmap_image.hpp"

using namespace std;
#define ENVTICKRATEMS 5

void gotoxy ( int column, int line );
uint8_t noQuit = 1;
//Globals

Envelope myEnvelope;
Envelope myEnvelope2;

int main( void )
{

    myEnvelope.setSustain( 110 );
    myEnvelope.setAttack( 50, 100 );
    myEnvelope.setDecay( 255, 60 );
    myEnvelope.setRelease( 150, -80 );
    myEnvelope.setAttackHold( 255 );

    myEnvelope2.setSustain( 160 );
    myEnvelope2.setAttack( 200, 100 );
    myEnvelope2.setDecay( 255, 60 );
    myEnvelope2.setRelease( 150, -35 );
    myEnvelope2.setAttackHold( 255 );
    long msTicks = 0;
    long lastService = 0;



    //Has quit.  Now write a file
    msTicks = 0;
    lastService = 0;
    noQuit = 1;
    //Make the delays
    TimeKeeper testTK;


    uint8_t noteOnServiced = 0;
    uint8_t noteOnServiced2 = 0;
    uint8_t noteOffServiced = 0;
    uint8_t noteOffServiced2 = 0;
    uint8_t noteOnShadowServiced = 0;
    uint16_t lastTestTK = 0;
    uint8_t lastNoteState = 0;

   // make_bitmap_1();
   bitmap_image image(1024,768);

   image_drawer draw(image);

   image.set_region(3,3,1018,762,bitmap_image::red_plane,255);
   image.set_region(3,3,1018,762,bitmap_image::blue_plane,255);
   image.set_region(3,3,1018,762,bitmap_image::green_plane,255);

   uint16_t origin[2] = {6,261};
   uint16_t maxBox[2] = {1011,255};
   uint16_t lastXY[2] = {6,261};
   uint16_t lastXY2[2] = {6,261};

   uint16_t origin2[2] = {6,374};
   uint16_t maxBox2[2] = {1011,96};
   uint16_t lastXY3[2] = {6,374};

   uint16_t Box2origin[2] = {6, 645};
   uint16_t Box2maxBox[2] = {1011,255};
   uint16_t Box2lastXY[2] = {6,645};
   uint16_t Box2lastXY2[2] = {6,645};

   uint16_t Box2origin2[2] = {6,759};
   uint16_t Box2maxBox2[2] = {1011,96};
   uint16_t Box2lastXY3[2] = {6,759};
   uint16_t Box2lastXY4[2] = {6,759};

   //Grid prep
   draw.pen_color(200,200,200);
   draw.pen_width(1);

   //First Grid
   int grid;
   for(grid = 0; grid < maxBox[0]; grid = grid + 10)
   {
       draw.vertical_line_segment(origin[1],origin[1] - maxBox[1],origin[0]+grid);
   }
   for(grid = 0; grid < maxBox[1]; grid = grid + 16)
   {
       draw.horiztonal_line_segment(origin[0],origin[0]+maxBox[0],origin[1]-grid);
   }

   uint16_t mX1 = origin[0];
   uint16_t mY1 = origin[1];
   uint16_t mX2 = origin[0] + maxBox[0];
   uint16_t mY2 = origin[1] - maxBox[1];
   draw.rectangle(mX1,mY1,mX2,mY2);

   //Second Grid
   for(grid = 0; grid < maxBox2[0]; grid = grid + 10)
   {
       draw.vertical_line_segment(origin2[1],origin2[1] - maxBox2[1],origin2[0]+grid);
   }
   for(grid = 0; grid < maxBox2[1]; grid = grid + 16)
   {
       draw.horiztonal_line_segment(origin2[0],origin2[0]+maxBox2[0],origin2[1]-grid);
   }

   mX1 = origin2[0];
   mY1 = origin2[1];
   mX2 = origin2[0] + maxBox2[0];
   mY2 = origin2[1] - maxBox2[1];
   draw.rectangle(mX1,mY1,mX2,mY2);


   //Third Grid
   for(grid = 0; grid < Box2maxBox[0]; grid = grid + 10)
   {
       draw.vertical_line_segment(Box2origin[1],Box2origin[1] - Box2maxBox[1],Box2origin[0]+grid);
   }
   for(grid = 0; grid < Box2maxBox[1]; grid = grid + 16)
   {
       draw.horiztonal_line_segment(Box2origin[0],Box2origin[0]+Box2maxBox[0],Box2origin[1]-grid);
   }

   mX1 = Box2origin[0];
   mY1 = Box2origin[1];
   mX2 = Box2origin[0] + Box2maxBox[0];
   mY2 = Box2origin[1] - Box2maxBox[1];
   draw.rectangle(mX1,mY1,mX2,mY2);

   //Fourth Grid
   for(grid = 0; grid < Box2maxBox2[0]; grid = grid + 10)
   {
       draw.vertical_line_segment(Box2origin2[1],Box2origin2[1] - Box2maxBox2[1],Box2origin2[0]+grid);
   }
   for(grid = 0; grid < Box2maxBox2[1]; grid = grid + 16)
   {
       draw.horiztonal_line_segment(Box2origin2[0],Box2origin2[0]+Box2maxBox2[0],Box2origin2[1]-grid);
   }

   mX1 = Box2origin2[0];
   mY1 = Box2origin2[1];
   mX2 = Box2origin2[0] + Box2maxBox2[0];
   mY2 = Box2origin2[1] - Box2maxBox2[1];
   draw.rectangle(mX1,mY1,mX2,mY2);




    //Help
    gotoxy(0,0);
    printf("Writing output files.\n");


    draw.pen_color(0,0,0);
    draw.pen_width(2);

    //Format:
    //[time]
    //[event]
    //[serviced flag]
    uint16_t events[20][3];
    uint8_t eventPointer = 0;
    events[0][0] = 100;
    events[0][1] = 1;
    events[0][2] = 0;

    events[1][0] = 3000;
    events[1][1] = 0;
    events[1][2] = 0;

    events[2][0] = 6000;
    events[2][1] = 1;
    events[2][2] = 0;

    events[3][0] = 6300;
    events[3][1] = 0;
    events[3][2] = 0;

    events[9][0] = 10000;
    events[9][1] = 255; //quit
    events[9][2] = 0;

    //Format:
    //[time]
    //[event]
    //[serviced flag]
    uint16_t events2[20][3];
    uint8_t event2Pointer = 0;
    events2[0][0] = 100;
    events2[0][1] = 1;
    events2[0][2] = 0;

    events2[1][0] = 1400;
    events2[1][1] = 0;
    events2[1][2] = 0;

    events2[2][0] = 1800;
    events2[2][1] = 1;
    events2[2][2] = 0;

    events2[3][0] = 2300;
    events2[3][1] = 0;
    events2[3][2] = 0;

    events2[4][0] = 4100;
    events2[4][1] = 11;
    events2[4][2] = 0;

    events2[5][0] = 6000;
    events2[5][1] = 0;
    events2[5][2] = 0;

    events2[6][0] = 6500;
    events2[6][1] = 1;
    events2[6][2] = 0;

    events2[7][0] = 6700;
    events2[7][1] = 0;
    events2[7][2] = 0;

    events2[8][0] = 7200;
    events2[8][1] = 1;
    events2[8][2] = 0;

    events2[9][0] = 7950;
    events2[9][1] = 0;
    events2[9][2] = 0;
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
            myEnvelope2.tick( ENVTICKRATEMS );

        }

        //Check serviced until clear, then move on
        if(events[eventPointer][2] == 0)
        {
            //check time
            if(testTK.mGet() > events[eventPointer][0])
            {
                //expired! needs service.
                switch( events[eventPointer][1] )
                {
                case 0:
                    myEnvelope.setNoteOff();
                    break;
                case 1:
                    myEnvelope.setNoteOn();
                    break;
                case 255:
                    noQuit = 0;
                default:
                    break;
                }
                //Now check serviced box
                events[eventPointer][2] = 1;
            }

        }
        else
        {
            //Increment point
            eventPointer++;
        }

        //Check serviced until clear, then move on
        if(events2[event2Pointer][2] == 0)
        {
            //check time
            if(testTK.mGet() > events2[event2Pointer][0])
            {
                //expired! needs service.
                switch( events2[event2Pointer][1] )
                {
                case 0:
                    myEnvelope2.setNoteOff();
                    break;
                case 1:
                    myEnvelope2.setNoteOn();
                    break;
                case 11:
                    myEnvelope2.setAttackHold( 0 );
                    myEnvelope2.setNoteOn();
                    break;
                case 255:
                    noQuit = 0;
                default:
                    break;
                }
                //Now check serviced box
                events2[event2Pointer][2] = 1;
            }

        }
        else
        {
            //Increment point
            event2Pointer++;
        }

        //Present the output here
        draw.pen_color(255,0,0);
        draw.line_segment(lastXY2[0],lastXY2[1],origin[0] + msTicks/10,origin[1]-myEnvelope.shadowAmp);
        lastXY2[0] = origin[0] + msTicks/10;
        lastXY2[1] = ((int)origin[1]-myEnvelope.shadowAmp);


        draw.pen_color(0,0,0);
        draw.line_segment(lastXY[0],lastXY[1],origin[0] + msTicks/10,origin[1]-myEnvelope.amp);
        lastXY[0] = origin[0] + msTicks/10;
        lastXY[1] = ((int)origin[1]-myEnvelope.amp);


        draw.pen_color(255,127,0);
        draw.line_segment(lastXY3[0],lastXY3[1],origin2[0] + msTicks/10,origin2[1]-(32*myEnvelope.noteState + 16));
        lastXY3[0] = origin2[0] + msTicks/10;
        lastXY3[1] = ((int)origin2[1]-(32*myEnvelope.noteState + 16));


        draw.pen_color(0,255,0);
        draw.line_segment(Box2lastXY2[0],Box2lastXY2[1],Box2origin[0] + msTicks/10,Box2origin[1]-myEnvelope2.shadowAmp);
        Box2lastXY2[0] = Box2origin[0] + msTicks/10;
        Box2lastXY2[1] = ((int)Box2origin[1]-myEnvelope2.shadowAmp);


        draw.pen_color(0,0,0);
        draw.line_segment(Box2lastXY[0],Box2lastXY[1],Box2origin[0] + msTicks/10,Box2origin[1]-myEnvelope2.amp);
        Box2lastXY[0] = Box2origin[0] + msTicks/10;
        Box2lastXY[1] = ((int)Box2origin[1]-myEnvelope2.amp);


        draw.pen_color(0,0,255);
        draw.line_segment(Box2lastXY4[0],Box2lastXY4[1],Box2origin2[0] + msTicks/10,Box2origin2[1]-((myEnvelope2.envAttackHold.timeScale*255/25/4) + 16));
        Box2lastXY4[0] = Box2origin2[0] + msTicks/10;
        Box2lastXY4[1] = ((int)Box2origin2[1]-((myEnvelope2.envAttackHold.timeScale*255/25/4) + 16));


        draw.pen_color(255,0,255);
        draw.line_segment(Box2lastXY3[0],Box2lastXY3[1],Box2origin2[0] + msTicks/10,Box2origin2[1]-(32*myEnvelope2.noteState + 16));
        Box2lastXY3[0] = Box2origin2[0] + msTicks/10;
        Box2lastXY3[1] = ((int)Box2origin2[1]-(32*myEnvelope2.noteState + 16));

    }
    image.save_image("newfile.bmp");



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

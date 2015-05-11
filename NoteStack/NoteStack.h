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
#ifndef NOTESTACK_H_INCLUDED
#define NOTESTACK_H_INCLUDED

#include "stdint.h"

//Depth into the stack variable
typedef uint8_t stackDepthVar;

#define NULLNOTE -1

//Note data type
struct stackNote
{
    int8_t noteValue;
    int8_t noteVelocity;

    //A pointer to the lower note
    stackNote * lowerStackNote;

};


//Classes
class NoteStack
{
    stackDepthVar maxDepth;
    stackDepthVar currentDepth;

    //A pointer to the upper most note
public:
    stackNote * topStackNote;
	stackNote nullNote;

public:
    NoteStack( stackDepthVar ); //Construct with passed max depth
    NoteStack( void );
    void pushNote( stackNote & ); //Pass stackNote
    stackNote popNote( void ); //returns stackNote
    stackNote peekNote( void );
    stackNote peekNote( stackDepthVar );
    void dropNote( stackDepthVar ); //Pass position, returns stackNote
    int8_t seekNote( stackNote & ); //pass stackNote, returns position
    stackDepthVar stackDepth( void ); //returns depth of stack.
    void printfNoteStack( void );


};

#endif // FILE_H_INCLUDED

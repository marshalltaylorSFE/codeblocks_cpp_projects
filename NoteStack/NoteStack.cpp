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

//Includes
#include "NoteStack.h"
#include "stdint.h"
#include "stdio.h"

//**********************************************************************//
//  First class methods (heh.)
//

//Construct with passed max depth
NoteStack::NoteStack( stackDepthVar inputDepth )
{
    topStackNote = 0;
    currentDepth = 0 ;
    maxDepth = inputDepth;
}
NoteStack::NoteStack( void )
{
    currentDepth = 0;
    topStackNote = 0;
    maxDepth = 4;
}

//Pass stackNote
void NoteStack::pushNote( stackNote & noteToPush )
{
    //This creates a new note object, but then forgets the name
    //and doesn't delete the note.
    //
    //If the stack size is too big, new notes won't be accepted
    if( currentDepth < maxDepth )
    {
        stackNote *newStackNote = new stackNote;
        currentDepth++;

        newStackNote->noteValue = noteToPush.noteValue;
        newStackNote->noteVelocity = noteToPush.noteVelocity;
        //newStackNote->lowerStackNote = topStackNote;
        newStackNote->lowerStackNote = topStackNote;
        topStackNote = newStackNote; //equals the address of the new note
    }

}

//returns stackNote
stackNote NoteStack::popNote( void )
{
    stackNote * noteToDestroy;

    if(currentDepth > 0)
    {
        noteToDestroy = topStackNote;
        //move top pointer
        topStackNote = noteToDestroy->lowerStackNote;
        //destruct topStackNote by topStack
        delete noteToDestroy;
        currentDepth--;
    }
    else
    {
        topStackNote = 0;
    }
    return *topStackNote;
}

//returns stackNote
stackNote NoteStack::peekNote( void )
{
    stackNote * noteToReturn;
    noteToReturn = topStackNote;
    return *noteToReturn;
}
//returns stackNote
stackNote NoteStack::peekNote( stackDepthVar inputDepth ) //depth 0 = top
{
    stackNote * noteToReturn;

    if(inputDepth <= maxDepth) //Prevent out of range peeks
    {
        noteToReturn = topStackNote;
        //Iterate to the depth
        for( uint8_t i = 0; i < inputDepth; i++ )
        {
            noteToReturn = noteToReturn->lowerStackNote;
        }
    }
    else
    {
        noteToReturn = 0;
    }
    return *noteToReturn;
}

//Pass position, returns stackNote
void NoteStack::dropNote( stackDepthVar depthToDrop )
{
    //Protect against invalid depths
    if( depthToDrop == 0 )
    {
        popNote();
    }
    else if( depthToDrop > currentDepth )
    {
        //invalid, do nothing
    }
    else
    {
        //Ok, good to do it.

        //pointer for note above note to drop
        stackNote * noteAbove = topStackNote;
        //pointer for note to drop
        stackNote * noteToDrop;
        //go to the depth above the one to drop
        for(uint8_t i = 0; i < (depthToDrop - 1); i++ )
        {
            noteAbove = noteAbove->lowerStackNote;
        }
        //point the note to drop
        noteToDrop = noteAbove->lowerStackNote;
        //Redirect the note above
        noteAbove->lowerStackNote = noteToDrop->lowerStackNote;//noteToDrop->lowerStackNote;
        //Drop it like it's hot
        delete noteToDrop;
        currentDepth--;
    }

}

//pass stackNote, returns position
uint8_t NoteStack::seekNote( stackNote & noteToSeek )
{
    //Create temporary note to use for comparison
    stackNote * noteVar = new stackNote;
    noteVar = topStackNote;
    int8_t returnVar = -1;

    for(uint8_t i = 0; i < currentDepth; i++)
    {
        if( noteVar->noteValue == noteToSeek.noteValue )
        {
             returnVar = i;
        }
        //Walk down into the stack
        noteVar = noteVar->lowerStackNote;
    }
    return returnVar;
}

//returns depth of stack.
stackDepthVar NoteStack::stackDepth( void )
{
    return currentDepth;
}

//printfs the stack
void NoteStack::printfNoteStack( void )
{
    stackNote * noteToPrint;
    noteToPrint = topStackNote;
    //Iterate to the depth
    for( uint8_t i = 0; i < currentDepth; i++ )
    {
        printf("Note address: %X\n", noteToPrint);
        printf("Note value: %d\n", noteToPrint->noteValue);
        printf("\n");
        noteToPrint = noteToPrint->lowerStackNote;

    }
}

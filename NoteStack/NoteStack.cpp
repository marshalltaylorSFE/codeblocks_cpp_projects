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
//  depth 0 is 0 notes.  Shall point to null
//
//
//**********************************************************************//

//Includes
#include "NoteStack.h"
#include "stdint.h"
#include "stdio.h"


//**********************************************************************//
//  NoteStack Class
//
//  Creates link list
//
//  Construct with passed max depth or use default 4
//
NoteStack::NoteStack( stackDepthVar inputDepth )
{
    topStackNote = &nullNote;  //start pointed to null note
    currentDepth = 0 ;  //Size 0
    maxDepth = inputDepth;  //set max size
    nullNote.noteValue = -1;  //define null note
	nullNote.noteVelocity = -1;  //define null note
}
NoteStack::NoteStack( void )
{
    topStackNote = &nullNote;
    currentDepth = 0;
    maxDepth = 4;
    nullNote.noteValue = -1;
	nullNote.noteVelocity = -1;

}

//Pass stackNote to be pushed to the stack (top)
void NoteStack::pushNote( stackNote & noteToPush )
{
    //This creates a new note object, but then forgets the name
    //and doesn't delete the note.
    //
    //If the stack size is too big, new notes won't be accepted
    if( currentDepth < maxDepth )
    {
		//Make a new note to the heap
        stackNote *newStackNote = new stackNote;
		//grow stack size
        currentDepth++;
		//Set the new note's params
        newStackNote->noteValue = noteToPush.noteValue;
        newStackNote->noteVelocity = noteToPush.noteVelocity;
        //Point the top of stack to the new note
        newStackNote->lowerStackNote = topStackNote;
        topStackNote = newStackNote; //equals the address of the new note
    }

}

//returns stackNote
stackNote NoteStack::popNote( void )
{
	//Target note variable
    stackNote * noteToDestroy;

    if(currentDepth > 0)//If there are any notes on the stack,
    {
        noteToDestroy = topStackNote;
        //move top pointer
        topStackNote = noteToDestroy->lowerStackNote;
        //destruct topStackNote by topStack
        delete noteToDestroy;
        currentDepth--;

        //If depth now == 0, point top to null
        if(currentDepth == 0)
        {
            topStackNote = &nullNote;
        }
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
stackNote NoteStack::peekNote( stackDepthVar inputDepth ) //position 0 = top
{
    stackNote * noteToReturn;

    if(inputDepth < maxDepth) //Prevent out of range peeks
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
        noteToReturn = &nullNote;
    }
    return *noteToReturn;
}

//Pass position, returns void
void NoteStack::dropNote( stackDepthVar depthToDrop )
{
    //Check depth
    if( depthToDrop == 0 )
    {
		//If depth is 0 and there is a note on top, pop it
		if(topStackNote != &nullNote)
		{
			popNote();
		}

    }
    else if( depthToDrop >= currentDepth )
    {
        //Too deep!  Do nothing.
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
int8_t NoteStack::seekNote( stackNote & noteToSeek )
{
    //Create temporary note to use for comparison
    stackNote * noteVar = new stackNote;
    noteVar = topStackNote;
    int8_t returnVar = -1;

    for(uint8_t i = 0; i < currentDepth; i++)
    {
		//For now, only use the notevalue
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

//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  420, 2015
//
//**********************************************************************//

//Includes
#include "NoteStack.h"
#include "stdio.h"

//instantiates NoteStack constructed to max depth of (n) stored notes
NoteStack myNoteStack(5);


int main( void )
{
    //This used to provide test note.  Can be controlled by midi
    stackNote myStackNote;

    myStackNote.noteValue = 24;
    myStackNote.noteVelocity = 64;
    printf("Pushing note.\n");
    myNoteStack.pushNote( myStackNote );
    myNoteStack.printfNoteStack();

    myStackNote.noteValue = 12;
    myStackNote.noteVelocity = 32;
    printf("Pushing note.\n");
    myNoteStack.pushNote( myStackNote );
    myNoteStack.printfNoteStack();

    printf("Popping off the last note.\n");
    myNoteStack.popNote();
    myNoteStack.printfNoteStack();

    myStackNote.noteValue = 19;
    myStackNote.noteVelocity = 40;
    printf("Pushing note.\n");
    myNoteStack.pushNote( myStackNote );
    myNoteStack.printfNoteStack();

    printf("Pushing 10 notes.\n");
    for( int i = 1; i < 10; i++)
    {
        myStackNote.noteValue = i;
        myNoteStack.pushNote( myStackNote );
    }
    myNoteStack.printfNoteStack();

    printf("Popping off the last note.\n");
    myNoteStack.popNote();
    myNoteStack.printfNoteStack();


    printf("Pushing 10 more notes.\n");
    for( int i = 10; i < 20; i++)
    {
        myStackNote.noteValue = i;
        myNoteStack.pushNote( myStackNote );
    }
    myNoteStack.printfNoteStack();

    printf("Seeking notevalue 19.\n");
    printf("Depth: %d\n", myNoteStack.seekNote( myStackNote ));
    myNoteStack.printfNoteStack();

    printf("dropping 19.\n");
    myNoteStack.dropNote(myNoteStack.seekNote( myStackNote ));
    myNoteStack.printfNoteStack();

    printf("attempt to pop 10 off\n");
    for( int i = 0; i < 10; i++ )
    {
        myNoteStack.popNote();
    }
    myNoteStack.printfNoteStack();

    printf("seeking notevalue 22\n");
    myStackNote.noteValue = 22;
    printf("Depth: %d\n", myNoteStack.seekNote( myStackNote ));

}



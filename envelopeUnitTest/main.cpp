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
#include "file.h"
#include "stdio.h"

//Globals

Envelope myEnvelope;

int main( void )
{
    char str [80];
    int i;

    printf ("Enter your family name: ");
    scanf ("%79s",str);
    printf (str);

    myEnvelope.setAttack(.05);
    myEnvelope.setDecay(.04);
    myEnvelope.setRelease(.1);
    myEnvelope.setSustain(64);
    printf("Passing 31, adding 20\n");

}

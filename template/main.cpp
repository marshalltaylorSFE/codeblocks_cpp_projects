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
BasicClass myClass;

int main( void )
{
    printf("Passing 31, adding 20\n");
    printf("Result is: %d", myClass.basicMethod(31));

}

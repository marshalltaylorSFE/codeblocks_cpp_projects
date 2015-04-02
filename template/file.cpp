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

//**********************************************************************//
//  First class methods (heh.)
//
BasicClass::BasicClass( void )
{
    variable = 20;
}

int BasicClass::basicMethod( int input )
{
    return input + variable;
}

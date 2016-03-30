//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  March 23, 2016
//
//**********************************************************************//
#ifndef FREQUENCYMATH_H_INCLUDED
#define FREQUENCYMATH_H_INCLUDED
#include <stdint.h>

//Classes
class BPOTestClass
{
public:
    BPOTestClass( void );
    float calcBPO( uint16_t );
    uint64_t calcBPOInc( uint16_t );

};

#endif // FILE_H_INCLUDED

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

//Includes
#include "frequencyMath.h"
#include <stdint.h>

// bitPerOctaveLUTs.c
extern "C" {
extern const uint16_t twoPowers13bit[8192];
extern const float note2bpo[129];
}


//**********************************************************************//
//  First class methods (heh.)
//
BPOTestClass::BPOTestClass( void )
{
}

float BPOTestClass::calcBPO( uint16_t input )
{
    //split top 3 bits
    uint16_t inputFractional = input & 0x1FFF;
    uint16_t inputWhole = ( input & 0xE000 ) >> 13;
    uint16_t baseFreq = ( 5 << inputWhole );
    return baseFreq * ((float)twoPowers13bit[inputFractional]/8192 + 1);
    //return (float)twoPowers13bit[inputFractional]/8192;
}

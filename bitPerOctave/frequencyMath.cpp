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
extern const uint16_t twoPowers12bit[4096];
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
    uint16_t inputFractional = input & 0x0FFF;
    uint16_t inputWhole = ( input & 0x7000 ) >> 12;
    uint16_t baseFreq = ( 5 << inputWhole );
    return baseFreq * ((float)twoPowers12bit[inputFractional]/65536 + 1);
    //return (float)twoPowers13bit[inputFractional]/8192;
}

uint64_t BPOTestClass::calcBPOInc( uint16_t input )
{
    //split top 3 bits
    uint16_t inputFractional = input & 0x0FFF;
    uint16_t inputWhole = ( input & 0x7000 ) >> 12;
    uint64_t baseFreq = ( 0x76D6A << inputWhole );
    return (baseFreq * ((((uint64_t)twoPowers12bit[inputFractional] + 0x10000 ) << 8))) >> 24;
    //return (baseFreq * 0x1000000 ) >> 24;
}

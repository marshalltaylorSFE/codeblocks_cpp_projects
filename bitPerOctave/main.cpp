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
#include "stdio.h"
#include <stdint.h>

//Globals
BPOTestClass myClass;

// bitPerOctaveLUTs.c
extern "C" {
extern const uint16_t twoPowers13bit[8192];
extern const float note2bpo[129];
}

int main( void )
{
    printf("float BPO lookup of key A4 is: %f\n", note2bpo[69]);

    //This will be done at key press time -- generate a bit-per-octave Q3.13 format to pass
    uint16_t wholePart = (uint16_t)note2bpo[69];
    uint16_t fractionalPart = (uint16_t)((note2bpo[69] - wholePart)*8192);
    uint16_t tempAudioBlockData = wholePart << 13 | fractionalPart;
    printf("Q3.13 value is: %X\n", tempAudioBlockData);

    //This will be done at update time
    printf("Result in hz is: %f\n", myClass.calcBPO(tempAudioBlockData));

    //This shows adding whole liner numbers to pitch
    printf("\nNow '1' is added to the Q3.13 representation of the previous\n", tempAudioBlockData);
    wholePart = (uint16_t)note2bpo[69];
    fractionalPart = (uint16_t)((note2bpo[69] - wholePart)*8192);
    wholePart++;
    tempAudioBlockData = wholePart << 13 | fractionalPart;
    printf("Q3.13 value is: %X\n", tempAudioBlockData);

    //This will be done at update time
    printf("Result in hz is: %f\n", myClass.calcBPO(tempAudioBlockData));
}

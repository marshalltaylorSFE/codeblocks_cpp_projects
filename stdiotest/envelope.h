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
#ifndef ENVELOPE_H_INCLUDED
#define ENVELOPE_H_INCLUDED
#include "stdint.h"

//States
#define SM_IDLE 0
#define SM_ATTACK 1
#define SM_DECAY 2
#define SM_RELEASE 3

//Note states
#define NOTE_OFF 0
#define NOTE_ON 1
#define NOTE_NEW 2

class Delta8BitMath
{
public:
  //Constructor
  Delta8BitMath( void );

  //Methods
  void set( float, int8_t );

  //Variables
  uint8_t whole;
  uint16_t partial;
  uint16_t partialAcu;
  int8_t dir;  //1 is positive, -1 is ne..  yeah...

};

class Level8BitMath
{
public:
  //Constructor
  Level8BitMath( void );

  //Methods
  void set( uint8_t );

  //Variables
  uint8_t level;

};

class Envelope
{
public:
  //Constructor
  Envelope( void );

  //Methods
  void tick( void );
  void setNoteOn( void );
  void setNoteOff( void );
  void setAttack( float );
  void setDecay( float );
  void setSustain( uint8_t );
  void setRelease( float );

  //Variables
  uint8_t amp;

//private:
  //Methods
  void changeAmp( Delta8BitMath & );

  //Variables
  uint8_t state;
  uint8_t noteState;
  Delta8BitMath envAttack;
  Delta8BitMath envDecay;
  Level8BitMath envSustain;
  Delta8BitMath envRelease;

};

#endif // ENVELOPE_H_INCLUDED

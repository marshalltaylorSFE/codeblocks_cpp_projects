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

class RateParameter
{
public:
  //Constructor
  RateParameter( void );

  //Variables
  uint16_t timeScale;
  int8_t powerScale;
  uint8_t maxAmp;
  uint8_t refValue;
  int8_t dir; // 1 or -1

};

class LevelParameter
{
public:
  //Constructor
  LevelParameter( void );

  //Variables
  uint8_t level;

};

class TimeKeeper
{
public:
    TimeKeeper( void );
    void mClear( void );
    uint16_t mGet( void );
    void mIncrement( uint8_t );

private:
    uint16_t timeElapsed;

};

class Envelope
{
public:
  //Constructor
  Envelope( void );

  //Methods
  void tick( uint8_t );
  void setNoteOn( void );
  void setNoteOff( void );
  void setAttack( uint8_t, int8_t );
  void setDecay( uint8_t, int8_t );
  void setSustain( uint8_t );
  void setRelease( uint8_t, int8_t );

  //Variables
  uint8_t amp;

//private:
  //Methods
  void changeAmp( RateParameter &, uint16_t );

  //Variables
  uint8_t state;
  uint8_t noteState;
  //Something to do with time tracking...  time since state entry?
  TimeKeeper mainTK;

  RateParameter envAttack;
  RateParameter envDecay;
  LevelParameter envSustain;
  RateParameter envRelease;

};

#endif // ENVELOPE_H_INCLUDED

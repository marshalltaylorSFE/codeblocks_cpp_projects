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
#define SM_POST_DECAY 2
#define SM_PRE_DECAY 3
#define SM_DECAY 4
#define SM_PRE_RELEASE 5
#define SM_RELEASE 6
#define SM_POST_RELEASE 7
#define SM_ATTACK_HOLD 8

//Note states
#define NOTE_OFF 0
#define NOTE_ON 1
#define NOTE_NEW 2

//Knob range scaling
#define MAX_ATTACK_MS 1000
#define MAX_DECAY_MS 1000
#define MAX_RELEASE_MS 5000

class RateParameter
{
public:
  //Constructor
  RateParameter( void );
//Variables
  uint16_t timeScale;
  int8_t powerScale;


};

class LevelParameter
{
public:
  //Constructor
  LevelParameter( void );
  uint8_t getLevel( void );
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
  uint16_t getDecay( void );
  void setSustain( uint8_t );
  void setRelease( uint8_t, int8_t );
  void setAttackHold( uint8_t );
  //void setState( uint8_t );

  //RateParameter getAttack( void );
  //RateParameter getDecay( void );
  //RateParameter getRelease( void );

  //Variables
  uint8_t amp;
  uint8_t shadowAmp;
private:

  //Methods
  void changeAmp( RateParameter&, uint16_t, uint8_t, uint8_t& );
  void changeAmp( LevelParameter&, uint8_t& );
  void changeAmp( uint8_t, uint8_t& );

  //Variables
  uint8_t state;
public:
  uint8_t noteState;
private:
  //Timekeepers are modified logical counter modules
  TimeKeeper mainTimeKeeper;
  TimeKeeper shadowTimeKeeper;

  RateParameter envAttack;
  //RateParameter envAttackShadow;
  RateParameter envDecay;
  //RateParameter envDecayShadow;
  LevelParameter envSustain;
  RateParameter envRelease;
  //RateParameter envReleaseShadow;
public:
  RateParameter envAttackHold;

};

#endif // ENVELOPE_H_INCLUDED

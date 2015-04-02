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
Envelope::Envelope( void )
{
  //Init variables
  state = SM_IDLE;
  noteState = NOTE_OFF;
  amp = 0;

  setAttack( 0 );
  setDecay( 0 );
  setSustain( 0 );
  setRelease( 0 );

}

//Call at some repetition-- 1kHz.
void Envelope::tick()
{
  //State machine
  uint8_t next_state = state;
  switch( state ) {
    case SM_IDLE:
      //do nothing
      if( noteState != NOTE_OFF )
      {
        next_state = SM_ATTACK;
        noteState = NOTE_ON;
      }
      break;
    case SM_ATTACK:
      //Increment amp or leave
      if((( noteState == NOTE_OFF )&&( amp > envSustain.level )) || ( amp >= 127 ) )
      {
        next_state = SM_DECAY;
      }
      else
      {
        //ignore new note states, convert to note on
        if( noteState == NOTE_NEW )
        {
          noteState = NOTE_ON;
        }
        //change amp
        changeAmp( envAttack );
      }
      break;
    case SM_DECAY:
      //If there is a new note, start attacking and do nothing else
      if( noteState == NOTE_NEW )
      {
        noteState = NOTE_ON;
        next_state = SM_ATTACK;
      }
      else
      {
        if( amp > envSustain.level )  //Always get to sustain level before advancing
        {
          changeAmp( envDecay );
        }
        else
        {
          if( noteState == NOTE_OFF )
          {
            //Advance
            next_state = SM_RELEASE;
          }
          else
          {
            //Hold
          }
        }
      }
      break;
    case SM_RELEASE:
      if( noteState != NOTE_OFF )
      {
        next_state = SM_ATTACK;
      }
      else
      {
        if( amp > 0 )
        {
          changeAmp( envRelease );
        }
      }
      break;
    default:
      break;
  }
  //Apply the next state after the logic has completed
  state = next_state;
}

void Envelope::changeAmp( Delta8BitMath & param )
{
  int16_t amp_temp = amp;
    //Only allow 0-127 as outputs

    //add the whole part
    amp_temp += (param.whole * param.dir);

    //add the partial to the accumulator
    param.partialAcu = param.partialAcu + param.partial;

    //if the partial is greater than 1, crop the accumulator and increment the pointer
    amp_temp += ( param.partialAcu >> 15 ) * param.dir;

    param.partialAcu &= 0x7FFF;

    //limit max to 127
    if( amp_temp > 127 )
    {
      amp_temp = 127;
    }
    if( amp_temp < 0 )
    {
      amp_temp = 0;
    }

    amp = amp_temp;
    //maybe some scaling goes here?
}

void Envelope::setNoteOn( void )
{
  noteState = NOTE_NEW;

}

void Envelope::setNoteOff()
{
  noteState = NOTE_OFF;

}

void Envelope::setAttack( float var_attack )
{
  envAttack.set( var_attack, 1 );

}

void Envelope::setDecay( float var_decay )
{
  envDecay.set( var_decay, -1);
}

void Envelope::setSustain( uint8_t var_sustain )
{
  envSustain.set( var_sustain );

}

void Envelope::setRelease( float var_release )
{
  envRelease.set( var_release, -1 );

}

Delta8BitMath::Delta8BitMath( void )
{
  //init variables
  whole = 0;
  partial = 0;
  partialAcu = 0;

}

Level8BitMath::Level8BitMath( void )
{
  //init variables
  level = 0;

}

void Delta8BitMath::set( float time_seconds, int8_t var_dir )
{
  //used for delta
  float math_temp;
  math_temp =  256 / ( time_seconds * 1000 );
  whole = (uint16_t)math_temp;  //This can be unloaded into note_values.c if necessary
  partial = (uint16_t)(( math_temp - whole ) * 32768);
  dir = var_dir;

}

void Level8BitMath::set( uint8_t velocity )
{
  //used for level
  level = velocity & 0x7F;  //protection only

}

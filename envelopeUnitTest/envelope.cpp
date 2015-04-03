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
#include "envelope.h"
#include "math.h"
#include "stdio.h"

//**********************************************************************//
//  Envelope Constructor
Envelope::Envelope( void )
{
  //Init variables
  state = SM_IDLE;
  noteState = NOTE_OFF;
  amp = 0;

}

//**********************************************************************//
//Call at some repetition.  Pass ms since last call
void Envelope::tick( uint8_t msElapsed )
{
  mainTK.mIncrement(msElapsed);

  //State machine
  uint8_t next_state = state;
  switch( state ) {
    case SM_IDLE:
      //do nothing
      if( noteState != NOTE_OFF )
      {
        next_state = SM_ATTACK;
        mainTK.mClear();
        noteState = NOTE_ON;
      }
      break;
    case SM_ATTACK:
      //Increment amp or leave
      if((( noteState == NOTE_OFF )&&( amp > envSustain.level )) || ( amp >= 255 ) )
      {
        next_state = SM_DECAY;
        mainTK.mClear();
      }
      else
      {
        //ignore new note states, convert to note on
        if( noteState == NOTE_NEW )//inChar
        {
          noteState = NOTE_ON;
        }
        //change amp
        changeAmp( envAttack, mainTK.mGet() );
      }
      break;
    case SM_DECAY:
      //If there is a new note, start attacking and do nothing else
      if( noteState == NOTE_NEW )
      {
        noteState = NOTE_ON;
        next_state = SM_ATTACK;
        mainTK.mClear();
      }
      else
      {
        if( amp > envSustain.level )  //Always get to sustain level before advancing
        {
          changeAmp( envDecay, mainTK.mGet() );
        }
        else
        {
          if( noteState == NOTE_OFF )
          {
            //Advance
            next_state = SM_RELEASE;
            mainTK.mClear();
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
        mainTK.mClear();
      }
      else
      {
        if( amp > 0 )
        {
          changeAmp( envRelease, mainTK.mGet() );
        }
      }
      break;
    default:
      break;
  }
  //Apply the next state after the logic has completed
  state = next_state;
}

//**********************************************************************//
//ChangeAmp is called by the state machine, which is operated at some
//  frequency.  It is passed a 'time since state start' parameter.
void Envelope::changeAmp( RateParameter& param, uint16_t timeVar )
{


  int16_t ampTemp = (uint16_t)(param.maxAmp*(float)pow(((float)timeVar/(float)param.timeScale), (exp((double)2*(float)param.powerScale/127))));

  if(ampTemp > param.maxAmp)
  {
      ampTemp = param.maxAmp;
  }
  else
  {
      ampTemp = ampTemp;
  }
  amp = (param.refValue + ( param.dir * ampTemp ));
  if(ampTemp <= 0)
  {
      ampTemp = 0;
  }

  printf("\n\n\n%d    ", ampTemp);
  printf("\n\n\n\n\n");
  printf("%d   \n", param.timeScale );
  printf("%d   \n", param.powerScale );
  printf("%d   \n", param.maxAmp );
  printf("%d   \n", param.dir );
  printf("%d   \n", envSustain.level );

//  printf("%f\n", exp(2*(float)param.powerScale/127) );

}

void Envelope::setNoteOn( void )
{
  noteState = NOTE_NEW;

}

void Envelope::setNoteOff()
{
  noteState = NOTE_OFF;

}

void Envelope::setAttack( uint8_t var_attack, int8_t var_power )
{
  //Scale 0-255 input parameters to the appropriate phase range in ms
  envAttack.timeScale = (((uint32_t)var_attack * 1000) >> 8);
  envAttack.powerScale = var_power;
  envAttack.maxAmp = 255;
  envAttack.refValue = 0;
  envAttack.dir = 1;

}

void Envelope::setDecay( uint8_t var_decay, int8_t var_power )
{
  envDecay.timeScale = (((uint32_t)var_decay * 1000) >> 8);
  envDecay.powerScale = var_power;
  envDecay.maxAmp = 255 - envSustain.level;
  envDecay.refValue = 255;
  envDecay.dir = -1;

}

void Envelope::setSustain( uint8_t var_sustain )
{
  envSustain.level = var_sustain;

}

void Envelope::setRelease( uint8_t var_release, int8_t var_power )
{
  envRelease.timeScale = (((uint32_t)var_release * 5000) >> 8);
  envRelease.powerScale = var_power;
  envRelease.maxAmp = envSustain.level;
  envRelease.refValue = envSustain.level;
  envRelease.dir = -1;

}

RateParameter::RateParameter( void )
{
  //Constructor


}

LevelParameter::LevelParameter( void )
{
  //Constructor


}

//**********************************************************************//
//Keeps time in number of milliseconds
TimeKeeper::TimeKeeper( void )
{
    timeElapsed = 0;

}

void TimeKeeper::mClear( void )
{
    timeElapsed = 0;

}

uint16_t TimeKeeper::mGet( void )
{
    return timeElapsed;
}

void TimeKeeper::mIncrement( uint8_t var_Increment )
{
    //This can overflow without warning--
    //  sacrificing coverage for speed
    timeElapsed += var_Increment;
}

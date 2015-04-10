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
    mainTimeKeeper.mIncrement(msElapsed);
    shadowTimeKeeper.mIncrement(msElapsed);

    //State machine
    uint8_t next_state = state;
    switch( state )
    {
    case SM_IDLE:
        //do nothing
        if( noteState != NOTE_OFF )
        {
            next_state = SM_ATTACK;
            mainTimeKeeper.mClear();
            noteState = NOTE_ON;
        }
        break;
    case SM_ATTACK:
        //Increment amp or leave
        if( amp == 255 )
        {
            next_state = SM_ATTACK_HOLD;
            mainTimeKeeper.mClear();
        }
        else if( noteState == NOTE_OFF )
        {
            //keep attacking this state
            changeAmp( envAttack, mainTimeKeeper.mGet(), state, amp );

            shadowTimeKeeper.mClear();

            if( amp < envSustain.getLevel() )
            {
                //Start the shadow timer at exit condition for decay, straight to release
                changeAmp( envSustain.getLevel(), shadowAmp );
                next_state = SM_PRE_RELEASE;

            }
            else
            {
                //Start the shadow timer at the peak
                changeAmp( 255, shadowAmp );
                next_state = SM_PRE_DECAY;
            }
        }
        else
        {
            //ignore new note states, convert to note on
            if( noteState == NOTE_NEW )//inChar
            {
                noteState = NOTE_ON;
            }
            //change amp
            changeAmp( envAttack, mainTimeKeeper.mGet(), state, amp );
        }
        break;
    case SM_PRE_DECAY:
        //Keep attacking the main
        changeAmp( envAttack, mainTimeKeeper.mGet(), state, amp );
        //Decay the shadow
        changeAmp( envDecay, shadowTimeKeeper.mGet(), SM_DECAY, shadowAmp );
        if( shadowAmp <= amp ) //It's time to move
        {
            //Change to the shadowAmp
            next_state = SM_DECAY;
            mainTimeKeeper = shadowTimeKeeper;
            changeAmp( envDecay, mainTimeKeeper.mGet(), SM_DECAY, amp);
        }
        break;
    case SM_ATTACK_HOLD:
        //Increment amp or leave
        if( mainTimeKeeper.mGet() > envAttackHold.timeScale )
        {
            next_state = SM_DECAY;
            mainTimeKeeper.mClear();
        }
        break;
    case SM_DECAY:
        if( amp > envSustain.level )  //Always get to sustain level before advancing
        {
            changeAmp( envDecay, mainTimeKeeper.mGet(), state, amp );
        }
        //If there is a new note, start attacking
        if( noteState == NOTE_NEW )
        {
            noteState = NOTE_ON;
            //Get ready for next attack
            //Start the shadow as the new note, switch over later
            shadowTimeKeeper.mClear();
            changeAmp( 0, shadowAmp );

            next_state = SM_POST_DECAY;

        }
        else if( (noteState == NOTE_OFF)&&( amp <= envSustain.level ))
        {
            mainTimeKeeper.mClear();
            next_state = SM_RELEASE;
        }
        break;
    case SM_POST_DECAY:
        //Decay the main until sustain
        if( amp > envSustain.level )
        {
            changeAmp( envDecay, mainTimeKeeper.mGet(), state, amp );
        }
        // else do nothing
        //Attack the shadow
        changeAmp( envAttack, shadowTimeKeeper.mGet(), SM_ATTACK, shadowAmp );
        if( shadowAmp > amp ) //It's time to move
        {
            next_state = SM_ATTACK;
            //Now, copy the shadow to the main
            mainTimeKeeper = shadowTimeKeeper;
        }
        break;
    case SM_PRE_RELEASE:
        //Continue attacking the main
        changeAmp( envAttack, mainTimeKeeper.mGet(), state, amp );
        //Start releasing the shadow
        changeAmp( envRelease, shadowTimeKeeper.mGet(), SM_RELEASE, shadowAmp );
        if( shadowAmp < amp ) //It's time to move
        {
            //change to shadowAmp
            next_state = SM_RELEASE;
            mainTimeKeeper = shadowTimeKeeper;
            changeAmp( envRelease, mainTimeKeeper.mGet(), SM_RELEASE, amp );
        }
        break;
    case SM_RELEASE:
        if( noteState != NOTE_OFF )
        {
            //Note went back on!
            //Start the shadow as the new note, switch over later
            shadowTimeKeeper.mClear();
            changeAmp( 0, shadowAmp );
            next_state = SM_POST_RELEASE;

        }
        else  //it does equal note off
        {
            if( amp > 0 )
            {
                changeAmp( envRelease, mainTimeKeeper.mGet(), state, amp );
            }
        }
        break;
    case SM_POST_RELEASE:
        if( noteState == NOTE_OFF )
        {
            //go back to the release
            next_state = SM_RELEASE;
            //put the shadow away

        }
        //Keep decaying the main
        else
        {
            if( amp > 0 )
            {
                changeAmp( envRelease, mainTimeKeeper.mGet(), state, amp );
            }
            //Start attacking the shadow
            changeAmp( envAttack, shadowTimeKeeper.mGet(), SM_ATTACK, shadowAmp );
            if( shadowAmp > amp ) //It's time to move
            {
                next_state = SM_ATTACK;
                //Now, copy the shadow to the main

                mainTimeKeeper = shadowTimeKeeper;
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
void Envelope::changeAmp( RateParameter& param, uint16_t timeVar, uint8_t stateVar, uint8_t& ampVar )
{
    int8_t polarity = 1;
    uint8_t refLevel = 0;
    uint8_t maxAmp = 255;

    switch(stateVar)
    {
    case SM_DECAY:
    case SM_POST_DECAY:
        timeVar = param.timeScale - timeVar;
        polarity = 1;
        refLevel = envSustain.getLevel();
        maxAmp = 255 - refLevel;
        break;
    case SM_RELEASE:
    case SM_POST_RELEASE:
        if( timeVar < param.timeScale )
        {
            timeVar = param.timeScale - timeVar;
        }
        else
        {
            timeVar = 0;
        }
        maxAmp = envSustain.getLevel();
        polarity = 1;
        break;
    default:
        break;
    }

    int16_t ampTemp = (uint16_t)(maxAmp*(float)pow(((float)timeVar/(float)param.timeScale), (exp((double)2*(float)param.powerScale/127))));

    if(ampTemp > 255)
    {
        ampTemp = 255;
    }

    if(ampTemp <= 0)
    {
        ampTemp = 0;
    }

    ampVar = (refLevel + ( polarity * ampTemp ));


}

void Envelope::changeAmp( LevelParameter& param, uint8_t& ampVar )
{
    ampVar = param.level;

}

//Force output
void Envelope::changeAmp( uint8_t ampvar, uint8_t& ampVar )
{
    ampVar = ampvar;

}

//Use this if you need to force states.  Not a terribly polite method.
//void Envelope::setState( uint8_t stateVar )
//{
//    state = stateVar;
//
//}

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

}

void Envelope::setDecay( uint8_t var_decay, int8_t var_power )
{
    envDecay.timeScale = (((uint32_t)var_decay * 1000) >> 8);
    envDecay.powerScale = var_power;

}

void Envelope::setSustain( uint8_t var_sustain )
{
    envSustain.level = var_sustain;

}

uint8_t LevelParameter::getLevel( void )
{
    return level;

}

void Envelope::setRelease( uint8_t var_release, int8_t var_power )
{
    envRelease.timeScale = (((uint32_t)var_release * 5000) >> 8);
    envRelease.powerScale = var_power;

}

void Envelope::setAttackHold( uint8_t var_attackHold )
{
    envAttackHold.timeScale = (((uint32_t)var_attackHold * 25) >> 8);

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

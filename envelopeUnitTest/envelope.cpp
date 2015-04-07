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

extern Envelope myEnvelopeShadow;
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
    switch( state )
    {
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
        if( amp == 255 )
        {
            next_state = SM_DECAY;
            mainTK.mClear();
        }
        else if( noteState == NOTE_OFF )
        {
            myEnvelopeShadow.envDecay = envDecay;
            myEnvelopeShadow.envSustain = envSustain;
            myEnvelopeShadow.envRelease = envRelease;
            myEnvelopeShadow.mainTK.mClear();

            myEnvelopeShadow.setNoteOff();

            if( amp < envSustain.getLevel() )
            {
                //Start the shadow timer at exit condition for decay, straight to release
                myEnvelopeShadow.changeAmp( envSustain.getLevel() );
                myEnvelopeShadow.setState( SM_RELEASE );
                next_state = SM_PRE_RELEASE;

            }
            else
            {
                //Start the shadow timer at the peak
                myEnvelopeShadow.changeAmp(255);
                myEnvelopeShadow.setState( SM_DECAY );
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
            changeAmp( envAttack, mainTK.mGet(), state );
        }
        break;
    case SM_PRE_DECAY:
        changeAmp( envAttack, mainTK.mGet(), state );
        //Adjust a few cycles to 'go early'
        if( ((myEnvelopeShadow.amp - 8) < amp) && (myEnvelopeShadow.state > SM_ATTACK) ) //It's time to move
        {
            next_state = SM_DECAY;
            mainTK = myEnvelopeShadow.mainTK;


        }
        break;
    case SM_DECAY:
        //If there is a new note, start attacking and do nothing else
        if( noteState == NOTE_NEW )
        {
            noteState = NOTE_ON;
            //Get ready for next attack
            next_state = SM_POST_DECAY;
            //Start the shadow as the new note, switch over later
            myEnvelopeShadow.envAttack = envAttack;
            myEnvelopeShadow.envDecay = envDecay;
            myEnvelopeShadow.envSustain = envSustain;
            myEnvelopeShadow.envRelease = envRelease;
            myEnvelopeShadow.mainTK.mClear();
            myEnvelopeShadow.changeAmp( 0 );
            myEnvelopeShadow.setState( SM_ATTACK );
            myEnvelopeShadow.setNoteOn();

            next_state = SM_POST_DECAY;

        }
        else
        {
            if( amp > envSustain.level )  //Always get to sustain level before advancing
            {
                changeAmp( envDecay, mainTK.mGet(), state  );
            }
            else
            {
                if( noteState == NOTE_OFF )
                {
                    mainTK.mClear();
                    next_state = SM_RELEASE;

                }
                else
                {
                    //Hold
                    changeAmp( envSustain );
                }
            }
        }
        break;
    case SM_POST_DECAY:
        if( amp > envSustain.level )  //Always get to sustain level before advancing
        {
            changeAmp( envDecay, mainTK.mGet(), state  );
        }
        if( (myEnvelopeShadow.amp) > amp ) //It's time to move
        {
            next_state = SM_ATTACK;
            //Now, copy the shadow to the main

            mainTK = myEnvelopeShadow.mainTK;
            envAttack = myEnvelopeShadow.envAttack;
            envDecay = myEnvelopeShadow.envDecay;
            envSustain = myEnvelopeShadow.envSustain;
            envRelease = myEnvelopeShadow.envRelease;

        }
        break;
    case SM_PRE_RELEASE:
        changeAmp( envAttack, mainTK.mGet(), state );
        if( ((myEnvelopeShadow.amp - 8) < amp) && (myEnvelopeShadow.state == SM_RELEASE) ) //It's time to move
        {
            next_state = SM_RELEASE;
            mainTK = myEnvelopeShadow.mainTK;

        }
        break;
    case SM_RELEASE:
        if( noteState != NOTE_OFF )
        {
            //Note went back on!
            //Start the shadow as the new note, switch over later
            myEnvelopeShadow.envAttack = envAttack;
            myEnvelopeShadow.envDecay = envDecay;
            myEnvelopeShadow.envSustain = envSustain;
            myEnvelopeShadow.envRelease = envRelease;
            myEnvelopeShadow.mainTK.mClear();
            myEnvelopeShadow.setNoteOn();
            myEnvelopeShadow.changeAmp( 0 );
            myEnvelopeShadow.setState( SM_ATTACK );

            next_state = SM_POST_RELEASE;

        }
        else
        {
            if( amp > 0 )
            {
                changeAmp( envRelease, mainTK.mGet(), state  );
            }
        }
        break;
    case SM_POST_RELEASE:
        if( amp > 0 )
        {
            changeAmp( envRelease, mainTK.mGet(), state  );
        }
        if( (myEnvelopeShadow.amp) > amp ) //It's time to move
        {
            next_state = SM_ATTACK;
            //Now, copy the shadow to the main

            mainTK = myEnvelopeShadow.mainTK;
            envAttack = myEnvelopeShadow.envAttack;
            envDecay = myEnvelopeShadow.envDecay;
            envSustain = myEnvelopeShadow.envSustain;
            envRelease = myEnvelopeShadow.envRelease;


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
void Envelope::changeAmp( RateParameter& param, uint16_t timeVar, uint8_t stateVar )
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
        timeVar = param.timeScale - timeVar;
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

    amp = (refLevel + ( polarity * ampTemp ));


    printf("\n\n\n%d    ", ampTemp);
    printf("\n\n\n\n\n");
    printf("%d   \n", param.timeScale );
    printf("%d   \n", param.powerScale );
    //printf("%d   \n", param.maxAmp );
    //printf("%d   \n", param.dir );
    printf("%d   \n", envSustain.level );

//  printf("%f\n", exp(2*(float)param.powerScale/127) );

}

//Use this if you need to force states.  Not a terribly polite method.
void Envelope::setState( uint8_t stateVar )
{
    state = stateVar;

}

//Force amps
void Envelope::changeAmp( uint8_t AmpVar )
{
    amp = AmpVar;

}
//void Envelope::changeAmp( RateParameter& param, uint16_t timeVar )
//{
//    if( param.dir < 0 )
//    {
//        timeVar = param.timeScale - timeVar;
//    }
//
//    //generate a fully scaled shape here
//    int16_t ampTemp = (uint16_t)(param.maxAmp*(float)pow(((float)timeVar/(float)param.timeScale), (exp((double)2*(float)param.powerScale/127))));
//
//    if(ampTemp > param.maxAmp)
//    {
//        ampTemp = param.maxAmp;
//    }
//    else
//    {
//        ampTemp = ampTemp;
//    }
//    amp = (param.refValue + ( param.dir * ampTemp ));
//    if(ampTemp <= 0)
//    {
//        ampTemp = 0;
//    }
//
//    printf("\n\n\n%d    ", ampTemp);
//    printf("\n\n\n\n\n");
//    printf("%d   \n", param.timeScale );
//    printf("%d   \n", param.powerScale );
//    printf("%d   \n", param.maxAmp );
//    printf("%d   \n", param.dir );
//    printf("%d   \n", envSustain.level );
//
////  printf("%f\n", exp(2*(float)param.powerScale/127) );
//
//}

void Envelope::changeAmp( LevelParameter& param )
{
    amp = param.level;

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
    //envAttack.maxAmp = 255;
    //envAttack.refValue = 0;
    //envAttack.dir = 1;

}

void Envelope::setDecay( uint8_t var_decay, int8_t var_power )
{
    envDecay.timeScale = (((uint32_t)var_decay * 1000) >> 8);
    envDecay.powerScale = var_power;
    //envDecay.maxAmp = 255 - envSustain.level;
    //envDecay.refValue = 255;
    //envDecay.dir = -1;

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
    //envRelease.maxAmp = envSustain.level;
    //envRelease.refValue = envSustain.level;
    //envRelease.dir = -1;

}

RateParameter::RateParameter( void )
{
    //Constructor
    myEnvelopeShadow.setState( 1 );


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

#include "PanelSwitch.h"

PanelSwitch::PanelSwitch( uint8_t pinNum )
{
    pinNumber = pinNum;

}

void PanelSwitch::init( void )
{
    //pinMode( uint8_t pinNumber, INPUT_PULLUP );
    update();
    //force newData high in case knob starts on last value
    newData = 1;
}

void PanelSwitch::update( void )
{
    //uint8_t tempState = digitalRead( pinNumber );
    if( state != tempState )
    {
        state = tempState;
        newData = 1;
    }
}

uint8_t PanelSwitch::getState( void )
{
    newData = 0;

    return state;
}

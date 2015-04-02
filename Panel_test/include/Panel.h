#ifndef PANEL_H
#define PANEL_H

#include "PanelSwitch.h"

#define rampPin 1
#define squarePin 1
#define sinePin 1
#define loadPin 1

class Panel
{
    public:
        Panel();
        update();

        PanelSwitch ramp( rampPin );
        PanelSwitch square( squarePin );
        PanelSwitch sine( sinePin );
        PanelSwitch load( loadPin );
        //PanelRegister switch1( registerB2Pin, registerB1Pin, registerB0Pin );
        //PanelKnob master( masterPin );
        //PanelKnob coarse( coarsePin );
        //PanelKnob fine( finePin );
   protected:
    private:
};

#endif // PANEL_H

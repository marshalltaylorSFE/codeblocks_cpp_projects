#ifndef PANELSWITCH_H
#define PANELSWITCH_H


class PanelSwitch
{
    public:
        PanelSwitch( uint8_t );
        void update( void );
        void init( void );
        uint8_t getState( void );
        uint8_t state;
        uint8_t invert;
        uint8_t pinNumber;
    protected:
    private:
};

#endif // PANELSWITCH_H

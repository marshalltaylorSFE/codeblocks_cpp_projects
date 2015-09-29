#ifndef SYNTHVOICE_H
#define SYNTHVOICE_H

class SynthVoice
{
public:
    SynthVoice( void );
    uint8_t init( void );
    int8_t tick( void );
    void setFreq( char note );
    void buildTable( void );

    //Panel related variables
    float coarseTune;
    float fineTune;
    uint8_t dutyCycle;
    uint8_t rampVol;
    uint8_t sineVol;
    uint8_t pulseVol;
    uint8_t masterVol;
    int8_t octaveAdjust;
    uint8_t protectTuning;
    uint8_t rampEnabled;
    uint8_t sineEnabled;
    uint8_t pulseEnabled;


    int8_t wavetable[256];
    uint8_t wavetable_ptr;

    uint8_t outVolume;

    //voice parameter variables:
    uint16_t freq_whole;
    uint16_t freq_partial;
    uint16_t freq_partial_acu;


};

#endif // SYNTHVOICE_H

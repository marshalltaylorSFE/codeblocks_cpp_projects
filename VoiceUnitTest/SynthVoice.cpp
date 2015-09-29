#include "wavegen.h"
#include "note_values.h"

#include "SynthVoice.h"


SynthVoice::SynthVoice( void )
{
    //Panel related variables
    coarseTune = 1;
    fineTune = 1;
    dutyCycle = 60;
    rampVol = 255;
    sineVol = 255;
    pulseVol = 255;
    masterVol = 255;
    octaveAdjust = 0;
    protectTuning = 1;

    wavetable_ptr = 0;

    rampEnabled = 0;
    sineEnabled = 1;
    pulseEnabled = 0;

    outVolume = 255;

    //voice parameter variables:
    freq_whole = 0;
    freq_partial = 0;
    freq_partial_acu = 0;


}

// -----------------------------------------------------------------------------
uint8_t SynthVoice::init( void )
{
  //Generate table from libraries
  for(int i = 0; i < 256; i++)
  {
    wavetable[i] = 128;
  }

  // initialize serial:
  //Serial.begin(9600);
  setFreq(84);
  //Serial.println(freq_whole);
  //Serial.println(freq_partial);

  return 0;

}

// --------------------------
// Custom ISR Timer Routine
// --------------------------
int8_t SynthVoice::tick()
{
  //add the whole part
  wavetable_ptr += freq_whole;

  //add the partial to the accumulator
  freq_partial_acu += freq_partial;

  //if the partial is greater than 1, crop the accumulator and increment the pointer
  wavetable_ptr += freq_partial_acu >> 15;
  freq_partial_acu &= 0x7FFF;

  wavetable_ptr &= 0x00FF;

  //scaling
  //output = wavesample * volume/127 + 127 - volume
  return ((wavetable[wavetable_ptr] * outVolume) >> 8);
  //return wavetable[wavetable_ptr];
}



void SynthVoice::setFreq( char note )  // Give the midi note number
{
  float math_temp;
  int16_t noteAdjust = note + octaveAdjust * 12;
  if( noteAdjust < 1 )
  {
    noteAdjust = 0;
  }
  if( noteAdjust > 127 )
  {
    noteAdjust = 127;
  }
  math_temp = (note_frequency[noteAdjust] * fineTune * coarseTune) * 256 / 80000;

  //End tuning
  freq_whole = (uint16_t)math_temp;  //This can be unloaded into note_values.c if necessary
  freq_partial = (uint16_t)(( math_temp - freq_whole ) * 32768);
  return;
}

void SynthVoice::buildTable( void )
{
    //Re-calculate shape

    WaveGenerator testWave;
    testWave.resetOffset();
    // Parameters (all uint8_t): ( master, ramp, sine, pulseAmp, pulseDuty )
    testWave.setParameters( masterVol, rampEnabled * rampVol, sineEnabled * sineVol, pulseEnabled * pulseVol, dutyCycle );
    for(int i = 0; i < 256; i++)
    {
      wavetable[i] = testWave.getSample();

    }
}




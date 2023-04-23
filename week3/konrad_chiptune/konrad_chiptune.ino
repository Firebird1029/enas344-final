/*

This example demonstrates how to use the DC object to modulate the frequency of an oscillator to 
fixed/quantized values (as opposed smooth/interpolated modulation to using another waveform object).
In this example, we step through an array that corresponds to semi-tone transpositions (root, 5th, 8va, 8va+5th, etc)

*/

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


// GUItool: begin automatically generated code
AudioSynthWaveformDc     dc1;            //xy=344,131
AudioSynthWaveformModulated waveformMod1;   //xy=526,137
AudioMixer4              mixer1;         //xy=754,155
AudioOutputI2S           i2s1;           //xy=953,162
AudioConnection          patchCord1(dc1, 0, waveformMod1, 0);
AudioConnection          patchCord2(waveformMod1, 0, mixer1, 0);
AudioConnection          patchCord3(mixer1, 0, i2s1, 0);
AudioConnection          patchCord4(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=766,222
// GUItool: end automatically generated code

int freqModAmt = 6;
float ampPerSemiTone;

int arp[] = {0, 7, 12, 19, 24, 31, 36, 43, 48};
int arp_length = 9;
int arp_index = 0;

int tempo = 100;
elapsedMillis timer;

void setup() {
  Serial.begin(9600);
  

  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.707); 

  waveformMod1.begin(.5, 110, WAVEFORM_BANDLIMIT_SQUARE);
  waveformMod1.frequencyModulation(freqModAmt);
  // frequencyModulation sets the range of modulation (in octaves) that  maps to +/- 1.0 amplitude
  // An amplitude modualtion of 1 / (freqModAmt * 12) correspond to a semi-tone transposition
  ampPerSemiTone = 1.f / (freqModAmt * 12);

  mixer1.gain(0, .8);

  timer = 0;

}

void loop() {

  // you can either set a fixed tempo (in ms) when you declare the variable above setup
  //or use a sensor input to change it dynamically

  //tempo = analogRead(A0);

  if(timer>=tempo) {
    dc1.amplitude(arp[arp_index] * ampPerSemiTone);
    arp_index++;
    if (arp_index >= arp_length) arp_index = 0;
    timer = 0;
  }


}
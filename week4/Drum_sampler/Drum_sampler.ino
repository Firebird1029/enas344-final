#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdRaw           playSdRaw[8];  //xy=204,89
AudioMixer4              mixer1;         //xy=425,184
AudioMixer4              mixer2;         //xy=426,260
AudioMixer4              mixer3;         //xy=571,226
AudioOutputI2S           i2s1;           //xy=717,226
AudioConnection          patchCord1(playSdRaw[0], 0, mixer1, 0);
AudioConnection          patchCord2(playSdRaw[1], 0, mixer1, 1);
AudioConnection          patchCord3(playSdRaw[2], 0, mixer1, 2);
AudioConnection          patchCord4(playSdRaw[3], 0, mixer2, 1);
AudioConnection          patchCord5(playSdRaw[4], 0, mixer1, 3);
AudioConnection          patchCord6(playSdRaw[5], 0, mixer2, 0);
AudioConnection          patchCord7(playSdRaw[6], 0, mixer2, 2);
AudioConnection          patchCord8(playSdRaw[7], 0, mixer2, 3);
AudioConnection          patchCord9(mixer1, 0, mixer3, 0);
AudioConnection          patchCord10(mixer2, 0, mixer3, 1);
AudioConnection          patchCord11(mixer3, 0, i2s1, 0);
AudioConnection          patchCord12(mixer3, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=717,283
// GUItool: end automatically generated code


const char * sf_808[] = {
  "00_kick2.raw", "01_snare.raw", "02_tom_low.raw", "04_tom_high.raw", "05_hihat_open.raw", "06_hihat_closed.raw", 
  "07_rim.raw", "08_shaker.raw"
};

elapsedMillis timer;
int counter = 0;
int delta_t = 150;

int triggers[8][16] {
   {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0},
   {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
   {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
   {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0},
   {1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1}
};

void setup() {
   
   Serial.begin(115200);


  // This uses the SD port on the teensy 4.1 *** NOT the SD reader on the audio adapter!!!
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Unable to access SD Card");
  }
  


   mixer1.gain(0, .5);
   mixer1.gain(1, .5);
   mixer1.gain(2, .5);   
   mixer1.gain(3, .5);

   mixer2.gain(0, .5);
   mixer2.gain(1, .5);
   mixer2.gain(2, .5);   
   mixer2.gain(3, .5);

   mixer3.gain(0, 1.0);
   mixer3.gain(1, 1.0);
    
   AudioMemory(120);
   sgtl5000_1.enable();
   sgtl5000_1.volume(0.8);
   
   timer = 0;
}


void loop() {

  if (timer >= delta_t){

      for (int i=0;i<8;i++){
        if(triggers[i][counter]==1) playSdRaw[i].play(sf_808[i]);
      }
      counter+=1;
      if(counter >= 16) counter = 0;
      timer = 0;
  }
  delay(5);
}

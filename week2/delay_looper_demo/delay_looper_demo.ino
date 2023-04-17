#include <Bounce2.h>
#include <Audio.h>
#include <SD.h>
#include <SPI.h>
#include "effect_delayLoop.h"


// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;      //xy=150,185
AudioEffectEnvelope      envelope1;      //xy=288,186
AudioMixer4              mixer1;         //xy=422,291
// AudioEffectDelay         delay1;         //xy=604,588
AudioEffectdelayLoop    delay1;          //xy=499,240
AudioMixer4              mixer2;         //xy=797,223
AudioOutputI2S           i2s1;           //xy=1011,215
AudioConnection          patchCord1(waveform1, envelope1);
AudioConnection          patchCord2(envelope1, 0, mixer1, 0);
AudioConnection          patchCord3(envelope1, 0, mixer2, 0);
AudioConnection          patchCord4(mixer1, delay1);
AudioConnection          patchCord5(delay1, 0, mixer2, 1);
AudioConnection          patchCord6(delay1, 0, mixer1, 1);
AudioConnection          patchCord7(mixer2, 0, i2s1, 0);
AudioConnection          patchCord8(mixer2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=137,104
// GUItool: end automatically generated code


Button Loop_button = Button();
Button Synth_button = Button();
int Loop_BUTTON_PIN = 32;
int Synth_BUTTON_PIN = 31;

elapsedMillis timer;

// number of samples at 44100 samples a second, determines the maximum delay/loop time
// in this case it's 10 seconds
#define DELAYLINE_MAX_LEN 441000  
EXTMEM int16_t delay_line[DELAYLINE_MAX_LEN] = {};

#define LOOP_TIME 2000

bool recording = false;

void setup() {

  Serial.begin(115200);
  
  Loop_button.attach(Loop_BUTTON_PIN, INPUT_PULLUP );
  Loop_button.interval(5);
  Loop_button.setPressedState(LOW);

  Synth_button.attach(Synth_BUTTON_PIN, INPUT_PULLUP );
  Synth_button.interval(5);
  Synth_button.setPressedState(LOW);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.8);

  // alocate some audio memory for 
  AudioMemory(20);

  mixer1.gain(0, 0.0);  // mixer1 input 0 is the signal from the synth to the delay/loop effect 
  mixer1.gain(1, 1.0);  // mixer1 input 1 is the feedback from the delay, set to 1.0 so the loop repeats forever

  mixer2.gain(0, .7);  // mixer2 input 0 is the 'dry' synth singal
  mixer2.gain(1, .7);  // mixer2 input 1 is the looped/delayed singal

  // start up the effect and pass it an array to store the samples
  delay1.begin(delay_line, DELAYLINE_MAX_LEN);

  waveform1.begin(0.5, 220, WAVEFORM_BANDLIMIT_SQUARE);
  envelope1.delay(0);
  envelope1.attack(10);
  envelope1.hold(0);
  envelope1.decay(250);
  envelope1.sustain(0.0);
  envelope1.release(0);

  delay1.delay(0, LOOP_TIME);  // fixed, 2-second loop duration.  This can also be changed dynamically in your program up to the limit set by the DELAYLINE_MAX_LEN value

}


void loop() {

  Loop_button.update();
  Synth_button.update();  

  // this example uses a 'loop' button to turn on the delay input and feedback gains, allowing 
  // you to route audio into the 2-second loop




  if ( Loop_button.pressed() ) {
    Serial.println("Loop button Pressed");
    if(!recording) {
      // send the synth signal into the delay loop
      mixer1.gain(0, 1.0);  
      // comment this line if you don't want to clear the loop when you record new material
      mixer1.gain(1, 0.0); 
      timer = 0; 
      recording = true;

      mixer2.gain(1, 0.0);

    }
  }

  if(timer>=LOOP_TIME && recording) {
    // turn off the signal into the delay loop
     mixer1.gain(0, 0.0);  
     // resets the feedback to 1.0 so the loop repeats indefinitely
     mixer1.gain(1, 1.0);
    recording = false;
    // timer = 0;
    mixer2.gain(1, 1.0);
  }

  if ( Synth_button.pressed()) {
    //  && timer < LOOP_TIME 
    Serial.println("Synth button Pressed");
    //set synth to random freq and trigger envelope
    waveform1.frequency(random(100, 500));
    envelope1.noteOn();
    // envelope1.noteOff()

  }

}

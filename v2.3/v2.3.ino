#include <Adafruit_GFX.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Audio.h>
#include <Bounce2.h>
#include <Encoder.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Wire.h>

#include "effect_delayLoop.h"
// TODO fix VS code errors
// #include "mixer.h"

// GENERAL

#define LOOP_TIME 2000
#define MASTER_VOLUME 0.7

// menu encoder sensitivity (how many encoder steps per menu item)
#define MES 10

#define DELAYLINE_MAX_LEN 441000  // 44100 samples/sec * 10 sec

// PINOUT

#define ENCODER_DT_PIN 3
#define ENCODER_CLK_PIN 4
#define ENCODER_BTN_PIN 2
#define RIBBON_POT_PIN_1 41  // A17
#define RIBBON_POT_PIN_2 40  // A16
#define LOOP_BTN_PIN 32
#define COMMIT_BTN_PIN 31
#define CLEAR_BTN_PIN 30

// OLED

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET_PIN -1
#define SCREEN_ADDRESS 0x3C

// MOTION

#define MOTION_ALPHA 0.5  // 0 = acceleration, 1 = gyro, default: 0.995

#define ACCELX_OFF 0.295
#define ACCELY_OFF 0.08
#define ACCELZ_OFF -0.2

#define GYROX_OFF -0.1197
#define GYROY_OFF -0.0157
#define GYROZ_OFF -0.0190

#define ACCELX_SCALE 1.008
#define ACCELY_SCALE 1.00
#define ACCELZ_SCALE 0.98

#define GYROX_SCALE 1
#define GYROY_SCALE 1
#define GYROZ_SCALE 1

// AUDIO
EXTMEM int16_t DELAY_LINE_TEMP[DELAYLINE_MAX_LEN] = {};
EXTMEM int16_t DELAY_LINE_FULL[DELAYLINE_MAX_LEN] = {};

// CUSTOM OVERRIDES
AudioEffectdelayLoop tempDelay;  // xy=607,182
AudioEffectdelayLoop fullDelay;  // xy=868,201

// GUItool: begin automatically generated code
AudioSynthWaveformDc chipDC;                // xy=55,613
AudioSynthWaveform chord3fm;                // xy=59,539
AudioSynthWaveform chord2fm;                // xy=60,499
AudioSynthSimpleDrum drumSynth;             // xy=64.5,167
AudioSynthWaveform inWaveformFM;            // xy=77,401
AudioSynthWaveformModulated inWaveformMod;  // xy=191.5,452
AudioSynthWaveformModulated chipWave;       // xy=196,620
AudioSynthWaveformModulated chord3wave;     // xy=201,545
AudioSynthWaveformModulated chord2wave;     // xy=205,506
AudioMixer4 modeSelect;                     // xy=219,80
AudioMixer4 inMixer;                        // xy=335.5,177
AudioEffectEnvelope chord3env;              // xy=351,545
AudioEffectEnvelope chord2env;              // xy=353,506
AudioEffectEnvelope inEnvelope;             // xy=363.5,453
AudioMixer4 tempDelayMixer;                 // xy=457.5,302
AudioMixer4 chordMixer;                     // xy=528,502
// AudioEffectDelay         tempDelay;      //xy=607.5,182
AudioSynthWaveformSine outLadderFreqSine;  // xy=681.5,89
AudioMixer4 outMixer;                      // xy=712.5,35
AudioMixer4 fullDelayMixer;                // xy=844.5,211
AudioFilterLadder outLadder;               // xy=880.5,30
// AudioEffectDelay         fullDelay;      //xy=1022.5,193
AudioOutputI2S i2s1;  // xy=1071.5,29
AudioConnection patchCord1(chipDC, 0, chipWave, 0);
AudioConnection patchCord2(chord3fm, 0, chord3wave, 0);
AudioConnection patchCord3(chord2fm, 0, chord2wave, 0);
AudioConnection patchCord4(drumSynth, 0, inMixer, 1);
AudioConnection patchCord5(drumSynth, 0, modeSelect, 3);
AudioConnection patchCord6(inWaveformFM, 0, inWaveformMod, 0);
AudioConnection patchCord7(inWaveformMod, inEnvelope);
AudioConnection patchCord8(chipWave, 0, modeSelect, 1);
AudioConnection patchCord9(chord3wave, chord3env);
AudioConnection patchCord10(chord2wave, chord2env);
AudioConnection patchCord11(modeSelect, 0, inMixer, 0);
AudioConnection patchCord12(inMixer, 0, tempDelayMixer, 1);
AudioConnection patchCord13(inMixer, 0, outMixer, 0);
AudioConnection patchCord14(chord3env, 0, chordMixer, 2);
AudioConnection patchCord15(chord2env, 0, chordMixer, 1);
AudioConnection patchCord16(inEnvelope, 0, modeSelect, 0);
AudioConnection patchCord17(inEnvelope, 0, chordMixer, 0);
AudioConnection patchCord18(tempDelayMixer, tempDelay);
AudioConnection patchCord19(chordMixer, 0, modeSelect, 2);
AudioConnection patchCord20(tempDelay, 0, tempDelayMixer, 0);
AudioConnection patchCord21(tempDelay, 0, outMixer, 2);
AudioConnection patchCord22(tempDelay, 0, fullDelayMixer, 1);
AudioConnection patchCord23(outLadderFreqSine, 0, outLadder, 1);
AudioConnection patchCord24(outMixer, 0, outLadder, 0);
AudioConnection patchCord25(fullDelayMixer, fullDelay);
AudioConnection patchCord26(outLadder, 0, i2s1, 0);
AudioConnection patchCord27(outLadder, 0, i2s1, 1);
AudioConnection patchCord28(fullDelay, 0, fullDelayMixer, 0);
AudioConnection patchCord29(fullDelay, 0, outMixer, 1);
AudioControlSGTL5000 sgtl5000_1;  // xy=64.5,20
// GUItool: end automatically generated code

// BUTTONS
// Button loopButton = Button();
// Button synthButton = Button();
// Button clearButton = Button();
Button encButton = Button();

// ENCODER
Encoder enc(ENCODER_DT_PIN, ENCODER_CLK_PIN);

// MPU6050 IMU
Adafruit_MPU6050 mpu;

// OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET_PIN);

// GLOBAL - MOTION

float accelx, accely, accelz, gyrox, gyroy, gyroz = 0;

float accelPitch;  // about y
float accelRoll;   // about x

float gyroPitch = 0;  // about y
float gyroRoll = 0;   // about x
float gyroYaw = 0;    // about z

float pitchDot = 0;
float rollDot = 0;
float yawDot = 0;

float pitch = 0;
float roll = 0;
float yaw = 0;

// GLOBAL - AUDIO

float baseFreq = 440.0;

// GLOBAL - GENERAL

elapsedMillis timer;  // master timer (auto-incrementing)

long unsigned int
    tempRecordingStart =
        0,  // timestamp when recording started (recording into temp delay)
    commitRecordingStart =
        0;  // timestamp when commit started (recording into full delay)

int mappedRibbonPotVal =
    0;  // ribbon softpot values mapped to integer half-tones above fundamental

// GLOBAL - AUDIO STATES
bool isRecordingLoop = false,  // true if recording loop
    isCommittingLoop = false,  // true if committing loop
    listeningForRoll =
        false;  // true if listening for roll angle (after recording loop)

// curSustainStatus == 0 && ribbon released -> do nothing
// curSustainStatus == 0 && ribbon pressed -> note on, set status := 1
// curSustainStatus == 1 && ribbon pressed -> sustain note
// curSustainStatus == 1 && ribbon released -> note off, set status := 0
bool curSustainStatus = false;

// GLOBAL - MENU (OLED + ENCODER)

#define NUM_MODE_OPTIONS 5
const char *MODE_OPTIONS[NUM_MODE_OPTIONS] = {"Simple", "Soft", "Chiptune",
                                              "Chord", "Percussion"};
int modeIndex = 0;
int clientMasterVolume = 100;
enum MENU_STATE {
  MENU_MODE,
  MENU_VOLUME,
  MENU_CLEAR,
  MENU_POWER,
  MENU_START_LOOP
};  // MENU_POWER, MENU_START_LOOP unused TODO
enum MENU_ACTIVE { MENU_INACTIVE, MENU_ACTIVE_OPTION };
MENU_STATE menuState = MENU_MODE;
MENU_ACTIVE menuActiveState = MENU_INACTIVE;

int encValue,       // current encoder value
    savedEncValue;  // encValue of main menu (before entering active option)

void setup(void) {
  // SERIAL SETUP
  Serial.begin(115200);
  Serial.println("Serial started");

  // AUDIO SETUP
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(MASTER_VOLUME);

  // Basic Synth
  inWaveformFM.begin(0.0, 6, WAVEFORM_SINE);
  inWaveformMod.begin(1.0, baseFreq, WAVEFORM_SINE);
  inWaveformMod.frequencyModulation(1);
  inEnvelope.attack(10.5);
  inEnvelope.hold(2.5);
  inEnvelope.decay(35);
  inEnvelope.sustain(0.8);
  inEnvelope.release(300);

  // Chiptune
  chipDC.amplitude(0);
  chipWave.begin(1.0, baseFreq, WAVEFORM_SAWTOOTH);
  chipWave.frequencyModulation(6);

  // Chords -- copy & paste from Basic Synth
  // TODO set to defines
  chord2fm.begin(0.0, 6, WAVEFORM_SINE);
  chord2wave.begin(1.0, baseFreq, WAVEFORM_SINE);
  chord2wave.frequencyModulation(1);
  chord2env.attack(10.5);
  chord2env.hold(2.5);
  chord2env.decay(35);
  chord2env.sustain(0.8);
  chord2env.release(300);

  chord3fm.begin(0.0, 6, WAVEFORM_SINE);
  chord3wave.begin(1.0, baseFreq, WAVEFORM_SINE);
  chord3wave.frequencyModulation(1);
  chord3env.attack(10.5);
  chord3env.hold(2.5);
  chord3env.decay(35);
  chord3env.sustain(0.8);
  chord3env.release(300);

  chordMixer.gain(0, 1.0);
  chordMixer.gain(1, 1.0);
  chordMixer.gain(2, 1.0);

  // Percussion
  drumSynth.frequency(60);
  drumSynth.length(1500);
  drumSynth.secondMix(0.0);
  drumSynth.pitchMod(0.55);

  // Mode Selection
  modeSelect.gain(0, 1.0);  // simple/soft
  modeSelect.gain(1, 0.0);  // chiptune
  modeSelect.gain(2, 0.0);  // chord
  modeSelect.gain(3, 0.0);  // percussion
  inMixer.gain(0, 1.0);     // mode output
  inMixer.gain(1, 1.0);     // drum synth

  // Delay Lines - Mixers Only
  tempDelayMixer.gain(0, 0.0);  // feedback
  tempDelayMixer.gain(1, 0.0);  // inMixer
  fullDelayMixer.gain(0, 1.0);  // feedback
  fullDelayMixer.gain(1, 0.0);  // temp mixer

  // Output
  outMixer.gain(0, 1.0);  // realtime dry input
  outMixer.gain(1,
                0.0);  // full delay -- set to 0.0 to avoid initial random noise
  outMixer.gain(2,
                0.0);  // temp delay -- set to 0.0 to avoid initial random noise
  outLadderFreqSine.frequency(10);
  outLadderFreqSine.amplitude(0);
  outLadder.frequency(100000);
  outLadder.octaveControl(6);

  // BUTTON SETUP
  // loopButton.attach(LOOP_BTN_PIN, INPUT_PULLUP);
  // loopButton.interval(5);
  // loopButton.setPressedState(LOW);

  // synthButton.attach(COMMIT_BTN_PIN, INPUT_PULLUP);
  // synthButton.interval(5);
  // synthButton.setPressedState(LOW);

  // clearButton.attach(CLEAR_BTN_PIN, INPUT_PULLUP);
  // clearButton.interval(5);
  // clearButton.setPressedState(LOW);

  // ENCODER SETUP
  encButton.attach(ENCODER_BTN_PIN, INPUT_PULLUP);
  encButton.interval(5);
  encButton.setPressedState(LOW);

  // MOTION SETUP
  Serial.println("Initializing MPU6050...");
  if (!mpu.begin()) {
    Serial.println("ERROR: Failed to find MPU6050 chip!");
    while (1) {
      delay(10);
    }
  }
  Serial.println("Initialized MPU6050!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_184_HZ);

  // OLED SETUP
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true) {
      delay(1);
    }
  }
  display.display();
  delay(500);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  // DELAY LINES
  tempDelay.begin(DELAY_LINE_TEMP, DELAYLINE_MAX_LEN);
  fullDelay.begin(DELAY_LINE_FULL, DELAYLINE_MAX_LEN);
  tempDelay.delay(0, LOOP_TIME);  // must come after tempDelay.begin!
  fullDelay.delay(0, LOOP_TIME);  // must come after fullDelay.begin!

  // clear delay lines -- avoid initial random noise
  tempDelay.clear();
  fullDelay.clear();

  Serial.println("Finished setup.");
  delay(100);
}

void loop() {
  // Read motion data
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  accelx = -(a.acceleration.y - ACCELY_OFF) / ACCELY_SCALE;
  accely = -(a.acceleration.z - ACCELZ_OFF) / ACCELZ_SCALE;
  accelz = -(a.acceleration.x - ACCELX_OFF) / ACCELX_SCALE;

  gyrox = (g.gyro.y - GYROY_OFF);
  gyroy = (g.gyro.z - GYROZ_OFF);
  gyroz = (g.gyro.x - GYROX_OFF);

  pitchDot = gyroy * cos(roll) - gyroz * sin(roll);
  yawDot = (gyroy * sin(roll) + gyroz * cos(roll)) / cos(pitch);
  rollDot = gyrox + sin(pitch) * yawDot;

  // gyroPitch = gyroPitch + gyroy * 0.01;
  // gyroRoll =  gyroRoll + gyrox * 0.01;
  // gyroYaw =  gyroYaw + gyroz * 0.01;

  accelPitch = atan2(-accelx, sqrt(accely * accely + accelz * accelz));
  accelRoll = atan2(accely, accelz);

  pitch = MOTION_ALPHA * (pitch + pitchDot * 0.01) +
          (1 - MOTION_ALPHA) * accelPitch;  // final pitch value
  roll = MOTION_ALPHA * (roll + rollDot * 0.01) +
         (1 - MOTION_ALPHA) * accelRoll;  // final roll value

  // ladder1.frequency(abs(pitch * 1000 + 500));
  // outLadderFreqSine.amplitude(roll / 10);

  // RIBBON POT (PITCH)
  ribbonPotCode();

  // Read button data
  // loopButton.update();
  // synthButton.update();
  // clearButton.update();

  // START RECORDING
  // if (loopButton.pressed() && !isRecordingLoop && !isCommittingLoop) {
  //   // to prevent race conditions, do not allow recording a loop if in the
  //   // middle of committing a loop
  //   Serial.println("Loop button pressed");
  //   // send the synth signal into the delay loop
  //   tempDelayMixer.gain(1, 1.0);
  //   // comment this line if you don't want to clear the loop when you record
  //   // new material
  //   tempDelayMixer.gain(0, 0.0);

  //   outMixer.gain(2, 0.0);  // disable temp delay

  //   tempRecordingStart = timer;
  //   isRecordingLoop = true;
  // }

  // FINISH RECORDING
  if (timer >= (tempRecordingStart + LOOP_TIME) && isRecordingLoop) {
    Serial.println("Recording stopped");
    // turn off the signal into the delay loop
    tempDelayMixer.gain(1, 0.0);
    // resets the feedback to 1.0 so the loop repeats indefinitely
    tempDelayMixer.gain(0, 1.0);
    isRecordingLoop = false;

    outMixer.gain(2, 1.0);  // enable temp delay
    listeningForRoll = true;
  }

  // START COMMITTING LAYER
  // if (synthButton.pressed() && !isCommittingLoop && !isRecordingLoop) {
  if (listeningForRoll) {
    if (roll < -1.0) {
      Serial.println("SAVE roll detected");
      listeningForRoll = false;
      // enable temp delay signal into full delay
      fullDelayMixer.gain(1, 1.0);  // TODO fix double timbre glitch

      commitRecordingStart = timer;
      isCommittingLoop = true;
    }

    if (roll > 1.0) {
      Serial.println("DISCARD roll detected");
      listeningForRoll = false;
      tempDelay.clear();
    }
  }

  // FINISH COMMITTING LAYER
  if (timer >= (commitRecordingStart + LOOP_TIME) && isCommittingLoop) {
    Serial.println("Commit recording stopped");
    // disable temp delay signal into full delay
    fullDelayMixer.gain(1, 0.0);

    // disable temp delay signal feedback
    tempDelayMixer.gain(0, 0.0);

    // resets the feedback to 1.0 so the loop repeats indefinitely
    // fullDelayMixer.gain(0, 1.0); // already always 1.0 for full delay!
    isCommittingLoop = false;

    outMixer.gain(1, 1.0);  // full delay
  }

  // CLEAR FULL DELAY
  // if (clearButton.pressed() && !isCommittingLoop) {
  //   Serial.println("Clear button pressed");
  //   // clear the loop
  //   fullDelay.clear();
  // }

  // VIBRATO
  // Serial.println(pitch);
  inWaveformFM.amplitude(mapFloat(abs(pitch), 0, 0.5, 0, 0.1));

  if (abs(accelz) > 17.0) {
    // a shake up/down usually goes up to 19.0
    // TODO (optional) change to change in accel
    Serial.println(accelz);
    drumSynth.noteOn();
  }

  // MENU
  menuCode();

  delay(10);  // prevents ramp down bug, switch to timer instead of delay later
}

// RIBBON POT CODE

void ribbonPotCode() {
  mappedRibbonPotVal = getRibbonPotValAndMap(0, 12);
  // Serial.println(mappedRibbonPotVal);

  if (mappedRibbonPotVal > -1) {
    // ribbon pressed
    inWaveformMod.frequency(baseFreq * pow(2, mappedRibbonPotVal / 12.0));

    if (curSustainStatus) {
      // sustain note
    } else {
      // note on
      inEnvelope.noteOn();
      curSustainStatus = true;
    }
  } else {
    // inWaveformMod.amplitude(0);
    if (curSustainStatus) {
      // note off
      inEnvelope.noteOff();
      curSustainStatus = false;
    }
  }
}

/**
 * @brief get the ribbon pot val and map it to a new range if being pressed
 *
 * @param newMin min of new range, output will never be less than this
 * @param newMax max of new range, output will never be more than this
 * @return (int) -1 if ribbon pot is not pressed, otherwise a value between
 * newMin and newMax
 */
int getRibbonPotValAndMap(int newMin, int newMax) {
  float a1 = analogRead(RIBBON_POT_PIN_1);  // sensorValue2
  float a2 = analogRead(RIBBON_POT_PIN_2);  // sensorValue1

  if (a1 < 10 || a2 < 10) {
    return -1;
  }

  // calibrate by measuring actual resistance of 10k resistor (e.g. 9.93 or 9.8)
  float x = ((1023 / (float)a1) - 1) * (10 / 9.78);
  float y = ((1023 / (float)a2) - 1) * (10 / 9.81);

  float pos = (x + (1 - y)) / 2;  // normalized position (from 0.0 to 1.0)
  pos = (newMax - newMin) * pos + newMin;  // mapped position

  return (int)pos;
}

// MENU CODE

void menuCode() {
  // Encoder: Menu Navigation
  int encValue = enc.read();
  if (menuActiveState == MENU_INACTIVE) {
    // encoder toggles through menu options
    // TODO switch to programmatic way (non if statements)
    if (encValue >= 0 && encValue < MES) {
      menuState = MENU_START_LOOP;  // TODO replace with IMU
    } else if (encValue >= MES && encValue < 2 * MES) {
      menuState = MENU_CLEAR;
    } else if (encValue >= 2 * MES && encValue < 3 * MES) {
      menuState = MENU_MODE;
    } else if (encValue >= 3 * MES && encValue < 4 * MES) {
      menuState = MENU_VOLUME;
      // menuState = MENU_POWER;
    } else if (encValue < 0) {
      // TODO switch to programmatic way
      enc.write(39);
    } else {
      enc.write(0);
    }
  } else if (menuActiveState == MENU_ACTIVE_OPTION) {
    if (menuState == MENU_MODE) {
      // encoder toggles through mode options
      if (encValue >= MES) {
        modeIndex = (modeIndex + 1) % NUM_MODE_OPTIONS;
        enc.write(0);
      } else if (encValue <= -MES) {
        modeIndex = (modeIndex + NUM_MODE_OPTIONS - 1) % NUM_MODE_OPTIONS;
        enc.write(0);
      }
    }
    if (menuState == MENU_VOLUME) {
      // encoder adjusts volume
      clientMasterVolume += (int)(encValue / (int)(MES / 5));
      if (clientMasterVolume < 0) {
        clientMasterVolume = 0;
      } else if (clientMasterVolume > 100) {
        clientMasterVolume = 100;
      }

      // set volume
      for (int i = 0; i < 4; i++) {
        outMixer.gain(i, clientMasterVolume / 100.0);
      }

      enc.write(0);
    }
  }

  // Encoder Button: Change Active State
  encButton.update();
  if (encButton.pressed()) {
    Serial.println("Encoder button pressed");
    if (menuActiveState == MENU_INACTIVE) {
      // change to active option state
      if (menuState == MENU_START_LOOP || menuState == MENU_CLEAR) {
        // substate-less menu options
        if (menuState == MENU_CLEAR) {
          // clearLoop();
        }

        // TODO remove MENU_START_LOOP later
        if (menuState == MENU_START_LOOP) {
          // startLoop();
        }
      } else {
        savedEncValue = encValue;
        enc.write(0);
        menuActiveState = MENU_ACTIVE_OPTION;
      }
    } else {
      // change to inactive (main menu) state
      encValue = savedEncValue;
      enc.write(encValue);
      menuActiveState = MENU_INACTIVE;
    }
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  // display.println(encValue);

  // Start Loop Recording
  // TODO remove later
  displayMenuPrefix(MENU_START_LOOP);
  display.println("Start Loop");

  // Clear Loop
  displayMenuPrefix(MENU_CLEAR);
  display.println("Clear Loop");

  // Mode
  displayMenuPrefix(MENU_MODE);
  display.println(MODE_OPTIONS[modeIndex]);

  // Volume
  displayMenuPrefix(MENU_VOLUME);
  display.print("Volume: ");
  display.println(clientMasterVolume);

  display.display();
}

void displayMenuPrefix(int ms) {
  if (menuState == ms) {
    display.print(">");
  } else {
    display.print(" ");
  }
  if (menuState == ms && menuActiveState == MENU_ACTIVE_OPTION) {
    display.print(">");
  } else {
    display.print(" ");
  }
  display.print(" ");
}

// HELPER FUNCTIONS

// https://forum.arduino.cc/t/arduino-map-function-for-float-values/112888/2
/**
 * @brief mapFloat (no automatic clipping, output can exceed out_min/out_max)
 */
float mapFloat(float x, float in_min, float in_max, float out_min,
               float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

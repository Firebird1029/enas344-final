#include <Adafruit_MPU6050.h>
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

// PINOUT

#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 3
#define RIBBON_POT_PIN_1 41  // A17
#define RIBBON_POT_PIN_2 40  // A16
#define LOOP_BTN_PIN 32
#define SYNTH_BTN_PIN 31

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

// AUDIO & GENERAL

#define LOOP_TIME 2000
#define MASTER_VOLUME 0.5

#define DELAYLINE_MAX_LEN 441000  // 44100 samples/sec * 10 sec

// AUDIO
EXTMEM int16_t DELAY_LINE_TEMP[DELAYLINE_MAX_LEN] = {};
EXTMEM int16_t DELAY_LINE_FULL[DELAYLINE_MAX_LEN] = {};

// CUSTOM OVERRIDES
AudioEffectdelayLoop tempDelay;  // xy=607,182
AudioEffectdelayLoop fullDelay;  // xy=868,201

// GUItool: begin automatically generated code
AudioSynthSimpleDrum drum1;                 // xy=55,176
AudioSynthWaveform inWaveformFM;            // xy=77,58
AudioSynthWaveformModulated inWaveformMod;  // xy=120,103
AudioEffectEnvelope inEnvelope;             // xy=282,102
AudioMixer4 tempDelayMixer;                 // xy=457,302
// AudioEffectDelay tempDelay;                 // xy=607,182
AudioSynthWaveformSine outLadderFreqSine;  // xy=681,89
AudioMixer4 outMixer;                      // xy=712,35
AudioMixer4 fullDelayMixer;                // xy=735,410
// AudioEffectDelay fullDelay;                 // xy=868,201
AudioFilterLadder outLadder;  // xy=880,30
AudioOutputI2S i2s1;          // xy=1071,29
AudioConnection patchCord1(drum1, 0, tempDelayMixer, 2);
AudioConnection patchCord2(inWaveformFM, 0, inWaveformMod, 0);
AudioConnection patchCord3(inWaveformMod, inEnvelope);
AudioConnection patchCord4(inEnvelope, 0, tempDelayMixer, 1);
AudioConnection patchCord5(inEnvelope, 0, outMixer, 0);
AudioConnection patchCord6(tempDelayMixer, tempDelay);
AudioConnection patchCord7(tempDelayMixer, 0, fullDelayMixer, 1);
AudioConnection patchCord8(tempDelay, 0, tempDelayMixer, 0);
AudioConnection patchCord9(tempDelay, 0, outMixer, 2);
AudioConnection patchCord10(outLadderFreqSine, 0, outLadder, 1);
AudioConnection patchCord11(outMixer, 0, outLadder, 0);
AudioConnection patchCord12(fullDelayMixer, fullDelay);
AudioConnection patchCord13(fullDelay, 0, fullDelayMixer, 0);
AudioConnection patchCord14(fullDelay, 0, outMixer, 1);
AudioConnection patchCord15(outLadder, 0, i2s1, 0);
AudioConnection patchCord16(outLadder, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;  // xy=64.5,20
// GUItool: end automatically generated code

// ENCODER
Encoder enc(ENCODER_A_PIN, ENCODER_B_PIN);

// BUTTONS
Button loopButton = Button();
Button synthButton = Button();

// MPU6050
Adafruit_MPU6050 mpu;

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

elapsedMillis timer;
long unsigned int tempRecordingStart;
int mappedRibbonPotVal = 0;
bool isRecordingLoop = false;

// curSustainStatus == 0 && ribbon released -> do nothing
// curSustainStatus == 0 && ribbon pressed -> note on, set status := 1
// curSustainStatus == 1 && ribbon pressed -> sustain note
// curSustainStatus == 1 && ribbon released -> note off, set status := 0
bool curSustainStatus = false;

void setup(void) {
  // SERIAL SETUP
  Serial.begin(115200);
  Serial.println("Serial started");

  // AUDIO SETUP
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(MASTER_VOLUME);

  inWaveformFM.begin(0, baseFreq, WAVEFORM_SINE);
  inWaveformMod.begin(1.0, baseFreq, WAVEFORM_SAWTOOTH);
  inWaveformMod.frequencyModulation(1);
  inEnvelope.attack(10.5);
  inEnvelope.hold(2.5);
  inEnvelope.decay(35);
  inEnvelope.sustain(0.8);
  inEnvelope.release(300);

  tempDelayMixer.gain(0, 0.0);  // feedback
  tempDelayMixer.gain(1, 0.0);  // ribbon TODO change to inMixer
  tempDelayMixer.gain(2, 0.0);  // drum TODO remove

  outLadderFreqSine.frequency(10);
  outLadderFreqSine.amplitude(0);

  outMixer.gain(0, 1.0);  // realtime dry input
  outMixer.gain(1, 1.0);  // full delay
  outMixer.gain(2, 1.0);  // temp delay

  fullDelayMixer.gain(0, 0.0);  // feedback
  fullDelayMixer.gain(1, 0.0);  // temp mixer

  fullDelay.delay(0, LOOP_TIME);

  // delay lines
  tempDelay.begin(DELAY_LINE_TEMP, DELAYLINE_MAX_LEN);
  fullDelay.begin(DELAY_LINE_FULL, DELAYLINE_MAX_LEN);
  tempDelay.delay(0, LOOP_TIME);  // must come after tempDelay.begin!
  fullDelay.delay(0, LOOP_TIME);  // must come after fullDelay.begin!

  outLadder.frequency(100000);
  outLadder.octaveControl(6);

  // BUTTON SETUP
  loopButton.attach(LOOP_BTN_PIN, INPUT_PULLUP);
  loopButton.interval(5);
  loopButton.setPressedState(LOW);

  synthButton.attach(SYNTH_BTN_PIN, INPUT_PULLUP);
  synthButton.interval(5);
  synthButton.setPressedState(LOW);

  // MOTION SETUP
  // TODO
  /* Serial.println("Initializing MPU6050...");
  if (!mpu.begin()) {
    Serial.println("ERROR: Failed to find MPU6050 chip!");
    while (1) {
      delay(10);
    }
  }
  Serial.println("Initialized MPU6050!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_184_HZ); */

  Serial.println("Finished setup.");
  delay(100);
}

void loop() {
  // Read motion data
  // TODO
  /* sensors_event_t a, g, temp;
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
  // outLadderFreqSine.amplitude(roll / 10); */

  // Read encoder data
  // Serial.println(enc.read());
  // inWaveformMod.frequency(baseFreq + enc.read());

  // Read ribbon pot data
  mappedRibbonPotVal = getRibbonPotValAndMap(0, 12);
  // Serial.println(mappedRibbonPotVal);

  if (mappedRibbonPotVal > -1) {
    // ribbon pressed
    inWaveformMod.frequency(baseFreq * pow(2, mappedRibbonPotVal / 12.0));

    if (curSustainStatus) {
      // sustain note
    } else {
      // note on
      Serial.println("note on");
      inEnvelope.noteOn();
      curSustainStatus = true;
    }
  } else {
    // inWaveformMod.amplitude(0);
    if (curSustainStatus) {
      // note off
      Serial.println("note off");
      inEnvelope.noteOff();
      curSustainStatus = false;
    }
  }

  // Read button data
  loopButton.update();
  synthButton.update();

  // Recording functionality
  if (loopButton.pressed()) {
    Serial.println("Loop button pressed");
    if (!isRecordingLoop) {
      // send the synth signal into the delay loop
      tempDelayMixer.gain(1, 1.0);
      // comment this line if you don't want to clear the loop when you record
      // new material
      tempDelayMixer.gain(0, 0.0);
      tempRecordingStart = timer;
      isRecordingLoop = true;
    }
  }

  if (timer >= (tempRecordingStart + LOOP_TIME) && isRecordingLoop) {
    Serial.println("Recording stopped");
    // turn off the signal into the delay loop
    tempDelayMixer.gain(1, 0.0);
    // resets the feedback to 1.0 so the loop repeats indefinitely
    tempDelayMixer.gain(0, 1.0);
    isRecordingLoop = false;
  }

  delay(10);  // prevents ramp down bug, switch to timer instead of delay later
}

// HELPER FUNCTIONS

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
  float x = ((1023 / (float)a1) - 1) * (9.93 / 9.39);
  float y = ((1023 / (float)a2) - 1) * (9.8 / 9.39);

  float pos = (x + (1 - y)) / 2;  // normalized position (from 0.0 to 1.0)
  pos = (newMax - newMin) * pos + newMin;  // mapped position

  return (int)pos;
}

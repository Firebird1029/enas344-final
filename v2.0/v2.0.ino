#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Audio.h>
#include <Encoder.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Wire.h>

#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 3
#define RIBBON_POT_PIN 41  // A17

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

// GUItool: begin automatically generated code
AudioSynthWaveform waveform1;   // xy=113,188
AudioSynthWaveformSine sine2;   // xy=144,432
AudioEffectEnvelope envelope1;  // xy=337,189
AudioFilterLadder ladder1;      // xy=697,296
AudioOutputI2S i2s1;            // xy=895,129
AudioConnection patchCord1(waveform1, envelope1);
AudioConnection patchCord2(sine2, 0, ladder1, 1);
AudioConnection patchCord3(envelope1, 0, ladder1, 0);
AudioConnection patchCord4(ladder1, 0, i2s1, 0);
AudioConnection patchCord5(ladder1, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;  // xy=77,67
// GUItool: end automatically generated code

// AUDIO - GLOBAL VARS

float baseFreq = 440.0;
float ampScale = 0.5;

// ENCODER - PITCH
Encoder enc(ENCODER_A_PIN, ENCODER_B_PIN);

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

// GLOBAL - GENERAL

long int timer = 0;
int ribbonPotVal = 0, mappedRibbonPotVal = 0;

// curSustainStatus == 0 && ribbon released -> do nothing
// curSustainStatus == 0 && ribbon pressed -> note on, set status := 1
// curSustainStatus == 1 && ribbon pressed -> sustain note
// curSustainStatus == 1 && ribbon released -> note off, set status := 0
bool curSustainStatus = false;

void setup(void) {
  // SERIAL SETUP
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }
  Serial.println("Serial started");

  // AUDIO SETUP
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  waveform1.begin(ampScale, baseFreq, WAVEFORM_SAWTOOTH);
  waveform1.amplitude(1.0);
  sine2.frequency(10);
  sine2.amplitude(0);
  ladder1.frequency(5000);
  ladder1.octaveControl(10.0);

  // MOTION SETUP
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

  // TODO make dynamic
  ladder1.frequency(100000);

  Serial.println("Finished setup.");
  delay(100);
}

void loop() {
  timer = micros();

  // Read motion data
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
  // sine2.amplitude(roll / 10); */

  // Read encoder data
  // Serial.println(enc.read());
  // waveform1.frequency(baseFreq + enc.read());

  // Read ribbon pot data
  ribbonPotVal = analogRead(RIBBON_POT_PIN);
  // Serial.println(ribbonPotVal);
  mappedRibbonPotVal = getRibbonPotValAndMap(ribbonPotVal, 420, 740, 0, 12);
  // Serial.println(mappedRibbonPotVal);

  if (mappedRibbonPotVal > -1) {
    // ribbon pressed
    waveform1.frequency(baseFreq * pow(2, mappedRibbonPotVal / 12.0));

    if (curSustainStatus) {
      // sustain note
    } else {
      // note on
      envelope1.noteOn();
      curSustainStatus = true;
    }
  } else {
    // waveform1.amplitude(0);
    if (curSustainStatus) {
      // note off
      envelope1.noteOff();
      curSustainStatus = false;
    }
  }
}

// HELPER FUNCTIONS

/**
 * @brief get the ribbon pot val and map it to a new range if being pressed
 *
 * @param potVal the ribbon pot reading
 * @param min min of input value to map from
 * @param max max of input value to map from
 * @param newMin min of new range, output will never be less than this
 * @param newMax max of new range, output will never be more than this
 * @return (int) -1 if ribbon pot is not pressed, otherwise a value between
 * newMin and newMax
 */
int getRibbonPotValAndMap(int potVal, int min, int max, int newMin,
                          int newMax) {
  // check if ribbon pot is being pressed, if not return -1
  if (potVal > 1000 || potVal < 100) {
    return -1;
  }

  // clip potVal between min and max
  if (potVal < min) {
    potVal = min;
  } else if (potVal > max) {
    potVal = max;
  }

  // finally, map values
  potVal = map(potVal, min, max, newMin, newMax);

  return potVal;
}

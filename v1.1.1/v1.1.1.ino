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
#define RIBBON_POT_PIN 14  // A0

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
AudioSynthWaveformSine sine2;  // xy=147,199
AudioSynthWaveform waveform1;  // xy=159,141
AudioFilterLadder ladder1;     // xy=477,173
AudioOutputI2S i2s1;           // xy=764,174
AudioConnection patchCord1(sine2, 0, ladder1, 1);
AudioConnection patchCord2(waveform1, 0, ladder1, 0);
AudioConnection patchCord3(ladder1, 0, i2s1, 0);
AudioConnection patchCord4(ladder1, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;  // xy=154,80
// GUItool: end automatically generated code

// AUDIO - GLOBAL VARS

float base_freq = 440;
float amp_scale = 0.5;

// ENCODER - PITCH
Encoder enc(ENCODER_A_PIN, ENCODER_B_PIN);

// MPU6050
Adafruit_MPU6050 mpu;

// GLOBAL - MOTION

float accelx, accely, accelz, gyrox, gyroy, gyroz = 0;

float accel_pitch;  // about y
float accel_roll;   // about x

float gyro_pitch = 0;  // about y
float gyro_roll = 0;   // about x
float gyro_yaw = 0;    // about z

float pitch_dot = 0;
float roll_dot = 0;
float yaw_dot = 0;

float pitch = 0;
float roll = 0;
float yaw = 0;

// GLOBAL - GENERAL

long int timer = 0;
int ribbonPotVal = 0, mappedRibbonPotVal = 0;

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

  waveform1.begin(amp_scale, base_freq, WAVEFORM_SAWTOOTH);
  sine2.frequency(10);
  sine2.amplitude(0);
  ladder1.frequency(5000);
  ladder1.octaveControl(10.0);

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

  delay(100);

  // TODO make dynamic
  ladder1.frequency(100000);
}

void loop() {
  timer = micros();

  // Read motion data
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  accelx = -(a.acceleration.y - ACCELY_OFF) / ACCELY_SCALE;
  accely = -(a.acceleration.z - ACCELZ_OFF) / ACCELZ_SCALE;
  accelz = -(a.acceleration.x - ACCELX_OFF) / ACCELX_SCALE;

  gyrox = (g.gyro.y - GYROY_OFF);
  gyroy = (g.gyro.z - GYROZ_OFF);
  gyroz = (g.gyro.x - GYROX_OFF);

  pitch_dot = gyroy * cos(roll) - gyroz * sin(roll);
  yaw_dot = (gyroy * sin(roll) + gyroz * cos(roll)) / cos(pitch);
  roll_dot = gyrox + sin(pitch) * yaw_dot;

  // gyro_pitch = gyro_pitch + gyroy * 0.01;
  // gyro_roll =  gyro_roll + gyrox * 0.01;
  // gyro_yaw =  gyro_yaw + gyroz * 0.01;

  accel_pitch = atan2(-accelx, sqrt(accely * accely + accelz * accelz));
  accel_roll = atan2(accely, accelz);

  pitch = MOTION_ALPHA * (pitch + pitch_dot * 0.01) +
          (1 - MOTION_ALPHA) * accel_pitch;  // final pitch value
  roll = MOTION_ALPHA * (roll + roll_dot * 0.01) +
         (1 - MOTION_ALPHA) * accel_roll;  // final roll value

  // ladder1.frequency(abs(pitch * 1000 + 500));
  // sine2.amplitude(roll / 10);

  // Read encoder data
  // Serial.println(enc.read());
  // waveform1.frequency(base_freq + enc.read());

  // Read ribbon pot data
  ribbonPotVal = analogRead(RIBBON_POT_PIN);
  // Serial.println(ribbonPotVal);
  mappedRibbonPotVal = getRibbonPotValAndMap(ribbonPotVal, 420, 740, 0, 12);

  if (mappedRibbonPotVal > -1) {
    waveform1.frequency(base_freq * pow(2, ribbonPotVal / 12.0));
    waveform1.amplitude(1.0);
  } else {
    waveform1.amplitude(0);
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

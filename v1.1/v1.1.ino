#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Audio.h>
#include <Encoder.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Wire.h>

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
AudioSynthWaveformSine sine1;  // xy=250,275
AudioSynthWaveformSine sine2;  // xy=254,499
AudioSynthWaveform waveform1;  // xy=264,435
AudioFilterLadder ladder1;     // xy=487,434
AudioOutputI2S i2s1;           // xy=693,299
AudioConnection patchCord1(sine2, 0, ladder1, 1);
AudioConnection patchCord2(waveform1, 0, ladder1, 0);
AudioConnection patchCord3(ladder1, 0, i2s1, 0);
AudioConnection patchCord4(ladder1, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;  // xy=126,64
// GUItool: end automatically generated code

// AUDIO - GLOBAL VARS

float base_freq = 220;
float amp_scale = 0.5;

// ENCODER - PITCH
Encoder enc(2, 3);

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

  // sine1.frequency(base_freq);
  // sine1.amplitude(amp_scale);
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

  // Get default MPU values
  delay(100);
  // sensors_event_t a, g, temp;
  // mpu.getEvent(&a, &g, &temp);

  // accelx = -(a.acceleration.y - ACCELY_OFF) / ACCELY_SCALE;
  // accely = -(a.acceleration.z - ACCELZ_OFF) / ACCELZ_SCALE;
  // accelz = -(a.acceleration.x - ACCELX_OFF) / ACCELX_SCALE;

  // gyrox = (g.gyro.y - GYROY_OFF);
  // gyroy = (g.gyro.z - GYROZ_OFF);
  // gyroz = (g.gyro.x - GYROX_OFF);

  // accel_pitch = atan2(-accelx, sqrt(accely * accely + accelz * accelz));
  // accel_roll = atan2(accely, accelz);

  // pitch = accel_pitch;
  // roll = accel_roll;
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

  /* //Serial.print("pitch  ");
  Serial.print(degrees(pitch));
  //Serial.print("  roll  ");
  Serial.print(degrees(roll)); */

  // Serial.println(enc.read());
  waveform1.frequency(base_freq + enc.read());

  ladder1.frequency(abs(pitch * 1000 + 500));
  sine2.amplitude(roll / 10);
}

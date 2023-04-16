#include <Audio.h>
#include <Encoder.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Wire.h>

#include "GY521.h"

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

Encoder enc(2, 3);

GY521 sensor(0x68);

float base_freq = 220;
float amp_scale = 0.5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("ready");

  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  // sine1.frequency(base_freq);
  // sine1.amplitude(amp_scale);
  waveform1.begin(amp_scale, base_freq, WAVEFORM_SAWTOOTH);
  sine2.frequency(10);
  sine2.amplitude(0);
  ladder1.frequency(5000);
  ladder1.octaveControl(2.0);

  Wire.begin();

  delay(100);
  while (sensor.wakeup() == false) {
    Serial.print(millis());
    Serial.println("\tCould not connect to GY521");
    delay(1000);
  }
  sensor.setAccelSensitivity(2);  // 8g
  sensor.setGyroSensitivity(1);   // 500 degrees/s

  sensor.setThrottle();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(enc.read());
  waveform1.frequency(base_freq + enc.read());

  sensor.read();
  float pitch = sensor.getPitch();
  float roll = sensor.getRoll();
  // float yaw   = sensor.getYaw();

  ladder1.frequency(pitch * 10 + 500);
  sine2.amplitude(roll / 365.0);

  delay(100);
}

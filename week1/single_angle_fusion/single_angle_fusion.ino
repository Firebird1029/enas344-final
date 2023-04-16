// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float accelx,accely,accelz,gyrox,gyroy,gyroz = 0;
float accelx_off = 0.295;
float accely_off = 0.08; 
float accelz_off = -0.2; 

float gyrox_off = -0.1197; 
float gyroy_off = -0.0157; 
float gyroz_off = -0.0190;

float accelx_scale = 1.008;
float accely_scale = 1.00;
float accelz_scale = 0.98;


float gyrox_scale = 1;
float gyroy_scale = 1;
float gyroz_scale = 1;

float accel_pitch; // about y
float accel_roll; // about x

float gyro_pitch = 0; // about y
float gyro_roll = 0; // about x

float pitch_dot = 0;
float roll_dot = 0;
float yaw_dot = 0;

float pitch = 0;
float roll = 0;
float yaw = 0;

float alpha = 0.99;

long int timer = 0;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);// 2,4,8,16
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);// 250,500,1000,2000
  mpu.setFilterBandwidth(MPU6050_BAND_184_HZ); //260,184,94,44,21,10,5

  delay(100);
}

void loop() {
timer = micros();
  /* Get new sensor events with the readings */
sensors_event_t a, g, temp;
mpu.getEvent(&a, &g, &temp);

//accelx =  (a.acceleration.x - accelx_off)/accelx_scale;
accely =  (a.acceleration.y - accely_off)/accely_scale;
accelz =  (a.acceleration.z - accelz_off)/accelz_scale;

gyrox =  (g.gyro.x - gyrox_off);
//gyroy =  (g.gyro.y - gyroy_off);
//gyroz =  (g.gyro.z - gyroz_off);

accel_roll =   atan2(accely,accelz);
gyro_roll = gyro_roll + gyrox * 0.01;
roll = alpha * (roll + gyrox * 0.01) + (1-alpha) * accel_roll;

Serial.print( degrees(gyro_roll) );
Serial.print("  ");
Serial.print( degrees(accel_roll) );
Serial.print("  ");
Serial.println( degrees(roll) );

while((micros()-timer)<10000){ }// this delays for the remainder of the time up to 10ms
  
}

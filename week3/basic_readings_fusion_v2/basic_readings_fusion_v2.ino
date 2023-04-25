// Basic demo for accelerometer readings from Adafruit MPU6050
// this works for x = North, y = EAST, z = DOWN
// thia would be as if the chip were stuck onto the bottom of 
//the shoe with x pointing forward

// to stick on top of shoe, with x forward, one option would be to solder 
//pins coming out from board on other side, also leaving a flatter attachment
//surface

// NOTE: This was changed on April 25, 2023 from accel_roll =   atan2(accely,accelz); to accel_roll =   atan2(-accely,-accelz); because it was upside down.

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
float gyro_yaw = 0; // about z

float pitch_dot = 0;
float roll_dot = 0;
float yaw_dot = 0;

float pitch = 0;
float roll = 0;
float yaw = 0;

float alpha = 0.5;

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

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_184_HZ);
delay(100);
sensors_event_t a, g, temp;
mpu.getEvent(&a, &g, &temp);

accelx =  -(a.acceleration.x - accelx_off)/accelx_scale;
accely =  -(a.acceleration.y - accely_off)/accely_scale;
accelz =  -(a.acceleration.z - accelz_off)/accelz_scale;

gyrox =  (g.gyro.x - gyrox_off);
gyroy =  (g.gyro.y - gyroy_off);
gyroz =  (g.gyro.z - gyroz_off);

accel_pitch = atan2(-accelx,sqrt(accely*accely + accelz*accelz));
accel_roll =   atan2(-accely,-accelz);

pitch = accel_pitch;
roll = accel_roll;
  
  delay(100);
}

void loop() {
timer = micros();
  /* Get new sensor events with the readings */
sensors_event_t a, g, temp;
mpu.getEvent(&a, &g, &temp);

accelx =  -(a.acceleration.x - accelx_off)/accelx_scale;
accely =  -(a.acceleration.y - accely_off)/accely_scale;
accelz =  -(a.acceleration.z - accelz_off)/accelz_scale;

gyrox =  (g.gyro.x - gyrox_off);
gyroy =  (g.gyro.y - gyroy_off);
gyroz =  (g.gyro.z - gyroz_off);

pitch_dot = gyroy * cos(roll) - gyroz * sin(roll);
yaw_dot = (  gyroy * sin(roll) + gyroz * cos(roll)  )/cos(pitch);
roll_dot = gyrox + sin(pitch) * yaw_dot;

//gyro_pitch = gyro_pitch + gyroy * 0.01;
//gyro_roll =  gyro_roll + gyrox * 0.01;
//gyro_yaw =  gyro_yaw + gyroz * 0.01;

accel_pitch = atan2(-accelx,sqrt(accely*accely + accelz*accelz));
accel_roll =   atan2(-accely,-accelz);

pitch = alpha * (pitch + pitch_dot * 0.01) + (1-alpha) * accel_pitch;
roll = alpha * (roll + roll_dot * 0.01) + (1-alpha) * accel_roll;

  /* Print out the values */
//  Serial.print("Acceleration X: ");
//  Serial.print(accelx);
//  Serial.print(", Y: ");
//  Serial.print(accely);
//  Serial.print(", Z: ");
//  Serial.print(accelz);
//  Serial.println(" m/s^2");
//
//  Serial.print("Rotation X: ");
//  Serial.print(gyrox);
//  Serial.print(", Y: ");
//  Serial.print(gyroy);
//  Serial.print(", Z: ");
//  Serial.print(gyroz);
//  Serial.println(" rad/s");

// Serial.print(degrees(accel_pitch));
// Serial.print("    ");
//Serial.print("  roll  ");
// Serial.print( degrees(accel_roll));
// Serial.print("    ");
//Serial.print(degrees(gyro_pitch));
//Serial.print("    ");
////Serial.print("  roll  ");
//Serial.print( degrees(gyro_roll));
//Serial.print("    ");
//Serial.print( degrees(gyro_yaw));
//Serial.print("    ");
////Serial.print("pitch  ");
Serial.print(degrees(pitch)); 
Serial.print("    ");
////Serial.print("  roll  ");
Serial.print( degrees(roll));

Serial.println();
//  Serial.println(millis());
// while((micros()-timer)<10000){ // this delays for the remainder of the time up to 10ms
//   }
delay(10);
}

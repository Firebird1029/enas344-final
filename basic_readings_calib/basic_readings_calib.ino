// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float accelx,accely,accelz,gyrox,gyroy,gyroz = 0;
// float accelx_off = 0.35;
// float accely_off = 0.24; 
// float accelz_off = -.03; 

float accelx_off = 0.295;
float accely_off = 0.08; 
float accelz_off = -0.2; 

// float gyrox_off = -0.0420; 
// float gyroy_off = +0.0164; 
// float gyroz_off = -.0050;

float gyrox_off = -0.1197; 
float gyroy_off = -0.0157; 
float gyroz_off = -0.0190;

// float accelx_scale = 0.998;
// float accely_scale = 1;
// float accelz_scale = 1.018;

float accelx_scale = 1.008;
float accely_scale = 1.00;
float accelz_scale = 0.98;

float gyrox_scale = 1;
float gyroy_scale = 1;
float gyroz_scale = 1;

float accel_pitch; // about y
float accel_roll; // about x

float gyro_pitch; // about y
float gyro_roll; // about x
float temperature = 0;
float N_ave = 100;

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
//  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
//  Serial.print("Accelerometer range set to: ");
//  switch (mpu.getAccelerometerRange()) {
//  case MPU6050_RANGE_2_G:
//    Serial.println("+-2G");
//    break;
//  case MPU6050_RANGE_4_G:
//    Serial.println("+-4G");
//    break;
//  case MPU6050_RANGE_8_G:
//    Serial.println("+-8G");
//    break;
//  case MPU6050_RANGE_16_G:
//    Serial.println("+-16G");
//    break;
//  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
//  Serial.print("Gyro range set to: ");
//  switch (mpu.getGyroRange()) {
//  case MPU6050_RANGE_250_DEG:
//    Serial.println("+- 250 deg/s");
//    break;
//  case MPU6050_RANGE_500_DEG:
//    Serial.println("+- 500 deg/s");
//    break;
//  case MPU6050_RANGE_1000_DEG:
//    Serial.println("+- 1000 deg/s");
//    break;
//  case MPU6050_RANGE_2000_DEG:
//    Serial.println("+- 2000 deg/s");
//    break;
//  }

  mpu.setFilterBandwidth(MPU6050_BAND_184_HZ);
//  Serial.print("Filter bandwidth set to: ");
//  switch (mpu.getFilterBandwidth()) {
//  case MPU6050_BAND_260_HZ:
//    Serial.println("260 Hz");
//    break;
//  case MPU6050_BAND_184_HZ:
//    Serial.println("184 Hz");
//    break;
//  case MPU6050_BAND_94_HZ:
//    Serial.println("94 Hz");
//    break;
//  case MPU6050_BAND_44_HZ:
//    Serial.println("44 Hz");
//    break;
//  case MPU6050_BAND_21_HZ:
//    Serial.println("21 Hz");
//    break;
//  case MPU6050_BAND_10_HZ:
//    Serial.println("10 Hz");
//    break;
//  case MPU6050_BAND_5_HZ:
//    Serial.println("5 Hz");
//    break;
//  }

//  Serial.println("");
  delay(100);
}

void loop() {

  /* Get new sensor events with the readings */
accelx = 0;
accely = 0;
accelz = 0;

gyrox = 0;
gyroy = 0;
gyroz = 0;
temperature = 0;
for (int i = 0; i <N_ave; i++){
  
sensors_event_t a, g, temp;
mpu.getEvent(&a, &g, &temp);

accelx = accelx + (a.acceleration.x - accelx_off)*accelx_scale;
accely = accely + (a.acceleration.y - accely_off)*accely_scale;
accelz = accelz + (a.acceleration.z - accelz_off)*accelz_scale;

gyrox = gyrox + (g.gyro.x - gyrox_off);
gyroy = gyroy + (g.gyro.y - gyroy_off);
gyroz = gyroz + (g.gyro.z - gyroz_off);

temperature = temperature + temp.temperature;

}

accelx = accelx/N_ave;
accely = accely/N_ave;
accelz = accelz/N_ave;

gyrox = gyrox/N_ave;
gyroy = gyroy/N_ave;
gyroz = gyroz/N_ave;
temperature = temperature/N_ave;
  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(accelx);
  Serial.print(", Y: ");
  Serial.print(accely);
  Serial.print(", Z: ");
  Serial.print(accelz);
  Serial.println(" m/s^2");

//  Serial.print("Rotation X: ");
//  Serial.print(gyrox,4);
//  Serial.print(", Y: ");
//  Serial.print(gyroy,4);
//  Serial.print(", Z: ");
//  Serial.print(gyroz,4);
//  Serial.println(" rad/s");

//  Serial.print("Temperature: ");
//  Serial.print(temperature);
//  Serial.println(" degC");

  Serial.println("");
  delay(10);
}

/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and ground
  - LED connected from digital pin 9 to ground through 220 ohm resistor

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogInOutSerial
*/

int a0 = 0, a1 = 0;  // value read from the pot
float x, y;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  a0 = analogRead(41);
  a1 = analogRead(40);

  if (a0 > 10) {
    x = ((1023 / (float)a0) - 1) * (10 / 9.78);
  } else {
    x = -1;
  }
  if (a1 > 10) {
    y = ((1023 / (float)a1) - 1) * (10 / 9.81);
  } else {
    y = -1;
  }

  // print the results to the Serial Monitor:
  Serial.print("x = ");
  Serial.print(x);
  Serial.print("\t y = ");
  Serial.print(y);
  Serial.print("\t 1 - y = ");
  Serial.print(1 - y);
  Serial.print("\t avg = ");
  Serial.print((x + (1 - y)) / 2);
  Serial.print("\t a0 = ");
  Serial.print(a0);
  Serial.print("\t a1 = ");
  Serial.println(a1);

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2);
}

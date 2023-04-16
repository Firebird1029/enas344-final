# Documentation

## Week 1

Encoder sensor
Motion sensor reading - gyro

Issues

- Encoder runs on 5V not 3V, discovered later
- Motion sensor kept hanging, prob due to 5V not 3V, discovered later

## Week 2

Combining motion sensor with Teensy audio, previously all independent tests and features

- This is the v1 code -- encoder determines pitch, 3D motion determines ladder function cutoff and oscillation depth

Cardboard box

- <https://en.makercase.com/#/>
  - <https://en.makercase.com/#/basicbox>

Softpot

- <https://www.adafruit.com/product/1069>
  - "To use" section specifies to use resistors, which lets you know when resistor is being pressed at all or not

Issues

- encoder connection kept breaking
- Softpot resistor gave offset readings due to 5V

## Week 3

<!-- (Or Move to Week 2? If so, move current Week 2 to Week 1 and current Week 1 to Week 0) -->
- Envelopes on soft pot press
- Sampling w/ delay line
  - Demo given by Konrad, 2 button setup, start 2 second loop recording + play note
- Quantization
- Metronome ticks
- Button to mute, tilt toward to save recording, tilt away to discard recording

Issues

- ?

- Put everything into box
- Soft pot calibration

Issues

- ?

## Week 4

## Future Steps

- Cooler box than just a box

## Appendix

### Using Arduino with VS Code

Due to personal preferences, using VS Code as my Arduino IDE felt like it would allow me to be more productive. Arduino extension.

- Spent significant time debugging undefined identifier error
- <https://github.com/microsoft/vscode-arduino/issues/808>
- Fixed it by using Arduino extension rebuild intellisense configuration, which automatically inserted the right DEFINES into the C/C++ VS Code configuration

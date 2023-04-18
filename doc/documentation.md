# Documentation

## Code Versions

1. v1.0 -- Encoder, MPU6050 -- The encoder sets the pitch (continuous), and the motion sensor sets the ladder filter cutoff and oscillation depth.
2. v1.1 -- Ribbon pot -- The ribbon softpot sets the pitch (chromatic thru one octave) when touched.
   1. v1.1.1 -- Ribbon pot -- Added envelope to allow note on and off when touched and released. Does not work (see Issues).
   2. v1.1.2_broken -- Ribbon pot -- updated pinout to Teensy 4.1, commented out MPU, updated getRibbonPotValAndMap to use new softpot circuitry, added delay for ramp up bug. Does not work (see Issues).
   3. v1.1.3_broken -- Ribbon pot -- Tried to store stable values of pitch to combat ramp down bug. Does not work (see Issues).
3. v1.2 -- Ribbon pot -- Working code (doesn't use faulty map function, manually maps from two analog values). Uses ribbon_read formulas.
4. v2.0 -- Ribbon pot, buttons, MPU -- With two delay lines.
   1. v2.1 -- Ribbon pot, buttons, MPU -- with MPU motion connected (roll, pitch, shake)

## Week 1

Encoder sensor

- <https://lastminuteengineers.com/rotary-encoder-arduino-tutorial/>
- <https://www.pjrc.com/teensy/td_libs_Encoder.html>

Motion sensor reading - gyro

Issues

- Encoder runs on 5V not 3V, discovered later
- Motion sensor kept hanging, prob due to 5V not 3V, discovered later
- Motion sensor drifted a lot -- fusion code

## Week 2

Combining motion sensor with Teensy audio, previously all independent tests and features

- This is the v1 code -- encoder determines pitch, 3D motion determines ladder function cutoff and oscillation depth
  - TODO PJRC image HERE

Cardboard box

- <https://en.makercase.com/#/>
  - <https://en.makercase.com/#/basicbox>

Softpot

- 9.39 kilo ohms
- <https://www.adafruit.com/product/1069>
  - "To use" section specifies to use resistors, which lets you know when resistor is being pressed at all or not

- v1.1 code
  - Week 2 demo video
  - TODO PJRC picture here -- week2/v1.1-pjrcgui.png

- v1.1.1 code
  - softpot with envelope

- v2.0 code
  - konrad's delay line
  - The very confusing part of this code is that there are actually three loops happening at the same time. The first loop is the standard Arduino `loop()`, which takes a variable amount of time each iteration. Code (including if statements) are running at this rate, and so there is some innate level of latency built-in when "waiting" to listen for the button press that starts the measure loop recording. The second loop includes the Teensy delay lines, operating at a rate defined by `delayObject.delay(0, LOOP_TIME);`. Finally, the third loop is the same duration as `LOOP_TIME`, but it only runs once and doesn't operate on Teensy's clock. It starts based on client interaction (e.g. button press), goes through one iteration, then stops. For example, when the client presses a button, the third loop starts at 0 and goes to `LOOP_TIME`, but the second loop might be halfway through its iteration. This means that `LOOP_TIME` milliseconds are recorded (when the third loop goes from 0 to `LOOP_TIME`), although the second loop, i.e. the delay lines, are actually wrapping around, and therefore are not actually ending at `LOOP_TIME` after `LOOP_TIME` seconds.

Issues

- encoder connection kept breaking
- Softpot resistor gave offset readings due to 5V

- Envelopes on soft pot press √
- Sampling w/ delay line √
  - Demo given by Konrad, 2 button setup, start 2 second loop recording + play note
  - TODO PJRC blocks image HERE -- week2/delay_looper_demo-pjrcgui.png
- v2 code
  - combine everything so far (ribbon pot + delay line + MPU)
  - temp + full delay line
- v2.1 code
  - add MPU-related audio features -- tilt toward to save recording, tilt away to discard recording, pitch to add vibrato, one shake to add percussion

Issues

- only bottom half of pot responds to decay to of envelope.noteOff()  -- fixed by new pot circuitry setup (stops value from "floating", not pressed is always 0), averaging (accounts for finger width), and short delay (ramping up to pot resistor value)
  - ramp up bug fixed by adding delay
  - ramp down bug caused by faulty map function, fixed my new map function (given by profs)
    - PICTURE ramp-down-bug.png
- ribbon softpot was extremely inconsistent -- switched to new circuitry
- ribbon softpot played multiple values on ramping -- fixed by new map function (without clipping), delay
- delay lines not working -- order of .begin() .delay() lines reversed, also complexity of PJRC diagram

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

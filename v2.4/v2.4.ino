#include <Adafruit_GFX.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Audio.h>
#include <Bounce2.h>
#include <Encoder.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Wire.h>

#include "effect_delayLoop.h"
// TODO fix VS code errors
// #include "mixer.h"

// GENERAL

#define LOOP_TIME 2400  // 1 measure @ 100 bpm (1 quarter note = 600 ms)
#define MASTER_VOLUME 0.8

// menu encoder sensitivity (how many encoder steps per menu item)
#define MES 10

#define BASE_FREQ 349.23  // F4
#define CHIPTUNE_FREQ_MOD 6.0
#define CHIPTUNE_AMP_PER_SEMITONE (1.f / (CHIPTUNE_FREQ_MOD * 12))
#define DELAYLINE_MAX_LEN 441000  // 44100 samples/sec * 10 sec
#define RIBBON_POT_CHECK_RATE 5   // lower rate = faster rate

#define DEFAULT_ENV_ATTACK 10.5
#define DEFAULT_ENV_HOLD 2.5
#define DEFAULT_ENV_DECAY 35
#define DEFAULT_ENV_SUSTAIN 0.8
#define DEFAULT_ENV_RELEASE 300

// PINOUT

#define ENCODER_DT_PIN 3
#define ENCODER_CLK_PIN 4
#define ENCODER_BTN_PIN 2
#define RIBBON_POT_PIN_1 41  // A17
#define RIBBON_POT_PIN_2 40  // A16
#define LOOP_BTN_PIN 32
#define COMMIT_BTN_PIN 31
#define CLEAR_BTN_PIN 30

// OLED

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET_PIN -1
#define SCREEN_ADDRESS 0x3C

// MOTION

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
EXTMEM int16_t DELAY_LINE_TEMP[DELAYLINE_MAX_LEN] = {};
EXTMEM int16_t DELAY_LINE_FULL[DELAYLINE_MAX_LEN] = {};

// CUSTOM OVERRIDES
AudioEffectdelayLoop tempDelay;  // xy=607,182
AudioEffectdelayLoop fullDelay;  // xy=868,201

// GUItool: begin automatically generated code
AudioSynthWaveformDc chipDC;                // xy=55,583
AudioPlaySdRaw sdDrum1;                     // xy=58.5,701
AudioPlaySdRaw sdDrum2;                     // xy=58.5,738
AudioSynthWaveform chord2fm;                // xy=59.5,499
AudioPlaySdRaw sdDrum3;                     // xy=58.5,776
AudioSynthWaveform chord3fm;                // xy=60.5,539
AudioSynthSimpleDrum drumSynth;             // xy=64,661
AudioSynthWaveform inWaveformFM;            // xy=77.5,401
AudioSynthWaveformDc inRamp;                // xy=120,298
AudioSynthWaveformModulated inWaveformMod;  // xy=191.5,452
AudioSynthWaveformModulated chipWave;       // xy=196,590
AudioSynthWaveformModulated chord3wave;     // xy=201.5,545
AudioSynthWaveformModulated chord2wave;     // xy=205.5,506
AudioMixer4 modeSelect;                     // xy=219.5,80
AudioEffectMultiply multiply1;              // xy=252,325
AudioMixer4 drumMixer;                      // xy=314,715
AudioMixer4 inMixer;                        // xy=335.5,177
AudioEffectEnvelope chipEnv;                // xy=335,590
AudioEffectEnvelope chord3env;              // xy=351.5,545
AudioEffectEnvelope chord2env;              // xy=353.5,506
AudioEffectEnvelope inEnvelope;             // xy=363.5,453
AudioSynthSimpleDrum metronome;             // xy=448,39
AudioMixer4 tempDelayMixer;                 // xy=457.5,302
AudioMixer4 chordMixer;                     // xy=528.5,502
// AudioEffectDelay         tempDelay;      //xy=607.5,182
AudioSynthWaveformDc fullRamp;             // xy=641,313
AudioSynthWaveformSine outLadderFreqSine;  // xy=681.5,89
AudioMixer4 outMixer;                      // xy=712.5,35
AudioEffectMultiply multiply2;             // xy=770,336
AudioMixer4 fullDelayMixer;                // xy=844.5,211
AudioFilterLadder outLadder;               // xy=880.5,30
AudioAmplifier outAmp;                     // xy=997,75
// AudioEffectDelay         fullDelay;      //xy=1022.5,193
AudioOutputI2S i2s1;  // xy=1120.5,31
AudioConnection patchCord1(chipDC, 0, chipWave, 0);
AudioConnection patchCord2(sdDrum1, 0, drumMixer, 1);
AudioConnection patchCord3(sdDrum2, 0, drumMixer, 2);
AudioConnection patchCord4(chord2fm, 0, chord2wave, 0);
AudioConnection patchCord5(sdDrum3, 0, drumMixer, 3);
AudioConnection patchCord6(chord3fm, 0, chord3wave, 0);
AudioConnection patchCord7(drumSynth, 0, drumMixer, 0);
AudioConnection patchCord8(inWaveformFM, 0, inWaveformMod, 0);
AudioConnection patchCord9(inRamp, 0, multiply1, 0);
AudioConnection patchCord10(inWaveformMod, inEnvelope);
AudioConnection patchCord11(chipWave, chipEnv);
AudioConnection patchCord12(chord3wave, chord3env);
AudioConnection patchCord13(chord2wave, chord2env);
AudioConnection patchCord14(modeSelect, 0, inMixer, 0);
AudioConnection patchCord15(multiply1, 0, tempDelayMixer, 1);
AudioConnection patchCord16(drumMixer, 0, modeSelect, 3);
AudioConnection patchCord17(drumMixer, 0, inMixer, 1);
AudioConnection patchCord18(inMixer, 0, outMixer, 0);
AudioConnection patchCord19(inMixer, 0, multiply1, 1);
AudioConnection patchCord20(chipEnv, 0, modeSelect, 1);
AudioConnection patchCord21(chord3env, 0, chordMixer, 2);
AudioConnection patchCord22(chord2env, 0, chordMixer, 1);
AudioConnection patchCord23(inEnvelope, 0, modeSelect, 0);
AudioConnection patchCord24(inEnvelope, 0, chordMixer, 0);
AudioConnection patchCord25(metronome, 0, outMixer, 3);
AudioConnection patchCord26(tempDelayMixer, tempDelay);
AudioConnection patchCord27(chordMixer, 0, modeSelect, 2);
AudioConnection patchCord28(tempDelay, 0, tempDelayMixer, 0);
AudioConnection patchCord29(tempDelay, 0, outMixer, 2);
AudioConnection patchCord30(tempDelay, 0, multiply2, 1);
AudioConnection patchCord31(fullRamp, 0, multiply2, 0);
AudioConnection patchCord32(outLadderFreqSine, 0, outLadder, 1);
AudioConnection patchCord33(outMixer, 0, outLadder, 0);
AudioConnection patchCord34(multiply2, 0, fullDelayMixer, 1);
AudioConnection patchCord35(fullDelayMixer, fullDelay);
AudioConnection patchCord36(outLadder, outAmp);
AudioConnection patchCord37(outAmp, 0, i2s1, 0);
AudioConnection patchCord38(outAmp, 0, i2s1, 1);
AudioConnection patchCord39(fullDelay, 0, fullDelayMixer, 0);
AudioConnection patchCord40(fullDelay, 0, outMixer, 1);
AudioControlSGTL5000 sgtl5000_1;  // xy=64.5,20
// GUItool: end automatically generated code

// BUTTONS
// Button loopButton = Button();
// Button synthButton = Button();
// Button clearButton = Button();
Button encButton = Button();

// ENCODER
Encoder enc(ENCODER_DT_PIN, ENCODER_CLK_PIN);

// MPU6050 IMU
Adafruit_MPU6050 mpu;

// OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET_PIN);

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

// GLOBAL - AUDIO

enum RECORDING_STATE {
  READY_FOR_RECORD,
  RECORDING,
  READY_FOR_COMMIT,
  COMMITTING
};
RECORDING_STATE recordingState = READY_FOR_RECORD;

int baseFreq = BASE_FREQ;

const int majorScale[8] = {0, 2, 4, 5, 7, 9, 11, 12};
const int minorScale[8] = {0, 2, 3, 5, 7, 8, 10, 12};
const int pentatonicScale[6] = {0, 2, 4, 7, 9, 12};
enum SCALE_TYPE { MAJOR, MINOR, PENTATONIC, CHROMATIC };

#define ARPEGGIO_COUNT 1
#define ARPEGGIO_SPEED 50  // ms between arpeggio notes
int ARPEGGIO_LENGTHS[ARPEGGIO_COUNT] = {12};
int ARPEGGIO_1[] = {0, 7, 12, 19, 24, 31, 36, 31, 24, 19, 12, 7};
int *ARPEGGIOS[ARPEGGIO_COUNT] = {ARPEGGIO_1};

int currentArpeggio = 0, curArpNoteIdx = 0;
long unsigned int nextArpNoteTime = 0;

int metronomeSpeed = 0;  // 0 = off, 1 = tick at quarter note rate, 2 = tick at
                         // eighth note rate, 3 = tick at triplet rate, etc.

const char *SD_PERC[] = {"00_kick2.raw",      "01_snare.raw",
                         "02_tom_low.raw",    "04_tom_high.raw",
                         "05_hihat_open.raw", "06_hihat_closed.raw",
                         "07_rim.raw",        "08_shaker.raw"};

// GLOBAL - GENERAL

elapsedMillis timer;  // master timer (auto-incrementing)

// home = OLED + encoder, playing = softpot, other = opposite of OLED + encoder
enum ORIENTATION { HOME, PLAYING, OTHER };
ORIENTATION orientation = HOME;
ORIENTATION lastOrientation = HOME;
enum ROLL_STATE {
  FLAT,
  LEFT,
  LEFT_START_TRIGGER,
  LEFT_END_TRIGGER,
  RIGHT,
  RIGHT_START_TRIGGER,
  RIGHT_END_TRIGGER
};
ROLL_STATE rollState = FLAT;
ROLL_STATE lastRollState = FLAT;

long unsigned int lastCheckedRibbonPot = 0,  // last time ribbon pot was checked
    lastMetronomeTick = 0,                   // last time metronome ticked
    tempRecordingStart =
        0,  // timestamp when recording started (recording into temp delay)
    commitRecordingStart =
        0;  // timestamp when commit started (recording into full delay)

int mappedRibbonPotVal =
    0;  // ribbon softpot values mapped to integer half-tones above fundamental

// curSustainStatus == 0 && ribbon released -> do nothing
// curSustainStatus == 0 && ribbon pressed -> note on, set status := 1
// curSustainStatus == 1 && ribbon pressed -> sustain note
// curSustainStatus == 1 && ribbon released -> note off, set status := 0
bool curSustainStatus = false;

// GLOBAL - MENU (OLED + ENCODER)

// Warning: Make sure all 3 variables match!
#define NUM_MODE_OPTIONS 5
const char *MODE_OPTIONS[NUM_MODE_OPTIONS] = {"Simple", "Soft", "Chiptune",
                                              "Chord", "Percussion"};
enum MODE_STATE {
  MODE_SIMPLE = 0,
  MODE_SOFT,
  MODE_CHIPTUNE,
  MODE_CHORD,
  MODE_PERCUSSION
};

enum MENU_STATE { MENU_MODE, MENU_VOLUME, MENU_CLEAR, MENU_METRONOME };
enum MENU_ACTIVE { MENU_INACTIVE, MENU_ACTIVE_OPTION };
MENU_STATE menuState = MENU_MODE;
MENU_ACTIVE menuActiveState = MENU_INACTIVE;

MODE_STATE currentSoundMode = MODE_SIMPLE;  // see MODE_OPTIONS
int clientMasterVolume = 100,               // 0-100
    encValue,                               // current encoder value
    savedEncValueMenu,  // encValue of main menu (before entering active option)
    savedEncValueHome;  // encValue of menu (both main menu and active submenu)
                        // before switching to playing mode

void setup(void) {
  // SERIAL SETUP
  Serial.begin(115200);
  Serial.println("Serial started");

  // AUDIO SETUP
  AudioMemory(500);
  sgtl5000_1.enable();
  sgtl5000_1.volume(MASTER_VOLUME);

  // Basic Synth
  inWaveformFM.begin(0.0, 6, WAVEFORM_SINE);
  inWaveformMod.begin(0.5, baseFreq, WAVEFORM_SAWTOOTH);
  inWaveformMod.frequencyModulation(1);
  inEnvelope.attack(DEFAULT_ENV_ATTACK);
  inEnvelope.hold(DEFAULT_ENV_HOLD);
  inEnvelope.decay(DEFAULT_ENV_DECAY);
  inEnvelope.sustain(DEFAULT_ENV_SUSTAIN);
  inEnvelope.release(DEFAULT_ENV_RELEASE);

  // Chiptune
  chipDC.amplitude(0);
  chipWave.begin(0.25, baseFreq, WAVEFORM_BANDLIMIT_SQUARE);
  chipWave.frequencyModulation(CHIPTUNE_FREQ_MOD);
  chipEnv.attack(DEFAULT_ENV_ATTACK);
  chipEnv.hold(DEFAULT_ENV_HOLD);
  chipEnv.decay(DEFAULT_ENV_DECAY);
  chipEnv.sustain(DEFAULT_ENV_SUSTAIN);
  chipEnv.release(DEFAULT_ENV_RELEASE);

  // Chords -- copy & paste from Basic Synth
  chord2fm.begin(0.0, 6, WAVEFORM_SINE);
  chord2wave.begin(0.5, baseFreq, WAVEFORM_SINE);
  chord2wave.frequencyModulation(1);
  chord2env.attack(DEFAULT_ENV_ATTACK);
  chord2env.hold(DEFAULT_ENV_HOLD);
  chord2env.decay(DEFAULT_ENV_DECAY);
  chord2env.sustain(DEFAULT_ENV_SUSTAIN);
  chord2env.release(DEFAULT_ENV_RELEASE);

  chord3fm.begin(0.0, 6, WAVEFORM_SINE);
  chord3wave.begin(0.5, baseFreq, WAVEFORM_SINE);
  chord3wave.frequencyModulation(1);
  chord3env.attack(DEFAULT_ENV_ATTACK);
  chord3env.hold(DEFAULT_ENV_HOLD);
  chord3env.decay(DEFAULT_ENV_DECAY);
  chord3env.sustain(DEFAULT_ENV_SUSTAIN);
  chord3env.release(DEFAULT_ENV_RELEASE);

  chordMixer.gain(0, 1.0);
  chordMixer.gain(1, 1.0);
  chordMixer.gain(2, 1.0);

  // Percussion
  drumSynth.frequency(60);
  drumSynth.length(1500);
  drumSynth.secondMix(0.0);
  drumSynth.pitchMod(0.55);
  drumMixer.gain(0, 0.5);  // drum synth
  drumMixer.gain(1, 0.8);  // sd sample reader 1
  // ! using more than one sd read is buggy so sdDrum2 and sdDrum3 are unused
  drumMixer.gain(2, 0.0);  // sd sample reader 2
  drumMixer.gain(3, 0.0);  // sd sample reader 3

  // Metronome
  metronome.frequency(4000);
  metronome.length(50);
  metronome.secondMix(0.0);
  metronome.pitchMod(0.5);

  // Mode Selection
  modeSelect.gain(0, 1.0);  // simple/soft
  modeSelect.gain(1, 0.0);  // chiptune
  modeSelect.gain(2, 0.0);  // chord
  modeSelect.gain(3, 0.0);  // percussion
  inMixer.gain(0, 0.8);     // mode output
  inMixer.gain(1, 0.8);     // drum synth

  // Delay Lines - Mixers Only
  tempDelayMixer.gain(0, 0.0);  // feedback
  tempDelayMixer.gain(1, 0.0);  // inMixer
  fullDelayMixer.gain(0, 1.0);  // feedback
  fullDelayMixer.gain(1, 0.0);  // temp mixer

  // Amplitude Ramping
  inRamp.amplitude(0.0);
  fullRamp.amplitude(0.0);

  // Output
  outMixer.gain(0, 1.0);  // realtime dry input
  outMixer.gain(1,
                0.0);  // full delay -- set to 0.0 to avoid initial random noise
  outMixer.gain(2,
                0.0);  // temp delay -- set to 0.0 to avoid initial random noise
  outMixer.gain(3, 1.0);  // metronome
  outLadderFreqSine.frequency(10);
  outLadderFreqSine.amplitude(0);
  outLadder.frequency(100000);
  outLadder.octaveControl(6);
  outAmp.gain(1.0);

  // BUTTON SETUP
  // loopButton.attach(LOOP_BTN_PIN, INPUT_PULLUP);
  // loopButton.interval(5);
  // loopButton.setPressedState(LOW);

  // synthButton.attach(COMMIT_BTN_PIN, INPUT_PULLUP);
  // synthButton.interval(5);
  // synthButton.setPressedState(LOW);

  // clearButton.attach(CLEAR_BTN_PIN, INPUT_PULLUP);
  // clearButton.interval(5);
  // clearButton.setPressedState(LOW);

  // ENCODER SETUP
  encButton.attach(ENCODER_BTN_PIN, INPUT_PULLUP);
  encButton.interval(5);
  encButton.setPressedState(LOW);

  // MOTION SETUP
  Serial.println("Initializing MPU6050...");
  if (!mpu.begin()) {
    Serial.println("ERROR: Failed to find MPU6050 chip!");
    while (true) {
      delay(10);
    }
  }
  Serial.println("Initialized MPU6050!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_184_HZ);

  // OLED SETUP
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true) {
      delay(10);
    }
  }
  display.display();
  delay(500);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  // SD SETUP
  // uses SD port on Teensy 4.1, NOT the SD reader on audio shield
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Unable to access SD Card");
    while (true) {
      delay(10);
    }
  }

  // DELAY LINES
  tempDelay.begin(DELAY_LINE_TEMP, DELAYLINE_MAX_LEN);
  fullDelay.begin(DELAY_LINE_FULL, DELAYLINE_MAX_LEN);
  tempDelay.delay(0, LOOP_TIME);  // must come after tempDelay.begin!
  fullDelay.delay(0, LOOP_TIME);  // must come after fullDelay.begin!

  // clear delay lines -- avoid initial random noise
  // TODO add double clear
  tempDelay.clear();
  fullDelay.clear();

  Serial.println("Finished setup.");
  delay(100);
}

void loop() {
  // MOTION CODE -- Read motion data
  // This comes first to determine orientation
  motionCode();

  // ladder1.frequency(abs(pitch * 1000 + 500));
  // outLadderFreqSine.amplitude(roll / 10);

  // MODE SELECTION
  // must come before RIBBON POT (PITCH) code!
  modeSelectionCode();

  // RIBBON POT (PITCH)
  ribbonPotCode();

  // Read button data
  // loopButton.update();
  // synthButton.update();
  // clearButton.update();

  // LOOP RECORDING
  loopRecordingCode();

  // METRONOME
  metronomeCode();

  // MENU OR SOUND EFFECTS
  encValue = enc.read();
  encButton.update();

  if (orientation == HOME) {
    if (lastOrientation == PLAYING) {
      Serial.println("Playing -> Home");
      // restore old encoder value
      encValue = savedEncValueHome;
      enc.write(savedEncValueHome);
      lastOrientation = HOME;
    }

    menuCode();
  } else if (orientation == PLAYING) {
    if (lastOrientation == HOME) {
      Serial.println("Home -> Playing");
      // save encoder value
      savedEncValueHome = encValue;
      enc.write(0);

      // reset base octave
      baseFreq = BASE_FREQ;

      lastOrientation = PLAYING;
    }

    soundEffectsCode();
  }

  // SET LOOP OR OCTAVES
  if (orientation == HOME) {
    if (rollState == LEFT_END_TRIGGER) {
      // Change Master Full Delay Loop
      Serial.println("Left end trigger");
    }
    if (rollState == RIGHT_END_TRIGGER) {
      Serial.println("Right end trigger");
    }
  } else if (orientation == PLAYING) {
    // Change Octaves
    if (rollState == LEFT_END_TRIGGER) {
      baseFreq /= 2;
    }
    if (rollState == RIGHT_END_TRIGGER) {
      baseFreq *= 2;
    }
  }

  // QUICK PERCUSSION
  if (abs(accelz) > 19.0 && orientation == PLAYING) {
    // a shake up/down usually goes up to 19.0
    // TODO change to change in accel
    Serial.println(accelz);
    // drumSynth.noteOn();
    if (!sdDrum1.isPlaying()) {
      sdDrum1.play(SD_PERC[0]);
    }
  }

  delay(1);
}

// MOTION CODE

void motionCode() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  accelx = -(a.acceleration.x - ACCELX_OFF) / ACCELX_SCALE;
  accely = -(a.acceleration.y - ACCELY_OFF) / ACCELY_SCALE;
  accelz = -(a.acceleration.z - ACCELZ_OFF) / ACCELZ_SCALE;

  gyrox = (g.gyro.x - GYROX_OFF);
  gyroy = (g.gyro.y - GYROY_OFF);
  gyroz = (g.gyro.z - GYROZ_OFF);

  pitchDot = gyroy * cos(roll) - gyroz * sin(roll);
  yawDot = (gyroy * sin(roll) + gyroz * cos(roll)) / cos(pitch);
  rollDot = gyrox + sin(pitch) * yawDot;

  // gyroPitch = gyroPitch + gyroy * 0.01;
  // gyroRoll =  gyroRoll + gyrox * 0.01;
  // gyroYaw =  gyroYaw + gyroz * 0.01;

  accelPitch = atan2(-accelx, sqrt(accely * accely + accelz * accelz));
  accelRoll = atan2(accely, sqrt(accelx * accelx + accelz * accelz));

  pitch = MOTION_ALPHA * (pitch + pitchDot * 0.01) +
          (1 - MOTION_ALPHA) * accelPitch;  // final pitch value
  roll = -MOTION_ALPHA * (roll + rollDot * 0.01) -
         (1 - MOTION_ALPHA) * accelRoll;  // final roll value

  // pitch ranges from -1.5 to 1.5
  // roll ranges from -0.5 to 0.5

  // calculate roll state
  calculateRollState();

  // only change orientation state based on pitch, if no roll
  if (abs(roll) < 0.1) {
    if (abs(pitch) < 0.3) {
      orientation = PLAYING;
    } else if (pitch < -1.2) {
      orientation = HOME;
    } else if (pitch > 1.2) {
      orientation = OTHER;
    }
  }
}

void calculateRollState() {
  // state machine for roll
  switch (rollState) {
    case FLAT:
      if (roll > 0.3) {
        rollState = LEFT_START_TRIGGER;
      } else if (roll < -0.3) {
        rollState = RIGHT_START_TRIGGER;
      }
      break;
    case LEFT_START_TRIGGER:
      rollState = LEFT;
      break;
    case RIGHT_START_TRIGGER:
      rollState = RIGHT;
      break;
    case LEFT:
      if (roll <= 0.1) {
        rollState = LEFT_END_TRIGGER;
      }
      break;
    case RIGHT:
      if (roll >= -0.1) {
        rollState = RIGHT_END_TRIGGER;
      }
      break;
    case LEFT_END_TRIGGER:
    case RIGHT_END_TRIGGER:
      rollState = FLAT;
      break;
  }
}

// RIBBON POT CODE

void ribbonPotCode() {
  // only check ribbon pot for updates as fast as RIBBON_POT_CHECK_RATE
  if (timer >= lastCheckedRibbonPot + RIBBON_POT_CHECK_RATE) {
    lastCheckedRibbonPot = timer;

    mappedRibbonPotVal = getRibbonPotValAndMap(0, 8);

    if (mappedRibbonPotVal > -1) {
      // ribbon pressed

      // set frequencies in pitched modes
      if (currentSoundMode != MODE_PERCUSSION) {
        mappedRibbonPotVal = mapScale(MINOR, mappedRibbonPotVal);

        inWaveformMod.frequency(baseFreq * pow(2, mappedRibbonPotVal / 12.0));
        chipWave.frequency(baseFreq * pow(2, mappedRibbonPotVal / 12.0));
        // TODO FIX SET TO SEMITONES
        chord2wave.frequency(baseFreq *
                             pow(2, (mappedRibbonPotVal + 4) / 12.0));
        chord3wave.frequency(baseFreq *
                             pow(2, (mappedRibbonPotVal + 7) / 12.0));
      }

      if (curSustainStatus) {
        // sustain note
      } else {
        // note on
        inEnvelope.noteOn();
        chipEnv.noteOn();
        chord2env.noteOn();
        chord3env.noteOn();

        // percussion mode
        if (currentSoundMode == MODE_PERCUSSION) {
          mapPercussion(mappedRibbonPotVal);
        }

        curSustainStatus = true;
      }
    } else {
      // inWaveformMod.amplitude(0);
      if (curSustainStatus) {
        // note off
        inEnvelope.noteOff();
        chipEnv.noteOff();
        chord2env.noteOff();
        chord3env.noteOff();
        curSustainStatus = false;
      }
    }
  }
}

/**
 * @brief get the ribbon pot val and map it to a new range if being pressed
 *
 * @param newMin min of new range, output will never be less than this
 * @param newMax max of new range, output will never be more than this
 * @return (int) -1 if ribbon pot is not pressed, otherwise a value between
 * newMin and newMax
 */
int getRibbonPotValAndMap(int newMin, int newMax) {
  float a1 = analogRead(RIBBON_POT_PIN_1);  // sensorValue2
  float a2 = analogRead(RIBBON_POT_PIN_2);  // sensorValue1

  if (a1 < 10 || a2 < 10) {
    return -1;
  }

  // calibrate by measuring actual resistance of 10k resistor (e.g. 9.93 or 9.8)
  float x = ((1023 / (float)a1) - 1) * (10 / 9.78);
  float y = ((1023 / (float)a2) - 1) * (10 / 9.81);

  float pos = (x + (1 - y)) / 2;  // normalized position (from 0.0 to 1.0)
  pos = (newMax - newMin) * pos + newMin;  // mapped position

  // clip position
  if (pos < newMin) {
    pos = newMin;
  } else if (pos > newMax) {
    pos = newMax;
  }

  return (int)pos;
}

int mapScale(SCALE_TYPE scaleType, int step) {
  if (step == -1) {
    return -1;
  }
  if (scaleType == MAJOR) {
    // convert step (from 0-6 inclusive) to a major scale
    return majorScale[step];
  } else if (scaleType == MINOR) {
    // convert step (from 0-6 inclusive) to a minor scale
    return minorScale[step];
  } else if (scaleType == PENTATONIC) {
    // convert step (from 0-5 inclusive) to a pentatonic scale
    return pentatonicScale[step];
  }

  return step;
}

void mapPercussion(int sampleNum) {
  if (sampleNum < 0 || sampleNum > 7) {
    return;
  }

  // ! using more than one sd read is buggy so sdDrum2 and sdDrum3 are unused

  // TODO switch to drumSynth instead of sdDrum1

  switch (sampleNum) {
    case 0:
      sdDrum1.play(SD_PERC[0]);
      break;
    case 1:
      sdDrum1.play(SD_PERC[1]);
      break;
    case 2:
      sdDrum1.play(SD_PERC[2]);
      break;
    case 3:
      sdDrum1.play(SD_PERC[2]);
      break;
    case 4:
      sdDrum1.play(SD_PERC[3]);
      break;
    case 5:
      sdDrum1.play(SD_PERC[4]);
      break;
    case 6:
      sdDrum1.play(SD_PERC[5]);
      break;
    case 7:
      sdDrum1.play(SD_PERC[6]);
      break;
    case 8:
      sdDrum1.play(SD_PERC[7]);
      break;
  }

  // 3 different SD read objects to enable percussive polyphony
  // doesn't work :(
  // if (!sdDrum1.isPlaying()) {
  //   sdDrum1.play(SD_PERC[mappedRibbonPotVal]);
  // } else if (!sdDrum2.isPlaying()) {
  //   sdDrum2.play(SD_PERC[mappedRibbonPotVal]);
  // }
  // else if (!sdDrum3.isPlaying()) {
  //   sdDrum3.play(SD_PERC[mappedRibbonPotVal]);
  // }
}

// CHIPTUNE CODE

void chiptuneCode() {
  if (timer >= nextArpNoteTime + ARPEGGIO_SPEED) {
    chipDC.amplitude(
        ARPEGGIOS[currentArpeggio][curArpNoteIdx] * CHIPTUNE_AMP_PER_SEMITONE,
        0);
    curArpNoteIdx++;
    if (curArpNoteIdx >= ARPEGGIO_LENGTHS[currentArpeggio]) {
      curArpNoteIdx = 0;
    }
    nextArpNoteTime = timer;
  }
}

// METRONOME CODE

void metronomeCode() {
  if (metronomeSpeed > 0) {
    if (timer >= lastMetronomeTick + (LOOP_TIME / (metronomeSpeed * 4))) {
      lastMetronomeTick = timer;

      // theoretically reduces metronome drift,
      // but doesn't work if metronome can be turned off
      // lastMetronomeTick = lastMetronomeTick + (LOOP_TIME / (metronomeSpeed *
      // 4));

      metronome.noteOn();
    }
  }
}

// MODE SELECTION CODE

void modeSelectionCode() {
  // disable all input channels initially
  for (int i = 0; i < 4; i++) {
    modeSelect.gain(i, 0.0);
  }

  inMixer.gain(1, 1.0);  // enable quick percussion

  // Simple
  if (currentSoundMode == MODE_SIMPLE) {
    inWaveformMod.begin(WAVEFORM_SAWTOOTH);
    inWaveformMod.amplitude(0.5);
    modeSelect.gain(0, 1.0);
  }

  // Soft
  if (currentSoundMode == MODE_SOFT) {
    inWaveformMod.begin(WAVEFORM_SINE);
    inWaveformMod.amplitude(0.5);
    modeSelect.gain(0, 1.0);
  }

  // Chiptune
  if (currentSoundMode == MODE_CHIPTUNE) {
    chiptuneCode();
    modeSelect.gain(1, 1.0);
  }

  // Chord
  if (currentSoundMode == MODE_CHORD) {
    inWaveformMod.begin(WAVEFORM_SINE);
    inWaveformMod.amplitude(0.33 / 2);
    chord2wave.amplitude(0.33 / 2);
    chord3wave.amplitude(0.33 / 2);
    modeSelect.gain(2, 1.0);

  } else {
    chord2env.noteOff();
    chord3env.noteOff();
    // do not: chord2/3wave.amplitude(0.0); ! (prevents envelope from releasing)
  }

  // Percussion
  if (currentSoundMode == MODE_PERCUSSION) {
    inMixer.gain(1, 0.0);  // disable quick percussion
    modeSelect.gain(3, 1.0);
  }
}

// MENU CODE

void menuCode() {
  // Encoder: Menu Navigation
  if (menuActiveState == MENU_INACTIVE) {
    // encoder toggles through menu options
    // TODO switch to programmatic way (non if statements)
    if (encValue >= 0 && encValue < MES) {
      menuState = MENU_METRONOME;
    } else if (encValue >= MES && encValue < 2 * MES) {
      menuState = MENU_CLEAR;
    } else if (encValue >= 2 * MES && encValue < 3 * MES) {
      menuState = MENU_MODE;
    } else if (encValue >= 3 * MES && encValue < 4 * MES) {
      menuState = MENU_VOLUME;
      // menuState = MENU_POWER;
    } else if (encValue < 0) {
      // TODO switch to programmatic way
      enc.write(39);
    } else {
      enc.write(0);
    }
  } else if (menuActiveState == MENU_ACTIVE_OPTION) {
    if (menuState == MENU_MODE) {
      // encoder toggles through mode options
      if (encValue >= MES) {
        currentSoundMode =
            (MODE_STATE)(currentSoundMode + 1) % NUM_MODE_OPTIONS;
        enc.write(0);
      } else if (encValue <= -MES) {
        currentSoundMode =
            (MODE_STATE)(currentSoundMode + NUM_MODE_OPTIONS - 1) %
            NUM_MODE_OPTIONS;
        enc.write(0);
      }
    } else if (menuState == MENU_VOLUME) {
      // encoder adjusts volume
      clientMasterVolume += (int)(encValue / (int)(MES / 5));
      if (clientMasterVolume < 0) {
        clientMasterVolume = 0;
      } else if (clientMasterVolume > 100) {
        clientMasterVolume = 100;
      }

      // set volume
      outAmp.gain(clientMasterVolume / 100.0);

      enc.write(0);
    } else if (menuState == MENU_METRONOME) {
      // encoder adjusts metronome speed
      metronomeSpeed = (int)(encValue / (int)(MES));
      if (metronomeSpeed < 0) {
        metronomeSpeed = 0;
        enc.write(0);
      } else if (metronomeSpeed > 6) {
        metronomeSpeed = 6;
        enc.write(MES * 6);
      }
    }
  }

  // Encoder Button: Change Active State
  if (encButton.pressed()) {
    if (menuActiveState == MENU_INACTIVE) {
      // change to active option state

      // special exceptions, substate-less menu options
      if (menuState == MENU_CLEAR) {
        if (menuState == MENU_CLEAR) {
          // clear loops -- TODO add double clear
          tempDelay.clear();
          fullDelay.clear();
        }

      } else {
        savedEncValueMenu = encValue;
        menuActiveState = MENU_ACTIVE_OPTION;

        // reset encoder value
        enc.write(0);

        // special exceptions
        if (menuState == MENU_METRONOME) {
          enc.write(MES * metronomeSpeed);
        }
      }
    } else {
      // change to inactive (main menu) state
      encValue = savedEncValueMenu;
      menuActiveState = MENU_INACTIVE;

      // restore menu navigation scroll position
      enc.write(encValue);
    }
  }

  display.clearDisplay();
  display.setCursor(0, 0);

  // Metronome
  displayMenuPrefix(MENU_METRONOME);
  display.print("Metronome: ");
  if (metronomeSpeed == 0) {
    display.print("Off  ");
  } else {
    display.print(metronomeSpeed);
    display.print("    ");
  }
  display.println(recordingState == READY_FOR_RECORD ? " " : "*");

  // Clear Loop
  displayMenuPrefix(MENU_CLEAR);
  display.println("Clear Loop");

  // Mode
  displayMenuPrefix(MENU_MODE);
  display.println(MODE_OPTIONS[currentSoundMode]);

  // Volume
  displayMenuPrefix(MENU_VOLUME);
  display.print("Volume: ");
  display.println(clientMasterVolume);

  display.display();
}

void displayMenuPrefix(int ms) {
  display.print(" ");  // TODO remove when OLED replaced
  if (menuState == ms) {
    display.print(">");
  } else {
    display.print(" ");
  }
  if (menuState == ms && menuActiveState == MENU_ACTIVE_OPTION) {
    display.print(">");
  } else {
    display.print(" ");
  }
  display.print(" ");
}

// SOUNDS EFFECTS CODE

void soundEffectsCode() {
  // TODO
  // VIBRATO
  // Serial.println(pitch);
  // inWaveformFM.amplitude(mapFloat(abs(roll), 0, 0.5, 0, 0.1));
}

// LOOP RECORDING CODE
void loopRecordingCode() {
  // START RECORDING
  if (recordingState == READY_FOR_RECORD) {
    if (orientation == PLAYING) {
      // to prevent race conditions, do not allow recording a loop if in the
      // middle of committing a loop
      Serial.println("STARTING LOOP RECORDING");
      // send the synth signal into the delay loop
      tempDelayMixer.gain(1, 1.0);
      inRamp.amplitude(1.0, 50);

      // comment this line if you don't want to clear the loop when you record
      // new material
      // tempDelayMixer.gain(0, 0.0);
      tempDelayMixer.gain(0, 1.0);

      // outMixer.gain(2, 0.0);  // disable temp delay
      outMixer.gain(2, 1.0);  // enable temp delay

      tempRecordingStart = timer;

      // recordingState = RECORDING;
      recordingState = READY_FOR_COMMIT;
    }
  }

  // FINISH RECORDING
  /* else if (recordingState == RECORDING) {
    if (timer >= (tempRecordingStart + LOOP_TIME)) {
      Serial.println("Recording stopped");
      // turn off the signal into the delay loop
      // tempDelayMixer.gain(1, 0.0);
      inRamp.amplitude(0.0, 50);

      // resets the feedback to 1.0 so the loop repeats indefinitely
      tempDelayMixer.gain(0, 1.0);

      outMixer.gain(2, 1.0);  // enable temp delay
      recordingState = READY_FOR_COMMIT;
    }
  } */

  // START COMMITTING LAYER
  else if (recordingState == READY_FOR_COMMIT) {
    if (orientation == HOME) {
      Serial.println("SAVE pitch detected, committing recording");

      // FROM FINISH RECORDING

      // turn off the signal into the delay loop
      // tempDelayMixer.gain(1, 0.0);
      inRamp.amplitude(0.0, 50);

      // resets the feedback to 1.0 so the loop repeats indefinitely
      // tempDelayMixer.gain(0, 1.0); // already on

      outMixer.gain(2, 1.0);  // enable temp delay

      // END FROM FINISH RECORDING

      // enable temp delay signal into full delay
      fullDelayMixer.gain(1, 1.0);
      fullRamp.amplitude(1.0, 50);

      commitRecordingStart = timer;
      recordingState = COMMITTING;
    } else if (orientation == OTHER) {
      Serial.println("DISCARD pitch detected");

      // unnecessary because you have to orient back to playing position anyway
      // turn off the signal into the delay loop
      // tempDelayMixer.gain(1, 0.0);
      // inRamp.amplitude(0.0, 50);

      tempDelay.clear();  // TODO add double clear

      recordingState = READY_FOR_RECORD;
    }
  }

  // FINISH COMMITTING LAYER
  else if (recordingState == COMMITTING) {
    if (timer >= (commitRecordingStart + LOOP_TIME)) {
      Serial.println("Commit recording stopped");

      outMixer.gain(2, 0.0);  // disable temp delay, fixes double timbre glitch

      // disable temp delay signal into full delay
      // fullDelayMixer.gain(1, 0.0);
      fullRamp.amplitude(0.0, 50);

      // disable temp delay signal feedback
      tempDelayMixer.gain(0, 0.0);

      // clear temp delay (optional)
      tempDelay.clear();

      // resets the feedback to 1.0 so the loop repeats indefinitely
      // fullDelayMixer.gain(0, 1.0); // already always 1.0 for full delay!

      outMixer.gain(1, 1.0);  // enable full delay

      recordingState = READY_FOR_RECORD;
    }
  }

  // CLEAR FULL DELAY
  // if (clearButton.pressed() && !isCommittingLoop) {
  //   Serial.println("Clear button pressed");
  //   // clear the loop
  //   fullDelay.clear();
  // }
}

// HELPER FUNCTIONS

// https://forum.arduino.cc/t/arduino-map-function-for-float-values/112888/2
/**
 * @brief mapFloat (no automatic clipping, output can exceed out_min/out_max)
 */
float mapFloat(float x, float in_min, float in_max, float out_min,
               float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

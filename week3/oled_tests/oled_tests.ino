#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Bounce2.h>
#include <Encoder.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Wire.h>

#define ENCODER_DT_PIN \
  4  // reversed on purpose, so clockwise is positive, and
     // counter-clockwise is negative
#define ENCODER_CLK_PIN 3
#define ENCODER_BTN_PIN 2

Encoder enc(ENCODER_CLK_PIN, ENCODER_DT_PIN);
Button encButton = Button();

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels
#define OLED_RESET -1     // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define MES \
  10  // menu encoder sensitivity (how many encoder steps per menu item)

void setup() {
  Serial.begin(9600);

  // OLED SETUP
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true) {
      delay(1);
    }
  }
  display.display();
  delay(500);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  // BUTTON SETUP
  encButton.attach(ENCODER_BTN_PIN, INPUT_PULLUP);
  encButton.interval(5);
  encButton.setPressedState(LOW);
}

const int NUM_MODE_OPTIONS = 5;
const char *MODE_OPTIONS[NUM_MODE_OPTIONS] = {"Simple", "Soft", "Chiptune",
                                              "Chord", "Percussion"};
int modeIndex = 0;
int clientMasterVolume = 100;
enum MENU_STATE {
  MENU_MODE,
  MENU_VOLUME,
  MENU_CLEAR,
  MENU_POWER,
  MENU_START_LOOP
};  // MENU_POWER, MENU_START_LOOP unused TODO
enum MENU_ACTIVE { MENU_INACTIVE, MENU_ACTIVE_OPTION };
MENU_STATE menuState = MENU_MODE;
MENU_ACTIVE menuActiveState = MENU_INACTIVE;

int encValue, savedEncValue;  // savedEncValue = encValue of main menu (before
                              // entering active option)

void loop() {
  // Encoder: Menu Navigation
  int encValue = enc.read();
  if (menuActiveState == MENU_INACTIVE) {
    // encoder toggles through menu options
    // TODO switch to programmatic way (non if statements)
    if (encValue >= 0 && encValue < MES) {
      menuState = MENU_START_LOOP;  // TODO replace with IMU
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
        modeIndex = (modeIndex + 1) % NUM_MODE_OPTIONS;
        enc.write(0);
      } else if (encValue <= -MES) {
        modeIndex = (modeIndex + NUM_MODE_OPTIONS - 1) % NUM_MODE_OPTIONS;
        enc.write(0);
      }
    }
    if (menuState == MENU_VOLUME) {
      // encoder adjusts volume
      clientMasterVolume += (int)(encValue / (int)(MES / 5));
      if (clientMasterVolume < 0) {
        clientMasterVolume = 0;
      } else if (clientMasterVolume > 100) {
        clientMasterVolume = 100;
      }
      enc.write(0);
    }
  }

  // Encoder Button: Change Active State
  encButton.update();
  if (encButton.pressed()) {
    Serial.println("Encoder button pressed");
    if (menuActiveState == MENU_INACTIVE) {
      // change to active option state
      if (menuState == MENU_START_LOOP || menuState == MENU_CLEAR) {
        // substate-less menu options
        if (menuState == MENU_CLEAR) {
          // clearLoop();
        }

        // TODO remove MENU_START_LOOP later
        if (menuState == MENU_START_LOOP) {
          // startLoop();
        }
      }
      savedEncValue = encValue;
      enc.write(0);
      menuActiveState = MENU_ACTIVE_OPTION;
    } else {
      // change to inactive (main menu) state
      encValue = savedEncValue;
      enc.write(encValue);
      menuActiveState = MENU_INACTIVE;
    }
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  // display.println(encValue);

  // Start Loop Recording
  // TODO remove later
  displayMenuPrefix(MENU_START_LOOP);
  display.println("Start Loop");

  // Clear Loop
  displayMenuPrefix(MENU_CLEAR);
  display.println("Clear Loop");

  // Mode
  displayMenuPrefix(MENU_MODE);
  display.println(MODE_OPTIONS[modeIndex]);

  // Volume
  displayMenuPrefix(MENU_VOLUME);
  display.print("Volume: ");
  display.println(clientMasterVolume);

  display.display();

  delay(10);
}

void displayMenuPrefix(int ms) {
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

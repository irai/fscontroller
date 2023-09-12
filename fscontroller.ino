#include <HID_Buttons.h>  // Must import AFTER Keyboard.h

#include "electronics.h"

bool Debug = false;
statistics stats;

// Reserved pins
#define PIN_ON_OFF A0  // first pin after GND on Teensy 2.0++


// uncomment one of these to build the right panel
// #define LIGHTS_PANEL 1
// #define FLAPS_PANEL 1
// #define G1000_PANEL 1
// #define TEST_PANEL 1
// #define KEYBOARD_PANEL 1  // panel with no electronics used for keyboard
#define SINGLE_THROTTLE_QUADRANT_PANEL 1

Stream *piHandler;
Print *debugHandler;

typedef struct SerialMsg {
  Stream *Port;
  uint8_t buffer[64];
  char txBuffer[64];
  char txBufferLen;
  unsigned int count;
  int dataLen;
  int state;
  unsigned long timeout;
} SerialMsg;

SerialMsg *serialMsg;

void setup() {
  Serial.begin(9600);   // safe with 9600
  Serial1.begin(9600);  // safe with 9600

  while (!Serial)
    ;  // this only work for serial interface

  // Serial.println("serial ");

  piHandler = &Serial;
  debugHandler = piHandler;

  serialMsg = NewSerialMsg(piHandler);

// piHandler->println("pi serial");

// Use default board voltage reference
// see: https://www.pjrc.com/teensy/adc.html
#ifdef ARDUINO_TEENSY41
#define DEFAULT 0
#endif
  // analogReference(DEFAULT);


  int i;
  pinMode(LED_BUILTIN, OUTPUT);  // initialise led builtin as output

  for (i = 0; i < nSwitchButtons; i++) {
    pinMode(switchButtons[i].pin, INPUT_PULLUP);
    switchButtons[i].savedValue = digitalRead(switchButtons[i].pin);
  }

  for (i = 0; i < nPotButtons; i++) {
    pinMode(potButtons[i].pin, INPUT);
    potButtons[i].savedValue = analogRead(potButtons[i].pin);
  }

  for (i = 0; i < nRotaryControls; i++) {
    pinMode(rotaryControls[i].aPin, INPUT_PULLUP);
    pinMode(rotaryControls[i].bPin, INPUT_PULLUP);
    rotaryControls[i].aState = digitalRead(rotaryControls[i].aPin);
    rotaryControls[i].aStatePrevious = rotaryControls[i].aState;
    rotaryControls[i].bState = digitalRead(rotaryControls[i].bPin);
    rotaryControls[i].bStatePrevious = rotaryControls[i].bState;
  }
}

void loop() {

  digitalWrite(LED_BUILTIN, HIGH);  // Turn indicator light on.

  int i;
  for (i = 0; i < nSwitchButtons; i++) {
    if (switchButtons[i].debounceTime > millis()) {
      continue;
    }
    switchButtons[i].value = digitalRead(switchButtons[i].pin);
    processSwitch(piHandler, &switchButtons[i]);
  }

  for (i = 0; i < nPotButtons; i++) {
    if (potButtons[i].debounceTime > millis()) {
      continue;
    }
    potButtons[i].value = analogRead(potButtons[i].pin);
    processPot(piHandler, &(potButtons[i]));
  }

  for (i = 0; i < nRotaryControls; i++) {
    if (rotaryControls[i].debounceTime > millis()) {
      continue;
    }
    rotaryControls[i].aState = digitalRead(rotaryControls[i].aPin);
    rotaryControls[i].bState = digitalRead(rotaryControls[i].bPin);
    processRotary(piHandler, &rotaryControls[i]);
  }

  readPi(piHandler);
}

void readPi(Stream *s) {
  char b[64];

  int n = ReadMsgNonBlocking(serialMsg, (char *)&b, sizeof(b) / sizeof(char));
  if (n == -1) {
    return;
  }

  if (n == 0) {
    return;
  }

  if (Debug) {
    debugHandler->print("received msg =");
    debugHandler->print(b);
    debugHandler->println();
    debugHandler->flush();
  }

  char *tok = strtok((char *)b, ",");
  if (tok == 0) {
    return;
  }
  if (strcmp(tok, "panel") == 0) {
    txPanel(s, panelName);
    return;
  } else if (strcmp(tok, "log") == 0) {
    tok = strtok(0, ",");
    if (tok == 0) {
      return;
    }
    if (strcmp(tok, "debug") == 0) {
      Debug = true;
    } else {
      Debug = false;
    }
    debugHandler->print("debug=");
    debugHandler->println(Debug);
    return;
  } else {
    debugHandler->print("unknown command: ");
    debugHandler->println(tok);
    debugHandler->flush();
  }
}
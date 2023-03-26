#include <Keyboard.h>     // Use built-in Keyboard library
#include <HID_Buttons.h>  // Must import AFTER Keyboard.h

#include "electronics.h"

// Void keyboard dependency for boards that don't have usb keyboard.
// This allow building but won't send any key strokes to the usb.
// i.e. Mega
// #define NO_KEYBOARD 1

#define DEBUG 1

// Uncomment this line to enable ascii messages to be sent via the IDE to the arduino.
// This is useful for debugging. The arduino will interpret messages starting with "A" as an ascii msg.
// for example: A234 - send a msg of 2 bytes, type 3, value 4
// #define ENABLE_ASCII_MSG

// Reserved pins
#define PIN_ON_OFF A0  // first pin after GND on Teensy 2.0++


// uncomment one of these to build the right panel
// #define LIGHTS_BOX 1
// #define FLAPS_PANEL 1
#define G1000_PANEL 1
// #define TEST_PANEL 1
// #define KEYBOARD_PANEL 1  // panel with no electronics used for keyboard

Stream *piHandler;
Stream *xboxHandler;
Print *debugHandler;

typedef struct SerialMsg {
  Stream *Port;
  uint8_t buffer[16];
  unsigned int count;
  int dataLen;
  int state;
  unsigned long timeout;
} SerialMsg;

SerialMsg *serialMsg;

#ifdef KEYBOARD_PANEL
const uint8_t KEYBOARD_FLAG = 0x80;
#else
const uint8_t KEYBOARD_FLAG = 0x00;
#endif

void setup() {
  Serial.begin(9600);   // safe with 9600
  Serial1.begin(9600);  // safe with 9600

#ifndef KEYBOARD_PANEL
  // while (!Serial)   // this only work for serial interface - not keyboard
  // ;
#endif

  Serial.println("serial ");
  Serial1.println("serial 1");

#ifdef KEYBOARD_PANEL
  piHandler = &Serial1;
  xboxHandler = &Serial;
#else
  piHandler = &Serial;
  xboxHandler = &Serial1;
#endif

  debugHandler = piHandler;

  serialMsg = NewSerialMsg(piHandler);

  // piHandler->println("pi serial");
  // xboxHandler->println("xbox serial");

  int i;
  pinMode(LED_BUILTIN, OUTPUT);       // initialise led builtin as output
  pinMode(PIN_ON_OFF, INPUT_PULLUP);  // initalise control 1 with digital resistor (built in the board)

  for (i = 0; i < nSwitchButtons; i++) {
    pinMode(switchButtons[i].pin, INPUT_PULLUP);
    switchButtons[i].savedValue = digitalRead(switchButtons[i].pin);
  }

  for (i = 0; i < npushButtons; i++) {
    pinMode(pushButtons[i].pin, INPUT_PULLUP);
    pushButtons[i].savedValue = digitalRead(pushButtons[i].pin);
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

#ifndef NO_KEYBOARD
  Keyboard.begin();
  Keyboard.releaseAll();  // release any key that is pressed
#endif
}

void loop() {

  // System Disabled?
  if (digitalRead(PIN_ON_OFF) != 0) {
    digitalWrite(LED_BUILTIN, LOW);
    return;
  }
  digitalWrite(LED_BUILTIN, HIGH);  // Turn indicator light on.

  // read all pins first
  int i;
  for (i = 0; i < nSwitchButtons; i++) {
    switchButtons[i].value = digitalRead(switchButtons[i].pin);
  }
  for (i = 0; i < nPotButtons; i++) {
    potButtons[i].value = analogRead(potButtons[i].pin);
  }
  for (i = 0; i < npushButtons; i++) {
    pushButtons[i].value = digitalRead(pushButtons[i].pin);
  }
  for (i = 0; i < nRotaryControls; i++) {
    rotaryControls[i].aState = digitalRead(rotaryControls[i].aPin);
    rotaryControls[i].bState = digitalRead(rotaryControls[i].bPin);
  }

  // process all pins
  for (i = 0; i < nSwitchButtons; i++) {
    processSwitch(piHandler, &switchButtons[i]);
  }

  for (i = 0; i < nPotButtons; i++) {
    processPot(piHandler, &(potButtons[i]));
  }

  for (i = 0; i < npushButtons; i++) {
    processPushButton(piHandler, &pushButtons[i]);
  }

  for (i = 0; i < nRotaryControls; i++) {
    processRotary(piHandler, &rotaryControls[i]);
  }

  // read pi serial port
  readPi(piHandler);

  sendKeystrokeNonBlocking();
}

void readPi(Stream *s) {
  uint8_t b[16];


  int n = ReadMsgNonBlocking(serialMsg, (uint8_t *)&b, sizeof(b) / sizeof(uint8_t));
  if (n == -1) {
    return;
  }

  if (n == 0) {
    return;
  }
#ifdef DEBUG
  debugHandler->print("received msg type=");
  debugHandler->print(b[0]);
  debugHandler->print(" len=");
  debugHandler->println(n);
#endif

  switch (b[0]) {
    case KEYSTROKES:
      if (n > 1) {
        queueKeys(&b[1], n - 1);
#ifdef DEBUG
        debugHandler->flush();
#endif
      }
      return;
    case PANEL:
      txPanel(s, panelName);
      return;
  }
}
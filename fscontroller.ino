#include "electronics.h"

bool Debug = false;
statistics stats;

// Reserved pins
#define PIN_ON_OFF A0  // first pin after GND on Teensy 2.0++

// uncomment one of these to build the right panel
// #define LIGHTS_PANEL
// #define FLAPS_PANEL
// #define G1000_PANEL
// #define TEST_PANEL
#define SINGLE_THROTTLE_QUADRANT_PANEL
// #define KX165_COM_NAV_PANEL
// #define LANDING_GEAR_PANEL

// MUST include LedControl to prevent pre-compilation error in kx165panel.ino
#ifdef KX165_COM_NAV_PANEL
#include <LedControl.h> 
#endif

Print* debugHandler;
SerialMsg* serialMsg;

//handle notification is a pointer to a function that takes a char* and returns error
int (*handleNotification)(char*) = 0;



void setup() {
  Serial.begin(115200);  // baund rate is ignored when using usb

  Stream* handler = &Serial;
  debugHandler = handler;

  serialMsg = NewSerialMsg(handler);

  int i;
  pinMode(LED_BUILTIN, OUTPUT);  // initialise led builtin as output

  for (i = 0; i < nLedOutputs; i++) {
    pinMode(ledOutputs[i].pin, OUTPUT);
    digitalWrite(ledOutputs[i].pin, LOW);
  }

  for (i = 0; i < nSwitchButtons; i++) {
    switchButtons[i].pin.update();
  }

  for (i = 0; i < nPotButtons; i++) {
    potControls[i].pin.update();
  }

  for (i = 0; i < nRotaryControls; i++) {
    rotaryControls[i].aPin.update();
    rotaryControls[i].bPin.update();
  }

  panelInit();
}

void loop() {

  digitalWrite(LED_BUILTIN, HIGH);  // Turn indicator light on.

  int i;
  for (i = 0; i < nSwitchButtons; i++) {
    if (switchButtons[i].pin.update()) {
      processSwitch(serialMsg, &switchButtons[i]);
    }
  }

  for (i = 0; i < nPotButtons; i++) {
    // TODO: updated on 14/09/2024 - not tested yet - must upgrade throttle quadrant to test
    if (potControls[i].pin.update()) {
      processPot(serialMsg, &(potControls[i]));
    }
  }

  for (i = 0; i < nRotaryControls; i++) {
    if (rotaryControls[i].aPin.update()) {
      rotaryControls[i].bPin.update();
      processRotary(serialMsg, &rotaryControls[i]);
    }

  }

  readHost(serialMsg);
}

void readHost(SerialMsg* s) {
  char b[maxMsgSize + 1];

  int n = ReadMsgNonBlocking(serialMsg, (char*)&b, sizeof(b) / sizeof(char));
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


  if (strncmp((char*)&b, panelToken, sizeof(panelToken) - 1) == 0) {
    panelConnect(s);
    return;
  }
  else if (strncmp((char*)&b, notificationToken, sizeof(notificationToken) - 1) == 0) {
    panelNotification((char*)&b);
    return;
  }
  else if (strncmp((char*)&b, testToken, sizeof(testToken) - 1) == 0) {
    // txPot(s, A0, 1023);
    // txRotary(s, 6, -1);
    txSwitch(s, 1, 1);
    return;
  }
  else if (strncmp((char*)&b, logToken, sizeof(logToken) - 1) == 0) {
    char* tok = strtok((char*)&b, ",");
    if (tok == 0) {
      return;
    }
    tok = strtok(0, ",");
    if (tok == 0) {
      return;
    }
    if (strcmp(tok, "debug") == 0) {
      Debug = true;
    }
    else {
      Debug = false;
    }
    debugHandler->print("debug=");
    debugHandler->println(Debug);
    return;
  }

  debugHandler->print("unknown command: ");
  debugHandler->println((char*)b);
  debugHandler->flush();
}
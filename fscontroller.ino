
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
// #define SINGLE_THROTTLE_QUADRANT_PANEL 1
#define LANDING_GEAR_PANEL

Print *debugHandler;
SerialMsg *serialMsg;

//handle notification is a pointer to a function that takes a char* and returns error
int (*handleNotification)(char *) = 0;

int delayAnalogRead(uint8_t pin) {
  const int n = 64; // number of samples to average
  int value = 0;

  analogRead(pin); // discard first reading
  delayMicroseconds(10);  // IMPORTANT: must delay to minimise fluctuation on the analog port
  analogRead(pin); // discard second reading
  for (int i = 0; i < n; i++) {
    value += analogRead(pin);
  }
  value = value / n;

  return value;
}

void setup() {
  Serial.begin(115200);   // baund rate is ignored when using usb

  Stream *handler = &Serial;
  debugHandler = handler;

  serialMsg = NewSerialMsg(handler);

  int i;
  pinMode(LED_BUILTIN, OUTPUT);  // initialise led builtin as output

  for (i = 0; i < nLedOutputs; i++) {
    pinMode(ledOutputs[i].pin, OUTPUT);
    digitalWrite(ledOutputs[i].pin, LOW);
  }

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

  panelInit();
}

void loop() {

  digitalWrite(LED_BUILTIN, HIGH);  // Turn indicator light on.

  int i;
  for (i = 0; i < nSwitchButtons; i++) {
    if (switchButtons[i].debounceTime > millis()) {
      continue;
    }
    switchButtons[i].value = digitalRead(switchButtons[i].pin);
    processSwitch(serialMsg, &switchButtons[i]);
  }

  for (i = 0; i < nPotButtons; i++) {
    if (potButtons[i].debounceTime > millis()) {
      continue;
    }

    potButtons[i].value = delayAnalogRead(potButtons[i].pin);
    processPot(serialMsg, &(potButtons[i]));
  }

  for (i = 0; i < nRotaryControls; i++) {
    if (rotaryControls[i].debounceTime > millis()) {
      continue;
    }
    rotaryControls[i].aState = digitalRead(rotaryControls[i].aPin);
    rotaryControls[i].bState = digitalRead(rotaryControls[i].bPin);
    processRotary(serialMsg, &rotaryControls[i]);
  }

  readHost(serialMsg);
}

void readHost(SerialMsg *s) {
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


  if (strncmp((char *)&b, panelToken, sizeof(panelToken)-1) == 0) {
    panelConnect(s);
    return;
  } else if (strncmp((char *)&b, notificationToken, sizeof(notificationToken)-1) == 0) {
    panelNotification((char *)&b);
    return;
  } else if (strncmp((char *)&b, testToken, sizeof(testToken)-1) == 0) {
    txPot(s, A0, 1023);
    txRotary(s, 6, -1);
    txSwitch(s, 1, 1);
    return;
  } else if (strncmp((char *)&b, logToken, sizeof(logToken)-1) == 0) {
    char *tok = strtok((char *)&b, ",");
    if (tok == 0) {
      return;
    }
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
  }

  debugHandler->print("unknown command: ");
  debugHandler->println((char *)b);
  debugHandler->flush();
}
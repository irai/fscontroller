

#include <Keyboard.h>     // Use built-in Keyboard library
#include <HID_Buttons.h>  // Must import AFTER Keyboard.h

// #define USE_KEYBOARD 1  // add usb keyboard calls usb keyboard boards like the Teensy


#include "electronics.h"

// #define DEBUG 1
// #define Serial \
  // if (DEBUG) Serial  // enable printing if debuging

// Avoid using reserved pins
// PIN 2 - RX
// PIN 3 - TX
// PIN 6 - Led

#define PIN_ON_OFF A0  // first pin after GND on Teensy 2.0++


// #define FIRST_BOX 1
// #define SECOND_BOX 1
// #define THIRD_BOX 1
#define SERIAL_BOX 1

#ifdef FIRST_BOX
const int nSwitchButtons = 12;
button switchButtons[nSwitchButtons] = {
  { "strobe light", 0, { &strobeLightOnKeys, &strobeLightOffKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "nav light", 1, { &navLightOnKeys, &navLightOffKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "taxi lights", 2, { &taxiLightOnKeys, &taxiLightOffKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "landing lights", 3, { &landingLightOnKeys, &landingLightOffKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "beacon light", 4, { &beaconLightOnKeys, &beaconLightOffKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "pitot heat", 5, { &pitotHeatOnKeys, &pitotHeatOffKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "fuel pump", 6, { &fuelPumpOnKeys, &fuelPumpOffKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "free", 7, { NULL, NULL, NULL, NULL }, 0, 0, 0, 0 },
  { "master", 8, { &masterKeys, &masterKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "avionics", 9, { &avionics, &avionics, NULL, NULL }, 0, 0 },
  { "gear", 10, { &gearDownKeys, &gearUpKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "carby heat", 11, { &carbHeatOnKeys, &carbHeatOffKeys, NULL, NULL }, 0, 0, 0, 0 }
};

const int nPressureButtons = 2;
button pressureButtons[nPressureButtons] = {
  { "atc", 12, { &atcKeys, NULL, NULL, NULL }, 0, 0, 0, 0 },
  { "view", 13, { &viewDashboardFirstKeys, &viewDashboardNextKeys, &viewDashboardNextKeys, NULL }, 0, 0, 0, 3 }
};

// https://docs.arduino.cc/learn/electronics/potentiometer-basics
const int nPotButtons = 3;
button potButtons[nPotButtons] = {
  { "throtle", A1, { &throtleMaxKeys, &throtleCutKeys, &throtleIncreaseKeys, &throtleDecreaseKeys }, 0, 0, 1024 / 100, 1024 },   // in 100 steps - Cesna 172
  { "mixture", A2, { &mixtureRichKeys, &mixtureLeanKeys, &mixtureIncreaseKeys, &mixtureDecreaseKeys }, 0, 0, 1024 / 65, 1024 },  // in 64 stepsC - Cesna 172
  { "propeller", A3, { &propellerHiKeys, &propellerLowKeys, &propellerIncreaseKeys, &propellerDecreaseKeys }, 0, 0, 64, 1024 }
};

const int nRotaryControls = 1;
rotary rotaryControls[nRotaryControls] = {
  { "trim", NULL, A5, &trimDown, A4, &trimUp, 0, NULL, 0, 0, 0 },
};

HardwareSerial piHandler = Serial;
Serial_ xboxHandler = Serial;


#elif SECOND_BOX

// Second controller box

// The power is inverted in the the switches, swap on/off for now

const int nSwitchButtons = 15;
button switchButtons[nSwitchButtons] = {
  { "ignition off", 2, { NULL, &magnetoOffKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "mag right", 3, { NULL, &magnetoRightKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "mag left", 4, { NULL, &magnetoLeftKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "mag both", 5, { NULL, &magnetoBothKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "ignition start", 6, { NULL, &magnetoStartKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "no flaps", 7, { NULL, &flapsRetractKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "10% flaps", 8, { NULL, &flaps10Keys, NULL, NULL }, 0, 0, 0, 0 },
  { "20% flaps", 9, { NULL, &flaps20Keys, NULL, NULL }, 0, 0, 0, 0 },
  { "30% flaps", 10, { NULL, &flapsFullKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "Parking break", 11, { &parkingBreakKeys, &parkingBreakKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "Fuel valve", 12, { &fuelValveKeys, &fuelValveKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "Fuel off", A1, { NULL, &fuelTankOff, NULL, NULL }, 0, 0, 0, 0 },
  { "Fuel left", A2, { NULL, &fuelTankLeft, NULL, NULL }, 0, 0, 0, 0 },
  { "Fuel right", A3, { NULL, &fuelTankRight, NULL, NULL }, 0, 0, 0, 0 }
  // { "Fuel pump", 13, { &strobeLightKeys, &strobeLightKeys, NULL, NULL }, 0, 0, 0, 0 },
};
const int nPressureButtons = 0;
button pressureButtons[nPressureButtons] = {};

const int nPotButtons = 0;
button potButtons[nPotButtons] = {
  // { "switch", A1, { &throtleMaxKeys, &throtleCutKeys, &throtleIncreaseKeys, &throtleDecreaseKeys }, 0, 0, 256, 1024 }  // in steps of 4
};

const int nRotaryControls = 0;
rotary rotaryControls[nRotaryControls] = {};

HardwareSerial piHandler = Serial;
Serial_ xboxHandler = Serial;

#elif THIRD_BOX
// Third BOX - G1000 - Teensy 2.0++

const int nSwitchButtons = 0;
button switchButtons[nSwitchButtons] = {};

const int nPressureButtons = 4;
button pressureButtons[nPressureButtons] = {
  { "com 1/2 switch", 1, { &com1StbSwapKeys, &com1StbSwapKeys, &com1StbSwapKeys, &com1StbSwapKeys }, 0, 0, 0, 1 },
  { "set heading bug", 11, { &setHeadingBugKeys, NULL, &setHeadingBugKeys, NULL }, 0, 0, 0, 1 },
  { "set altitude", 16, { &setHeadingBugKeys, NULL, &setHeadingBugKeys, NULL }, 0, 0, 0, 1 },
  { "swap com stdby", 26, { &com1StbSwapKeys, NULL, NULL, NULL }, 0, 0, 0, 1 }
};

const int nPotButtons = 0;
button potButtons[nPotButtons] = {};

const int nRotaryControls = 5;
rotary rotaryControls[nRotaryControls] = {
  { "com freq", &focusFrequency, 27, &com1StbFreqUpKeys, 0, &com1StbFreqDownKeys, 1, &com1StbSwapKeys, 0, 0, 0 },
  { "com freq dec", &focusFrequencyDec, 7, &com1StbFreqDecUpKeys, 8, &com1StbFreqDecDownKeys, 1, NULL, 0, 0, 0 },
  { "heading bug", &focusReset, 9, &incHeadingBugKeys, 10, &decHeadingBugKeys, 11, &setHeadingBugKeys, 0, 0, 0 },
  { "altitude", &focusReset, 12, &increaseAltKeys, 13, &decreaseAltKeys, 16, NULL, 0, 0, 0 },
  { "altitude dec", &focusReset, 14, &increaseAltKeys, 15, &decreaseAltKeys, 16, NULL, 0, 0, 0 }
};

HardwareSerial piHandler = Serial1;
Serial_ xboxHandler = Serial;

#else
// Generic serial box - Testing

const int nSwitchButtons = 1;
button switchButtons[nSwitchButtons] = {
  { "test switch", 2 }
};

const int nPressureButtons = 1;
button pressureButtons[nPressureButtons] = {
  { "pressure button", 13 }

};

const int nPotButtons = 1;
button potButtons[nPotButtons] = {
  { "test pot", A2 }
};

const int nRotaryControls = 0;
rotary rotaryControls[nRotaryControls] = {};



#endif

HardwareSerial *piHandler;
HardwareSerial *xboxHandler;


void setup() {
  // Serial.begin(115200); // safe with 9600
  Serial.begin(9600);    // safe with 9600
  Serial1.begin(9600);  // safe with 9600
  while (!Serial) ;

  Serial.println("serial ");
  Serial1.println("serial 1");

 piHandler = &Serial;
 xboxHandler = &Serial1;

piHandler->println("pi serial");
  xboxHandler->println("xbox serial");



  int i;
  pinMode(LED_BUILTIN, OUTPUT);       // initialise led builtin as output
  pinMode(PIN_ON_OFF, INPUT_PULLUP);  // initalise control 1 with digital resistor (built in the board)

  for (i = 0; i < nSwitchButtons; i++) {
    pinMode(switchButtons[i].pin, INPUT_PULLUP);
    switchButtons[i].savedValue = digitalRead(switchButtons[i].pin);
  }

  for (i = 0; i < nPressureButtons; i++) {
    pinMode(pressureButtons[i].pin, INPUT_PULLUP);
    pressureButtons[i].savedValue = digitalRead(pressureButtons[i].pin);
  }

  for (i = 0; i < nPotButtons; i++) {
    pinMode(potButtons[i].pin, INPUT);
    potButtons[i].savedValue = analogRead(potButtons[i].pin);
  }

  for (i = 0; i < nRotaryControls; i++) {
    pinMode(rotaryControls[i].aPin, INPUT_PULLUP);
    pinMode(rotaryControls[i].bPin, INPUT_PULLUP);
    // pinMode(rotaryControls[i].buttonPin, INPUT_PULLUP);
    rotaryControls[i].aState = digitalRead(rotaryControls[i].aPin);
    rotaryControls[i].aStatePrevious = rotaryControls[i].aState;
    rotaryControls[i].bState = digitalRead(rotaryControls[i].bPin);
    rotaryControls[i].bStatePrevious = rotaryControls[i].bState;
  }

#ifdef USE_KEYBOARD
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
  for (i = 0; i < nPressureButtons; i++) {
    pressureButtons[i].value = digitalRead(pressureButtons[i].pin);
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

  for (i = 0; i < nPressureButtons; i++) {
    processPressureButton(piHandler, &pressureButtons[i]);
  }

  for (i = 0; i < nRotaryControls; i++) {
    processRotary(piHandler, &rotaryControls[i]);
  }

  readPi(piHandler);


  // testSerial(&Serial);
  // testSerial(&Serial1);
}

void readPi(Stream* s) {
  int inByte;
  if (s->available() > 0) {
    // get incoming byte:
    inByte = s->read();
    Serial.print("got char=");
    Serial.println(inByte);
  }
}
#include <Keyboard.h>     // Use built-in Keyboard library
#include <HID_Buttons.h>  // Must import AFTER Keyboard.h

#include "electronics.h"
#include "simkeys.h"

#define DEBUG true
#define Serial if(DEBUG)Serial  // enable printing if debuging


// #define FIRST_BOX 1
// #define SECOND_BOX 1

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
  { "throtle", A1, { &throtleMaxKeys, &throtleCutKeys, &throtleIncreaseKeys, &throtleDecreaseKeys }, 0, 0, 1024/100, 1024 },     // in 100 steps - Cesna 172
  { "mixture", A2, { &mixtureRichKeys, &mixtureLeanKeys, &mixtureIncreaseKeys, &mixtureDecreaseKeys }, 0, 0, 1024/65, 1024 },  // in 64 stepsC - Cesna 172
  { "propeller", A3, { &propellerHiKeys, &propellerLowKeys, &propellerIncreaseKeys, &propellerDecreaseKeys }, 0, 0, 64, 1024 }
};

const int nRotaryControls = 1;
rotary rotaryControls[nRotaryControls] = {
  { "trim", A5, &trimDown, A4, &trimUp, PIN_D6, NULL, 0, 0, 0 },
};

#elif SECOND_BOX

// Second controller box

// The power is inverted in the the switches, swap on/off for now

const int nSwitchButtons = 15;
button switchButtons[nSwitchButtons] = {
  { "ignition off", 2, { NULL, &magnetoOffKeys,  NULL, NULL }, 0, 0, 0, 0 },
  { "mag right", 3, { NULL, &magnetoRightKeys,  NULL, NULL }, 0, 0, 0, 0 },
  { "mag left", 4, { NULL, &magnetoLeftKeys,  NULL, NULL }, 0, 0, 0, 0 },
  { "mag both", 5, { NULL, &magnetoBothKeys,  NULL, NULL }, 0, 0, 0, 0 },
  { "ignition start", 6, { NULL, &magnetoStartKeys,  NULL, NULL }, 0, 0, 0, 0 },
  { "no flaps", 7, { NULL, &flapsRetractKeys,  NULL, NULL }, 0, 0, 0, 0 },
  { "10% flaps", 8, { NULL, &flaps10Keys,  NULL, NULL }, 0, 0, 0, 0 },
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
rotary rotaryControls[nRotaryControls] = {
};

#else

const int nSwitchButtons = 0;
button switchButtons[nSwitchButtons] = {};

const int nPressureButtons = 0;
button pressureButtons[nPressureButtons] = {};

const int nPotButtons = 0;
button potButtons[nPotButtons] = {};

const int nRotaryControls = 1;
rotary rotaryControls[nRotaryControls] = {
  { "com freq", A1, &com1StbFreqUpKeys, A2, &com1StbFreqDownKeys, PIN_D6, &com1StbSwapKeys, 0, 0, 0 },
  //{ "com freq dec", A3, &com1StbFreqUpKeys, A4, &com1StbFreqDownKeys, PIN_D6, &com1StbSwapKeys, 0, 0, 0 }
};

#endif

void pressKey(cmd* keys, int repeat) {
  if (keys == NULL) {
    Serial.println("null keys");
    return;
  }

  int i;

  for (i = 0; i < keys->len; i++) {
#ifndef DEBUG
    if (Keyboard.press(keys->seq[i]) != 1) {
      Serial.println("error in press");
      return;
    }
    delay(8);
#endif
  }

  delay(32);  // xbox does not work without this delay; need time for os to accept key press

  for (i = keys->len; i > 0;) {
    i--;
#ifndef DEBUG
    if (Keyboard.release(keys->seq[i]) != 1) {
      Serial.println("error in release");
      Keyboard.releaseAll();  // attempt to release numpad keys - fix bug of non stop repeating key
      return;
    }
    delay(8);
#endif
  }
  delay(32);  // xbox does not work without this delay; need time for os to accept key press
}

void setup() {
  Serial.begin(9600);
  // while (!Serial) ;

  int i;
  pinMode(LED_BUILTIN, OUTPUT);  // initialise led builtin as output
  pinMode(A0, INPUT_PULLUP);     // initalise control A0 with digital resistor (built in the board)

  for (i = 0; i < nSwitchButtons; i++) {
    pinMode(switchButtons[i].pin, INPUT_PULLUP);
  }

  for (i = 0; i < nPressureButtons; i++) {
    pinMode(pressureButtons[i].pin, INPUT_PULLUP);
  }

  for (i = 0; i < nPotButtons; i++) {
    pinMode(potButtons[i].pin, INPUT);
    potButtons[i].savedValue = analogRead(potButtons[i].pin);
  }

  for (i = 0; i < nRotaryControls; i++) {
    pinMode(rotaryControls[i].aPin, INPUT_PULLUP);
    pinMode(rotaryControls[i].bPin, INPUT_PULLUP);
    pinMode(rotaryControls[i].buttonPin, INPUT_PULLUP);
    rotaryControls[i].aState = digitalRead(rotaryControls[i].aPin);
    rotaryControls[i].aStatePrevious = rotaryControls[i].aState;
    rotaryControls[i].bState = digitalRead(rotaryControls[i].bPin);
    rotaryControls[i].bStatePrevious = rotaryControls[i].bState;
  }


  Keyboard.begin();
  Keyboard.releaseAll();  // release any key that is pressed
}

void loop() {
  // System Disabled?
  if (digitalRead(A0) != 0) {
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
    delay(5);
    rotaryControls[i].bState = digitalRead(rotaryControls[i].bPin);
  }

  // process all pins
  for (i = 0; i < nSwitchButtons; i++) {
    processSwitch(&switchButtons[i]);
  }

  for (i = 0; i < nPotButtons; i++) {
    processPot(&potButtons[i]);
  }

  for (i = 0; i < nPressureButtons; i++) {
    processPressureButton(&pressureButtons[i]);
  }

  for (i = 0; i < nRotaryControls; i++) {
    processRotary(&rotaryControls[i]);
  }
}

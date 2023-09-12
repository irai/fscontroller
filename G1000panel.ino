#ifdef G1000_PANEL
// G1000 Panel - Teensy 2.0++

button switchButtons[] = {
  // { 0 },  // rotary
  { 1 },
  { 2 },
  { 3 },
  { 4 },
  { 5 },
  { 6 },
  // { 7},  rotary
  // { 8 }, rotary
  // { 9 }, rotary
  // { 10 }, rotary
  { 11 },
  // { 12 }, rotary
  // { 13 }, rotary
  // { 14 }, rotary
  // { 15 }, rotary
  { 16 },
  { 17 },
  { 18 },
  { 19 },
  { 20 },
  { 21 },
  { 22 },
  { 23 },
  { 24 },
  { 25 },
  { 26 },
  // { 27 }, rotary
  { 28 },
  { 29 },
  { 30 },
  { 31 },
  { 32 },
  { 33 },
  { 34 },
  { 35 },
  { 36 },
  { 37 },
  { 38 },
  { 39 },
  { 40 },
  { 41 },
  { 42 },
  { 43 },
  { 44 },
  { 45 },
  { 46 },
  { 47 },
  { 48 },
  { 49 },
  { 50 },
  { 51 },
  { 52 },
  { 53 },
  { 54 },  // Analog pin PIN_A0
  { 55 },  // Analog pin PIN_A1
  { 56 },  // Analog pin PIN_A2
  { 57 },  // Analog pin PIN_A3
  { 58 },  // Analog pin PIN_A4
  { 59 },  // Analog pin PIN_A5
  { 60 },  // Analog pin PIN_A6
  { 61 }   // Analog pin PIN_A7
};

button potButtons[] = {};

rotary rotaryControls[] = {
  { .aPin = 27, .bPin = 0, .buttonPin = 1 },
  { .aPin = 7, .bPin = 8, .buttonPin = 1 },
  { .aPin = 9, .bPin = 10, .buttonPin = 11 },
  { .aPin = 12, .bPin = 13, .buttonPin = 16 },
  { .aPin = 14, .bPin = 15, .buttonPin = 16 }
};

const int nSwitchButtons = sizeof(switchButtons) / sizeof(button);
const int nPotButtons = sizeof(potButtons) / sizeof(button);
const int nRotaryControls = sizeof(rotaryControls) / sizeof(rotary);

const char* panelName = "G1000Panel";

#endif

#ifdef G1000_PANEL
// G1000 Panel - Teensy 2.0++

button switchButtons[] = {
  { 1 },
  { 11 },
  { 16 },
  { 26 },
};

button potButtons[] = {};

rotary rotaryControls[] = {
  { .aPin = 27, .bPin = 0, .buttonPin = 1},
  { .aPin = 7, .bPin = 8, .buttonPin = 1},
  { .aPin = 9, .bPin = 10, .buttonPin = 11},
  { .aPin = 12, .bPin = 13, .buttonPin = 16},
  { .aPin = 14, .bPin = 15, .buttonPin = 16}
};

const int nSwitchButtons = sizeof(switchButtons) / sizeof(button);
const int nPotButtons = sizeof(potButtons) / sizeof(button);
const int nRotaryControls = sizeof(rotaryControls) / sizeof(rotary);

const String panelName = "G1000Panel";

#endif

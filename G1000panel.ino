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
  { 27, 0, 1, 0, 0, 0, 0, 0 },
  { 7, 8, 1, 0, 0, 0, 0, 0 },
  { 9, 10, 11, 0, 0, 0, 0, 0 },
  { 12, 13, 16, 0, 0, 0, 0, 0 },
  { 14, 15, 16, 0, 0, 0, 0, 0 }
};

const int nSwitchButtons = sizeof(switchButtons) / sizeof(button);
const int nPotButtons = sizeof(potButtons) / sizeof(button);
const int nRotaryControls = sizeof(rotaryControls) / sizeof(rotary);

const String panelName = "G1000Panel";

#endif

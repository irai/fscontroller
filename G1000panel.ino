#ifdef G1000_PANEL
// G1000 Panel - Teensy 2.0++

button switchButtons[] = {};

button pushButtons[] = {
  { "com 1/2 switch", 1},
  { "set heading bug", 11},
  { "set altitude", 16},
  { "swap com stdby", 26},
};

button potButtons[] = {};

rotary rotaryControls[] = {
  { "com freq", 27, 0, 1, 0,0,0,0,0  },
  { "com freq dec", 7, 8, 1, 0,0, 0, 0, 0 },
  { "heading bug", 9, 10, 11, 0,0,0, 0, 0 },
  { "altitude", 12, 13, 16, 0,0, 0, 0, 0 },
  { "altitude dec", 14, 15, 16, 0,0, 0, 0, 0 }
};

const int nSwitchButtons = sizeof(switchButtons)/sizeof(button);
const int npushButtons = sizeof(pushButtons)/sizeof(button);
const int nPotButtons = sizeof(potButtons)/sizeof(button);
const int nRotaryControls = sizeof(rotaryControls)/sizeof(rotary);

const String panelName = "G1000Panel";

#endif

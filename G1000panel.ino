#ifdef G1000_PANEL
// G1000 Panel - Teensy 2.0++

button switchButtons[] = {};

button pressureButtons[] = {
  { "com 1/2 switch", 1, { &com1StbSwapKeys, &com1StbSwapKeys, &com1StbSwapKeys, &com1StbSwapKeys }, 0, 0, 0, 1 },
  { "set heading bug", 11, { &setHeadingBugKeys, NULL, &setHeadingBugKeys, NULL }, 0, 0, 0, 1 },
  { "set altitude", 16, { &setHeadingBugKeys, NULL, &setHeadingBugKeys, NULL }, 0, 0, 0, 1 },
  { "swap com stdby", 26, { &com1StbSwapKeys, NULL, NULL, NULL }, 0, 0, 0, 1 }
};

button potButtons[] = {};

rotary rotaryControls[] = {
  { "com freq", &focusFrequency, 27, &com1StbFreqUpKeys, 0, &com1StbFreqDownKeys, 1, &com1StbSwapKeys, 0, 0, 0 },
  { "com freq dec", &focusFrequencyDec, 7, &com1StbFreqDecUpKeys, 8, &com1StbFreqDecDownKeys, 1, NULL, 0, 0, 0 },
  { "heading bug", &focusReset, 9, &incHeadingBugKeys, 10, &decHeadingBugKeys, 11, &setHeadingBugKeys, 0, 0, 0 },
  { "altitude", &focusReset, 12, &increaseAltKeys, 13, &decreaseAltKeys, 16, NULL, 0, 0, 0 },
  { "altitude dec", &focusReset, 14, &increaseAltKeys, 15, &decreaseAltKeys, 16, NULL, 0, 0, 0 }
};

const int nSwitchButtons = sizeof(switchButtons)/sizeof(button);
const int nPressureButtons = sizeof(pressureButtons)/sizeof(button);
const int nPotButtons = sizeof(potButtons)/sizeof(button);
const int nRotaryControls = sizeof(rotaryControls)/sizeof(rotary);

const String panelName = "G1000";

// HardwareSerial piHandler = Serial1;
// Serial_ xboxHandler = Serial;

#endif

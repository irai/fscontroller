#ifdef TEST_PANEL
// Generic serial box - Testing

const int nSwitchButtons = 1;
button switchButtons[nSwitchButtons] = {
  { "test switch", 2 }
};

const int nPressureButtons = 2;
button pressureButtons[nPressureButtons] = {
  { "pressure button 2", 12 },
  { "pressure button 1", 13 }

};

const int nPotButtons = 1;
button potButtons[nPotButtons] = {
  { "test pot", A2 }
};

const int nRotaryControls = 0;
rotary rotaryControls[nRotaryControls] = {};


#endif

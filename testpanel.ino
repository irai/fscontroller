#ifdef TEST_PANEL
// Generic serial box - Testing

button switchButtons[] = {
  { "switch", 2 }
};

button pressureButtons[] = {
  { "pressure button 2", 12 },
  { "pressure button 1", 10 }
};


button potButtons[] = {
  { "test pot", A2 }
};

rotary rotaryControls[] = {};


const int nSwitchButtons = sizeof(switchButtons)/sizeof(button);
const int nPressureButtons = sizeof(pressureButtons)/sizeof(button);
const int nPotButtons = sizeof(potButtons)/sizeof(button);
const int nRotaryControls = sizeof(rotaryControls)/sizeof(rotary);

const String panelName = "TestPanel";
#endif

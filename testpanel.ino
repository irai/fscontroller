#ifdef TEST_PANEL
// Generic serial box - Testing
button switchButtons[] = {
  { "switch", 39 }
};

button pushButtons[] = {
  { "pressure button 2", 33 },
  { "pressure button 1", 34 }
};


button potButtons[] = {
  // { "test pot", A2 }
};

rotary rotaryControls[] = {};


const int nSwitchButtons = sizeof(switchButtons)/sizeof(button);
const int npushButtons = sizeof(pushButtons)/sizeof(button);
const int nPotButtons = sizeof(potButtons)/sizeof(button);
const int nRotaryControls = sizeof(rotaryControls)/sizeof(rotary);

const String panelName = "TestPanel";
#endif


#ifdef KEYBOARD_PANEL
// Panel with no electronics - used for Keyboard arduino
button switchButtons[] = {
};

button pushButtons[] = {
};

button potButtons[] = {
};

rotary rotaryControls[] = {
};

const int nSwitchButtons = sizeof(switchButtons)/sizeof(button);
const int npushButtons = sizeof(pushButtons)/sizeof(button);
const int nPotButtons = sizeof(potButtons)/sizeof(button);
const int nRotaryControls = sizeof(rotaryControls)/sizeof(rotary);
const String panelName = "KeyboardPanel";

#endif

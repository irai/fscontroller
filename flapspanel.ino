#ifdef FLAPS_PANEL

// Second controller box

// The power is inverted in the the switches, swap on/off for now

button switchButtons[] = {
  { 2},
  { 3},
  { 4},
  { 5},
  { 6},
  {  7},
  { 8},
  {  9},
  {  10},
  {  11},
  {  12},
  {  13},
  {  A1},
  {  A2},
  {  A3}
};

button potButtons[] = {
};

rotary rotaryControls[] = {};

const int nSwitchButtons = sizeof(switchButtons)/sizeof(button);
const int nPotButtons = sizeof(potButtons)/sizeof(button);
const int nRotaryControls = sizeof(rotaryControls)/sizeof(rotary);

const String panelName = "FlapsPanel";

#endif
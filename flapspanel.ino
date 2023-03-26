#ifdef FLAPS_PANEL

// Second controller box

// The power is inverted in the the switches, swap on/off for now

button switchButtons[] = {
  { "ignition off", 2},
  { "mag right", 3},
  { "mag left", 4},
  { "mag both", 5},
  { "ignition start", 6},
  { "no flaps", 7},
  { "10% flaps", 8},
  { "20% flaps", 9},
  { "30% flaps", 10},
  { "Parking break", 11},
  { "Fuel valve", 12},
  { "empty", 13},
  { "Fuel off", A1},
  { "Fuel left", A2},
  { "Fuel right", A3}
  // { "Fuel pump", 13, { &strobeLightKeys, &strobeLightKeys, NULL, NULL }, 0, 0, 0, 0 },
};

button pushButtons[] = {};

button potButtons[] = {
  // { "switch", A1, { &throtleMaxKeys, &throtleCutKeys, &throtleIncreaseKeys, &throtleDecreaseKeys }, 0, 0, 256, 1024 }  // in steps of 4
};

rotary rotaryControls[] = {};

const int nSwitchButtons = sizeof(switchButtons)/sizeof(button);
const int npushButtons = sizeof(pushButtons)/sizeof(button);
const int nPotButtons = sizeof(potButtons)/sizeof(button);
const int nRotaryControls = sizeof(rotaryControls)/sizeof(rotary);

const String panelName = "FlapsPanel";

#endif
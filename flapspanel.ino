#ifdef FLAPS_PANEL

// Second controller box

// The power is inverted in the the switches, swap on/off for now

const int nSwitchButtons = 15;
button switchButtons[nSwitchButtons] = {
  { "ignition off", 2, { NULL, &magnetoOffKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "mag right", 3, { NULL, &magnetoRightKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "mag left", 4, { NULL, &magnetoLeftKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "mag both", 5, { NULL, &magnetoBothKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "ignition start", 6, { NULL, &magnetoStartKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "no flaps", 7, { NULL, &flapsRetractKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "10% flaps", 8, { NULL, &flaps10Keys, NULL, NULL }, 0, 0, 0, 0 },
  { "20% flaps", 9, { NULL, &flaps20Keys, NULL, NULL }, 0, 0, 0, 0 },
  { "30% flaps", 10, { NULL, &flapsFullKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "Parking break", 11, { &parkingBreakKeys, &parkingBreakKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "Fuel valve", 12, { &fuelValveKeys, &fuelValveKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "Fuel off", A1, { NULL, &fuelTankOff, NULL, NULL }, 0, 0, 0, 0 },
  { "Fuel left", A2, { NULL, &fuelTankLeft, NULL, NULL }, 0, 0, 0, 0 },
  { "Fuel right", A3, { NULL, &fuelTankRight, NULL, NULL }, 0, 0, 0, 0 }
  // { "Fuel pump", 13, { &strobeLightKeys, &strobeLightKeys, NULL, NULL }, 0, 0, 0, 0 },
};
const int nPressureButtons = 0;
button pressureButtons[nPressureButtons] = {};

const int nPotButtons = 0;
button potButtons[nPotButtons] = {
  // { "switch", A1, { &throtleMaxKeys, &throtleCutKeys, &throtleIncreaseKeys, &throtleDecreaseKeys }, 0, 0, 256, 1024 }  // in steps of 4
};

const int nRotaryControls = 0;
rotary rotaryControls[nRotaryControls] = {};

const String panelName = "Flaps";
// HardwareSerial piHandler = Serial;
// Serial_ xboxHandler = Serial;
#endif
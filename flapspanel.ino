#ifdef FLAPS_PANEL

// Second controller box

led ledOutputs[] = {
};

// The power is inverted in the the switches, swap on/off for now

// NOTE: Flaps panel uses Leonardo board 
//       Leonardo compiler MUST have all fields in the same order as the structure
button switchButtons[] = {
  // { .pin = 2, .fireLow = false, .value = 0, .savedValue = 0, .debounceTime = 0 }, // both mag
  // { .pin = 3, .fireLow = false, .value = 0, .savedValue = 0, .debounceTime = 0}, // right mag
  // { .pin = 4, .fireLow = false, .value = 0, .savedValue = 0, .debounceTime = 0}, // left mag
  // { .pin = 5, .fireLow = false, .value = 0, .savedValue = 0, .debounceTime = 0},
  // { .pin = 6, .fireLow = false, .value = 0, .savedValue = 0, .debounceTime = 0},
  { .pin = 7, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "flaps", .index = -1, .setValue = 0},
  { .pin = 8, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "flaps", .index = -1, .setValue = 1},
  { .pin = 9, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "flaps", .index = -1, .setValue = 2},
  { .pin = 10,.fireLow = true,  .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "flaps", .index = -1, .setValue = 3},
  { .pin = 11,.fireLow = false,  .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "parking_brake", .index = -1, .setValue = -9999},
  { .pin = 12,.fireLow = false,  .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "carb_heat", .index = -1, .setValue = -9999},
  { .pin = 13,.fireLow = false,  .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set_n", .variable = "fuel_pump", .index = 1, .setValue = -9999},
  { .pin = A1,.fireLow = true,  .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "fuel_tank_selector", .index = -1, .setValue = 0}, // both
  { .pin = A2,.fireLow = true,  .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "fuel_tank_selector", .index = -1, .setValue = 1}, // left
  { .pin = A3,.fireLow = true,  .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "fuel_tank_selector", .index = -1, .setValue = 2} // right
};

pot potControls[] = {
};

rotary rotaryControls[] = {};

const int nLedOutputs = sizeof(ledOutputs) / sizeof(led);
const int nSwitchButtons = sizeof(switchButtons)/sizeof(button);
const int nPotButtons = sizeof(potButtons)/sizeof(button);
const int nRotaryControls = sizeof(rotaryControls)/sizeof(rotary);

const char* panelName = "FlapsPanel";
const char *panelVersion = "1.0.0";

int panelInit() {
  return 0;
}

void panelConnect(SerialMsg *s) {
    txPanel(s, panelName, panelVersion);
}

void panelNotification(char *msg) {
}

#endif
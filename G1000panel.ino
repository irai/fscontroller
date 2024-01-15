#ifdef G1000_PANEL
// G1000 Panel - Teensy 2.0++

const char *panelName = "G1000Panel";
const char *panelVersion = "1.0.0";

button ledOutputs[] = {};

button switchButtons[] = {
  // { 0 },  // rotary
  { .pin = 1, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "com_radio_active_toggle", .index = -1, .setValue = 1 },
  { .pin = 11, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "heading_bug", .index = -1, .setValue = 0 },
  { .pin = 26, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set_n", .variable = "com_radio_standby_swap", .index = 0, .setValue = 1 },
  { .pin = 40, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "ap_master", .index = -1, .setValue = 1 },
  { .pin = 41, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "ap_heading_hold", .index = -1, .setValue = 1 },
  { .pin = 21, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "ap_altitude_hold", .index = -1, .setValue = 1 },
  { .pin = 42, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "ap_nav_hold", .index = -1, .setValue = 1 },
  { .pin = 43, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "ap_approach_hold", .index = -1, .setValue = 1 },
  { .pin = 44, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "ap_vertical_speed_hold", .index = -1, .setValue = 1 }, // pin not firing????
  { .pin = 45, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "ap_vertical_speed_hold", .index = -1, .setValue = 1 },
  { .pin = 20, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "ap_flight_dir_hold", .index = -1, .setValue = 1 },
  { .pin = 22, .fireLow = false, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "pin_22", .index = -1, .setValue = 1 },
  { .pin = 23, .fireLow = false, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "pin_23", .index = -1, .setValue = 1 },
  { .pin = 24, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "inc", .variable = "ap_vertical_speed", .index = -1, .setValue = 1 },
  { .pin = 25, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "inc", .variable = "ap_vertical_speed", .index = -1, .setValue = -1 }
};

button potButtons[] = {};

rotary rotaryControls[] = {
  { .aPin = 7, .bPin = 8, .buttonPin = 1, 0, 0, 0, 0, 0, .action = "inc_n", .variable = "com_radio_freq_fract", .index = 0 },
  { .aPin = 27, .bPin = 0, .buttonPin = 1, 0, 0, 0, 0, 0, .action = "inc_n", .variable = "com_radio_freq_whole", .index = 0 },
  { .aPin = 9, .bPin = 10, .buttonPin = 11, 0, 0, 0, 0, 0, .action = "inc", .variable = "heading_bug", .index = -1 },
  { .aPin = 12, .bPin = 13, .buttonPin = 16, 0, 0, 0, 0, 0, .action = "inc", .variable = "ap_altitude", .index = -1 }, // big increment
  { .aPin = 14, .bPin = 15, .buttonPin = 16, 0, 0, 0, 0, 0, .action = "inc", .variable = "pin_14", .index = -1 } // small increment
};

const int nLedOutputs = sizeof(ledOutputs) / sizeof(button);
const int nSwitchButtons = sizeof(switchButtons) / sizeof(button);
const int nPotButtons = sizeof(potButtons) / sizeof(button);
const int nRotaryControls = sizeof(rotaryControls) / sizeof(rotary);

int panelInit() {
  return 0;
}

void panelConnect(SerialMsg *s) {
  txPanel(s, panelName, panelVersion);
}

void panelNotification(char *msg) {
}

#endif

#ifdef G1000_PANEL
// G1000 Panel - Teensy 2.0++

const char *panelName = "G1000Panel";
const char *panelVersion = "1.0.0";

led ledOutputs[] = {};

button switchButtons[] = {
  { .pin = DigitalPin(1,8), .fireLow = true, .action = "set", .variable = "com_radio_active_toggle", .index = -1, .setValue = 1 },
  { .pin = DigitalPin(11,8), .fireLow = true, .action = "set", .variable = "heading_bug", .index = -1, .setValue = 0 },
  { .pin = DigitalPin(26,8), .fireLow = true, .action = "set_n", .variable = "com_radio_standby_swap", .index = 0, .setValue = 1 },
  { .pin = DigitalPin(40,8), .fireLow = true, .action = "set", .variable = "ap_master", .index = -1, .setValue = 1 },
  { .pin = DigitalPin(41,8), .fireLow = true, .action = "set", .variable = "ap_heading_hold", .index = -1, .setValue = 1 },
  { .pin = DigitalPin(21,8), .fireLow = true, .action = "set", .variable = "ap_altitude_hold", .index = -1, .setValue = 1 },
  { .pin = DigitalPin(42,8), .fireLow = true, .action = "set", .variable = "ap_nav_hold", .index = -1, .setValue = 1 },
  { .pin = DigitalPin(43,8), .fireLow = true, .action = "set", .variable = "ap_approach_hold", .index = -1, .setValue = 1 },
  { .pin = DigitalPin(44,8), .fireLow = true, .action = "set", .variable = "ap_vertical_speed_hold", .index = -1, .setValue = 1 }, // pin not firing????
  { .pin = DigitalPin(45,8), .fireLow = true, .action = "set", .variable = "ap_vertical_speed_hold", .index = -1, .setValue = 1 },
  { .pin = DigitalPin(20,8), .fireLow = true, .action = "set", .variable = "ap_flight_dir_hold", .index = -1, .setValue = 1 },
  { .pin = DigitalPin(22,8), .fireLow = false, .action = "set", .variable = "pin_22", .index = -1, .setValue = 1 },
  { .pin = DigitalPin(23,8), .fireLow = false, .action = "set", .variable = "pin_23", .index = -1, .setValue = 1 },
  { .pin = DigitalPin(24,8), .fireLow = true, .action = "inc", .variable = "ap_vertical_speed", .index = -1, .setValue = 1 },
  { .pin = DigitalPin(25,8), .fireLow = true, .action = "inc", .variable = "ap_vertical_speed", .index = -1, .setValue = -1 }
};

pot potControls[] = {};

rotary rotaryControls[] = {
  { .aPin = DigitalPin(7,0), .bPin = DigitalPin(8,0), .action = "inc_n", .variable = "com_radio_freq_fract", .index = 0 },
  { .aPin = DigitalPin(27,0), .bPin = DigitalPin(0,0), .action = "inc_n", .variable = "com_radio_freq_whole", .index = 0 },
  { .aPin = DigitalPin(9,0), .bPin = DigitalPin(10,0), .action = "inc", .variable = "heading_bug", .index = -1 },
  { .aPin = DigitalPin(12,0), .bPin = DigitalPin(13,0), .action = "inc", .variable = "ap_altitude", .index = -1 }, // big increment
  { .aPin = DigitalPin(14,0), .bPin = DigitalPin(15,0), .action = "inc", .variable = "pin_14", .index = -1 } // small increment
};

const int nLedOutputs = sizeof(ledOutputs) / sizeof(led);
const int nSwitchButtons = sizeof(switchButtons) / sizeof(button);
const int nPotButtons = sizeof(potControls) / sizeof(pot);
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

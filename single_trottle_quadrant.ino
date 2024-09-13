#ifdef SINGLE_THROTTLE_QUADRANT_PANEL

const char *panelName = "single.throttle.quadrant";
const char *panelVersion = "1.0.0";

led ledOutputs[] = {};

button switchButtons[] = {};

pot potControls[] = {
  { .pin = A0, .action = "set_n", .variable = "throttle", .index = 0 },
  { .pin = A1, .action = "set_n", .variable = "propeller_pitch", .index = 0 },
  { .pin = A2, .action = "set_n", .variable = "mixture", .index = 0 }
};

rotary rotaryControls[] = {
  { .aPin = 2, .bPin = 3, .buttonPin = 5, .action = "inc", .variable = "elevator_trim", .index = -1 }
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

// handleNotification is called every time the panel receives a notification
void panelNotification(char *msg) {
}


#endif
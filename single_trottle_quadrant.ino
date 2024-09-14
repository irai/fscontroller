#ifdef SINGLE_THROTTLE_QUADRANT_PANEL

const char *panelName = "single.throttle.quadrant";
const char *panelVersion = "1.0.0";

led ledOutputs[] = {};

button switchButtons[] = {};

pot potControls[] = {
  { .pin = AnalogPin(A0,4), .action = "set_n", .variable = "throttle", .index = 0 },
  { .pin = AnalogPin(A1,4), .action = "set_n", .variable = "propeller_pitch", .index = 0 },
  { .pin = AnalogPin(A2,4), .action = "set_n", .variable = "mixture", .index = 0 }
};

rotary rotaryControls[] = {
  { .aPin = DigitalPin(2,2), .bPin = DigitalPin(3,2), .action = "inc", .variable = "elevator_trim", .index = -1 }
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
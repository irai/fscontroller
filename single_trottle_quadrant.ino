#ifdef SINGLE_THROTTLE_QUADRANT_PANEL

const char* panelName = "single.throttle.quadrant";
const char *panelVersion = "1.0.0";

button ledOutputs[] = {
};

button switchButtons[] = {
};

button potButtons[] = {
  {  .pin = A0}, 
   { .pin = A1},
   { .pin = A2} 
};

rotary rotaryControls[] = {
   { .aPin = 2, .bPin = 3, .buttonPin = 5}
};


const int nLedOutputs = sizeof(ledOutputs) / sizeof(button);
const int nSwitchButtons = sizeof(switchButtons)/sizeof(button);
const int nPotButtons = sizeof(potButtons)/sizeof(button);
const int nRotaryControls = sizeof(rotaryControls)/sizeof(rotary);

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
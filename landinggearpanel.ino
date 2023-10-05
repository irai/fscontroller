#ifdef LANDING_GEAR_PANEL
const int centerGearPin = 2;
const int leftGearPin = 3;
const int rightGearPin = 4;
const int motorPin = 5;

button ledOutputs[] = {
  { centerGearPin },
  { leftGearPin },
  { rightGearPin },
  { motorPin },
};

button switchButtons[] = {
  { 8 }
};

button potButtons[] = {};

rotary rotaryControls[] = {};

const int nLedOutputs = sizeof(ledOutputs) / sizeof(button);
const int nSwitchButtons = sizeof(switchButtons) / sizeof(button);
const int nPotButtons = sizeof(potButtons) / sizeof(button);
const int nRotaryControls = sizeof(rotaryControls) / sizeof(rotary);

const char *panelName = "landing.gear.panel";
const char *panelVersion = "1.0.0";

// initPanel is called at setup to initialise any panel specific variables
int panelInit() {
  return 0;
}

void panelConnect(SerialMsg *s) {
    txPanel(s, panelName, panelVersion);
    txNotification(s, "LANDING_GEAR");
}

// handleNotification is called every time the panel receives a notification
void panelNotification(char *msg) {
  char *tok = strtok(msg, ",");
  if (tok == 0 || strcmp(tok, "notification") != 0) {
    return;
  }

  // notification,LANDING_GEAR,up/down,up/down,up/down  - center, left, right
  tok = strtok(0, ",");
  if (tok == 0 || strcmp(tok, "LANDING_GEAR") != 0) {
    debugHandler->print("invalid notification ");
    debugHandler->println(tok);
    return;
  }

  char *gears[3];
  for (int i = 0; i < 3; i++) {
    tok = strtok(0, ",");
    if (tok == 0) {
      debugHandler->println("invalid notification: missing center,left,right gear state");
      return;
    }
    gears[i] = tok;
  }

  if (strcmp(gears[0], "up") == 0 && strcmp(gears[1], "up") == 0 && strcmp(gears[2], "up") == 0) {
    digitalWrite(centerGearPin, LOW);
    digitalWrite(leftGearPin, LOW);
    digitalWrite(rightGearPin, LOW);
    digitalWrite(motorPin, LOW);
    debugHandler->println("gear up");
    return;
  }
  if (strcmp(gears[0], "down") == 0 && strcmp(gears[1], "down") == 0 && strcmp(gears[2], "down") == 0) {
    digitalWrite(centerGearPin, HIGH);
    digitalWrite(leftGearPin, HIGH);
    digitalWrite(rightGearPin, HIGH);
    digitalWrite(motorPin, LOW);
    debugHandler->println("gear down");
    return;
  }
  digitalWrite(centerGearPin, LOW);
  digitalWrite(leftGearPin, LOW);
  digitalWrite(rightGearPin, LOW);
  digitalWrite(motorPin, HIGH);
  debugHandler->println("gear moving");
}


#endif
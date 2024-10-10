
#ifdef KX165_COM_NAV_PANEL
#include <LedControl.h>

// working firmware to match pcb fshub-nav-0.2 Oct 2024
const char* panelName = "kx165.com.nav.panel";
const char* panelVersion = "1.0.2";


/*
 DataIn, CLK, LOAD, number of devices
 */
LedControl comDisplay = LedControl(15, 14, 13, 4); // serial data, clock, load, number of devices
led ledOutputs[] = {};


button switchButtons[] = {
  {.pin = DigitalPin(2,4), .action = "set_n", .function = &turnOnCom1, .variable = "index 1", .index = -1, .setValue = 0}, // radio 1  
  {.pin = DigitalPin(3,4), .action = "set_n", .function = &turnOnCom2, .variable = "index 2", .index = -1, .setValue = 0}, // radio 2
  {.pin = DigitalPin(4,4), .action = "set_n", .function = &turnOnCom3, .variable = "index 3", .index = -1, .setValue = 0}, // radio 3
  {.pin = DigitalPin(10,4), .action = "set_n", .function = &defaultSwitchFireOnLowFunction, .variable = "nav_radio_standby_swap", .index = -1, .setValue = 0},
  {.pin = DigitalPin(22,4), .action = "set_n", .function = &defaultSwitchFireOnLowFunction, .variable = "com_radio_standby_swap", .index = -1, .setValue = 0},
};

pot potControls[] = {};

rotary rotaryControls[] = {
  {.aPin = DigitalPin(19, 2), .bPin = DigitalPin(18,2), .action = "inc_n", .function = &defaultRotaryFunction, .variable = "com_radio_freq_whole", .index = 0 },
  {.aPin = DigitalPin(21, 2), .bPin = DigitalPin(20,2), .action = "inc_n", .function = &defaultRotaryFunction, .variable = "com_radio_freq_fract", .index = 0 },
  {.aPin = DigitalPin(12, 2), .bPin = DigitalPin(11,2), .action = "inc_n", .function = &defaultRotaryFunction, .variable = "nav_radio_freq_whole", .index = 0 },
  {.aPin = DigitalPin(9, 2), .bPin = DigitalPin(8,2), .action = "inc_n", .function = &defaultRotaryFunction, .variable = "nav_radio_freq_fract", .index = 0 },
};

const int nLedOutputs = sizeof(ledOutputs) / sizeof(led);
const int nSwitchButtons = sizeof(switchButtons) / sizeof(button);
const int nPotButtons = sizeof(potControls) / sizeof(pot);
const int nRotaryControls = sizeof(rotaryControls) / sizeof(rotary);

const int com = 0;
const int comStby = 1;
const int nav = 2;
const int navStby = 3;

int kx165Index = 0;
int kx165On = false;

void turnOff() {
  kx165On = false;
  comDisplay.shutdown(com, true);
  comDisplay.setIntensity(com, 0);
  comDisplay.shutdown(comStby, true);
  comDisplay.setIntensity(comStby, 0);
  comDisplay.shutdown(nav, true);
  comDisplay.setIntensity(nav, 0);
  comDisplay.shutdown(navStby, true);
  comDisplay.setIntensity(navStby, 0);
}

void turnOn() {
  if (kx165On) { // can be called multiple times when you switch between radio1, radio2, radio3
    return;
  }
  kx165On = true;

  comDisplay.shutdown(com, false);
  comDisplay.setIntensity(com, 1);
  comDisplay.shutdown(comStby, false);
  comDisplay.setIntensity(comStby, 1);
  comDisplay.shutdown(nav, false);
  comDisplay.setIntensity(nav, 1);
  comDisplay.shutdown(navStby, false);
  comDisplay.setIntensity(navStby, 1);
  displayFrequency(comDisplay, com, "---.--");
  displayFrequency(comDisplay, comStby, "---.--");
  displayFrequency(comDisplay, nav, "---.--");
  displayFrequency(comDisplay, navStby, "---.--");
}


void turnOnCom1(SerialMsg* s, button* b) {
  if (b->pin.getState() == LOW) {
    turnOn();
    updateIndex(1);
    return;
  }
  if (kx165Index == 1) {
    updateIndex(0);
    turnOff();
  }
}

void turnOnCom2(SerialMsg* s, button* b) {
  if (b->pin.getState() == LOW) {
    turnOn();
    updateIndex(2);
    return;
  }
}

void turnOnCom3(SerialMsg* s, button* b) {
  if (b->pin.getState() == LOW) {
    turnOn();
    updateIndex(3);
    return;
  }
}

void updateIndex(int index) {
  for (int i = 0; i < nSwitchButtons; i++) {
    switchButtons[i].index = index;
  }
  for (int i = 0; i < nRotaryControls; i++) {
    rotaryControls[i].index = index;
  }
  for (int i = 0; i < nPotButtons; i++) {
    potControls[i].index = index;
  }
  kx165Index = index;
}


//NOTE: There is a bug in the arduino IDE pre-compilation that requires include of LedCOntrol in the main 
// fscontroller.ino file to prevent pre-compilation error when passing a reference to LedControl.
void displayFrequency(LedControl& display, int com, const char* frequency) {
  int len = strlen(frequency);
  int displayIndex = 0;
  bool dp = false;

  // Display each character of the frequency string in reverse order
  for (int i = len - 1; i >= 0; i--) {
    if (frequency[i] == '.') {
      dp = true; // Skip the decimal point, but set the decimal point on the next digit
    }
    else {
      display.setChar(com, displayIndex, frequency[i], dp);
      dp = false;
      displayIndex++;
    }
  }
}


// initPanel is called at setup to initialise any panel specific variables
int panelInit() {
  turnOff();
  // TODO: fix this hack to turn on
  if (switchButtons[0].pin.getPreviousState() == LOW) {
    turnOnCom1(serialMsg, &switchButtons[0]);
  }
  if (switchButtons[1].pin.getPreviousState() == LOW) {
    turnOnCom2(serialMsg, &switchButtons[1]);
  }
  if (switchButtons[2].pin.getPreviousState() == LOW) {
    turnOnCom3(serialMsg, &switchButtons[2]);
  }
  return 0;
}

void panelConnect(SerialMsg* s) {
  txPanel(s, panelName, panelVersion);
  txNotification(s, "RADIO");
}

// handleNotification is called every time the panel receives a notification
void panelNotification(char* msg) {

  // msg format:
  //
  // 	"notification", "RADIO",
  // ap.Com1ActiveFreq,
  // ap.Com1StandbyFreq,
  // ap.Com2ActiveFreq,
  // ap.Com2StandbyFreq,
  // ap.ComName,
  // ap.Transponder,
  // strconv.Itoa(int(ap.TransponderState)),
  // ap.Nav1ActiveFreq,
  // ap.Nav1StandbyFreq,
  // ap.Nav2ActiveFreq,
  // ap.Nav2StandbyFreq,
  // ap.NavName,

  char* tok = strtok(msg, ",");
  if (tok == 0 || strcmp(tok, "notification") != 0) {
    return;
  }
  tok = strtok(NULL, ",");
  if (tok == 0 || strcmp(tok, "RADIO") != 0) {
    debugHandler->print("invalid radio notification");
    return;
  }

  char* com1ActiveFreqStr = strtok(0, ",");
  char* com1StandbyFreqStr = strtok(0, ",");
  char* com2ActiveFreqStr = strtok(0, ",");
  char* com2StandbyFreqStr = strtok(0, ",");
  char* comName = strtok(0, ",");
  char* transponder = strtok(0, ",");
  char* transponderState = strtok(0, ",");
  char* nav1ActiveFreqStr = strtok(0, ",");
  char* nav1StandbyFreqStr = strtok(0, ",");
  char* nav2ActiveFreqStr = strtok(0, ",");
  char* nav2StandbyFreqStr = strtok(0, ",");
  char* navName = strtok(0, ",");

  if (navName == 0) {
    debugHandler->print("invalid radio notification");
    return;
  }

  switch (kx165Index) {
  case 2:
    displayFrequency(comDisplay, com, com2ActiveFreqStr);
    displayFrequency(comDisplay, comStby, com2StandbyFreqStr);
    displayFrequency(comDisplay, nav, nav2ActiveFreqStr);
    displayFrequency(comDisplay, navStby, nav2StandbyFreqStr);
    break;
  default:
    displayFrequency(comDisplay, com, com1ActiveFreqStr);
    displayFrequency(comDisplay, comStby, com1StandbyFreqStr);
    displayFrequency(comDisplay, nav, nav1ActiveFreqStr);
    displayFrequency(comDisplay, navStby, nav1StandbyFreqStr);
    break;
  }
}


#endif
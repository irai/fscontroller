
#ifdef KX165_COM_NAV_PANEL
#include <LedControl.h>

/*
 DataIn, CLK, LOAD, number of devices
 */
LedControl navDisplay = LedControl(15, 14, 13, 2); // serial data, clock, load, number of devices
LedControl comDisplay = LedControl(3, 2, 5, 2); // serial data, clock, load, number of devices

led ledOutputs[] = {};

bool comWholeNumber = true;
bool navWholeNumber = true;

void processComFreqInterceptor(SerialMsg* s, button* b) {
  comWholeNumber = !comWholeNumber;
}

void processNavFreqInterceptor(SerialMsg* s, button* b) {
  navWholeNumber = !navWholeNumber;
}

void processComFreqRotaryInterceptor(SerialMsg* s, rotary* r, float increment) {
  char* variable = "com_radio_freq_fract";
  if (comWholeNumber) {
    variable = "com_radio_freq_whole";
  }
  txAction(s, r->action, variable, r->index, increment);
}

void processNavFreqRotaryInterceptor(SerialMsg* s, rotary* r, float increment) {
  char* variable = "nav_radio_freq_fract";
  if (navWholeNumber) {
    variable = "nav_radio_freq_whole";
  }
  txAction(s, r->action, variable, r->index, increment);
}

button switchButtons[] = {
  {.pin = DigitalPin(16,4), .fireLow = true, .action = "set", .function = &defaultButtonFunction, .variable = "com_radio_standby_swap", .index = -1, .setValue = 0},
  {.pin = DigitalPin(17,4), .fireLow = true, .action = "set", .function = &processComFreqInterceptor, .variable = "com button", .index = -1, .setValue = 0},
  {.pin = DigitalPin(22,4), .fireLow = true, .action = "set", .function = &defaultButtonFunction, .variable = "nav_radio_standby_swap", .index = -1, .setValue = 0},
  {.pin = DigitalPin(26,4), .fireLow = true, .action = "set", .function = &processNavFreqInterceptor, .variable = "nav button", .index = -1, .setValue = 0},
};

pot potControls[] = {};

rotary rotaryControls[] = {
  {.aPin= DigitalPin(19, 2), .bPin= DigitalPin(18,2), .action = "inc_n", .function = &processComFreqRotaryInterceptor, .variable = "com_radio_freq_fract", .index = 0 },
  {.aPin= DigitalPin(28, 2), .bPin= DigitalPin(27,2), .action = "inc_n", .function = &processNavFreqRotaryInterceptor, .variable = "nav_radio_freq_fract", .index = 0 },
};

const int nLedOutputs = sizeof(ledOutputs) / sizeof(led);
const int nSwitchButtons = sizeof(switchButtons) / sizeof(button);
const int nPotButtons = sizeof(potControls) / sizeof(pot);
const int nRotaryControls = sizeof(rotaryControls) / sizeof(rotary);

const char* panelName = "kx165.com.nav.panel";
const char* panelVersion = "1.0.0";

const int com = 0;
const int comStby = 1;
const int nav = 0;
const int navStby = 1;

//NOTE: There is a bug in the arduino IDE pre-compilation that requires include of LedCOntrol in the main 
// fscontroller.ino file to prevent pre-compilation error when passing a reference to LedControl.
void displayFrequency(LedControl& display, int com, const char* frequency) {
  int len = strlen(frequency);
  int displayIndex = 4;
  bool dp = false;

  // Display each character of the frequency string in reverse order
  for (int i = len - 1; i >= 0; i--) {
    if (frequency[i] == '.') {
      dp = true; // Skip the decimal point, but set the decimal point on the next digit
    }
    else {
      display.setChar(com, displayIndex, frequency[i], dp);
      dp = false;
      displayIndex--;
    }
  }
}



// initPanel is called at setup to initialise any panel specific variables
int panelInit() {
  comDisplay.shutdown(com, false);
  comDisplay.setIntensity(com, 1);
  comDisplay.shutdown(comStby, false);
  comDisplay.setIntensity(comStby, 1);

  navDisplay.shutdown(nav, false);
  navDisplay.setIntensity(nav, 1);
  navDisplay.shutdown(navStby, false);
  navDisplay.setIntensity(navStby, 1);

  displayFrequency(comDisplay, com, "332.80");

  displayFrequency(comDisplay, comStby, "129.33");

  displayFrequency(navDisplay, nav, "111.33");

  displayFrequency(navDisplay, navStby, "333.44");

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

  displayFrequency(comDisplay, com, com1ActiveFreqStr);
  displayFrequency(comDisplay, comStby, com1StandbyFreqStr);
  displayFrequency(navDisplay, nav, nav1ActiveFreqStr);
  displayFrequency(navDisplay, navStby, nav1StandbyFreqStr);

}


#endif
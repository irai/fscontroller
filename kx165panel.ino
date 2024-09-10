
#ifdef KX165_COM_NAV_PANEL
#include <LedControl.h>

/*
 DataIn, CLK, LOAD, number of devices
 */
LedControl navDisplay = LedControl(15, 14, 13, 2); // serial data, clock, load, number of devices
LedControl comDisplay = LedControl(3, 2, 5, 2); // serial data, clock, load, number of devices

button ledOutputs[] = {};

button switchButtons[] = {
  { .pin = 16, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "com_radio_standby_swap", .index = -1, .setValue = 0},
  { .pin = 17, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "button1", .index = -1, .setValue = 0},
  { .pin = 22, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "nav_radio_standby_swap", .index = -1, .setValue = 0},
  { .pin = 26, .fireLow = true, .value = 0, .savedValue = 0, .debounceTime = 0, .action = "set", .variable = "button2", .index = -1, .setValue = 0},
};

button potButtons[] = {};

rotary rotaryControls[] = {
  { .aPin = 19, .bPin = 18, .buttonPin = 1, 0, 0, 0, 0, 0, .action = "inc_n", .variable = "com_radio_freq_whole", .index = 0 },
  { .aPin = 28, .bPin = 27, .buttonPin = 1, 0, 0, 0, 0, 0, .action = "inc_n", .variable = "nav_radio_freq_whole", .index = 0 },
};

const int nLedOutputs = sizeof(ledOutputs) / sizeof(button);
const int nSwitchButtons = sizeof(switchButtons) / sizeof(button);
const int nPotButtons = sizeof(potButtons) / sizeof(button);
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
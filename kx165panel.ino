
#ifdef KX165_COM_NAV_PANEL
#include <TM1637.h>
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn
 pin 11 is connected to the CLK
 pin 10 is connected to LOAD
 ***** Please set the number of devices you have *****
 But the maximum default of 8 MAX72XX wil also work.
 */
LedControl lc = LedControl(15, 14, 13, 3);

TM1637 lcd(16, 17);


// Instantiation and pins configurations
// TM1637 tm(16, 17); // CLK, DIO
//TM1637Display display(16, 17);

button ledOutputs[] = {};

button switchButtons[] = {};

button potButtons[] = {};

rotary rotaryControls[] = {};

const int nLedOutputs = sizeof(ledOutputs) / sizeof(button);
const int nSwitchButtons = sizeof(switchButtons) / sizeof(button);
const int nPotButtons = sizeof(potButtons) / sizeof(button);
const int nRotaryControls = sizeof(rotaryControls) / sizeof(rotary);

const char* panelName = "kx165.com.nav.panel";
const char* panelVersion = "1.0.0";

const int com = 0;
const int comStby = 1;
const int nav = 2;
const int navStby = 3;


void displayFrequency(int com, const char* frequency) {
  int len = strlen(frequency);
  int displayIndex = 0;
  bool dp = false;

  // Display each character of the frequency string in reverse order
  for (int i = len - 1; i >= 0; i--) {
    if (frequency[i] == '.') {
      dp = true; // Skip the decimal point, but set the decimal point on the next digit
    } else {
      lc.setChar(com, displayIndex, frequency[i], dp);
      dp = false;
      displayIndex++;
    }
  }
}



// initPanel is called at setup to initialise any panel specific variables
int panelInit() {
  lcd.begin();

  lc.shutdown(com, false);
  lc.setIntensity(com, 4);
  lc.shutdown(comStby, false);
  lc.setIntensity(comStby, 4);
  lc.shutdown(nav, false);
  lc.setIntensity(nav, 4);

  displayFrequency(com, "132.80");

  displayFrequency(comStby, "119.90");

  displayFrequency(nav, "178.4");

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

  float com1ActiveFreq = atof(strtok(0, ","));
  float com1StandbyFreq = atof(strtok(0, ","));
  float com2ActiveFreq = atof(strtok(0, ","));
  float com2StandbyFreq = atof(strtok(0, ","));
  char* comName = strtok(0, ",");
  char* transponder = strtok(0, ",");
  char* transponderState = strtok(0, ",");
  float nav1ActiveFreq = atof(strtok(0, ","));
  float nav1StandbyFreq = atof(strtok(0, ","));
  float nav2ActiveFreq = atof(strtok(0, ","));
  float nav2StandbyFreq = atof(strtok(0, ","));
  char* navName = strtok(0, ",");

  if (navName == 0) {
    debugHandler->print("invalid radio notification");
    return;
  }

  lcd.display(com1StandbyFreq);
}


#endif
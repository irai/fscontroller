
#ifdef KX165_COM_NAV_PANEL
#include <TM1637.h>
#include <LedControl.h>

/*
 DataIn, CLK, LOAD
 ***** Please set the number of devices you have *****
 But the maximum default of 8 MAX72XX wil also work.
 */
LedControl navDisplay = LedControl(15, 14, 13, 2);
LedControl comDisplay = LedControl(3, 2, 5, 2);

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
const int nav = 0;
const int navStby = 1;


//FIXME: could not pass LedControl object here. Using a hack type instead.
void displayFrequency(int display, int com, const char* frequency) {
  int len = strlen(frequency);
  int displayIndex = 4;
  bool dp = false;

  // Display each character of the frequency string in reverse order
  for (int i = len - 1; i >= 0; i--) {
    if (frequency[i] == '.') {
      dp = true; // Skip the decimal point, but set the decimal point on the next digit
    } else {
      if (display == 1) {
        navDisplay.setChar(com, displayIndex, frequency[i], dp);
      } else {
        comDisplay.setChar(com, displayIndex, frequency[i], dp);
      }
      dp = false;
      displayIndex--;
    }
  }
}



// initPanel is called at setup to initialise any panel specific variables
int panelInit() {
  lcd.begin();

  comDisplay.shutdown(com, false );
  comDisplay.setIntensity(com, 1);
  comDisplay.shutdown(comStby, false);
  comDisplay.setIntensity(comStby, 1);

  navDisplay.shutdown(nav, false);
  navDisplay.setIntensity(nav, 1);
  navDisplay.shutdown(navStby, false);
  navDisplay.setIntensity(navStby, 1);

  displayFrequency(0, com, "332.80");

  displayFrequency(0, comStby, "129.33");

  displayFrequency(1, nav, "111.22");

  displayFrequency(1, navStby, "333.44");

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
  // float com1ActiveFreq = atof(strtok(0, ","));
  // float com1StandbyFreq = atof(strtok(0, ","));
  // float com2ActiveFreq = atof(strtok(0, ","));
  // float com2StandbyFreq = atof(strtok(0, ","));
  char* comName = strtok(0, ",");
  char* transponder = strtok(0, ",");
  char* transponderState = strtok(0, ",");
  char* nav1ActiveFreqStr = strtok(0, ",");
  char* nav1StandbyFreqStr = strtok(0, ",");
  char* nav2ActiveFreqStr = strtok(0, ",");
  char* nav2StandbyFreqStr = strtok(0, ",");
  // float nav1ActiveFreq = atof(nav1ActiveFreqStr);
  // float nav1StandbyFreq = atof(nav1StandbyFreqStr);
  // float nav2ActiveFreq = atof(nav2ActiveFreqStr);
  // float nav2StandbyFreq = atof(nav2StandbyFreqStr);
  char* navName = strtok(0, ",");

  if (navName == 0) {
    debugHandler->print("invalid radio notification");
    return;
  }

  displayFrequency(0, com, com1ActiveFreqStr);
  displayFrequency(0, comStby, com1StandbyFreqStr);
  displayFrequency(1, nav, nav1ActiveFreqStr);
  displayFrequency(1, navStby, nav1StandbyFreqStr);

  // lcd.display(com1StandbyFreq);
}


#endif
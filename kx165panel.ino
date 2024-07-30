
#ifdef KX165_COM_NAV_PANEL

#include <TM1637.h>

// Instantiation and pins configurations
TM1637 tm(16, 17); // CLK, DIO

button ledOutputs[] = {
};

button switchButtons[] = {
};

button potButtons[] = {};

rotary rotaryControls[] = {};

const int nLedOutputs = sizeof(ledOutputs) / sizeof(button);
const int nSwitchButtons = sizeof(switchButtons) / sizeof(button);
const int nPotButtons = sizeof(potButtons) / sizeof(button);
const int nRotaryControls = sizeof(rotaryControls) / sizeof(rotary);

const char* panelName = "kx165.com.nav.panel";
const char* panelVersion = "1.0.0";


// initPanel is called at setup to initialise any panel specific variables
int panelInit() {
    tm.begin();
    tm.setBrightnessPercent(10);
    return 0;
}

void panelConnect(SerialMsg* s) {
    txPanel(s, panelName, panelVersion);
    txNotification(s, "RADIO");
}

// handleNotification is called every time the panel receives a notification
void panelNotification(char* msg) {
    char* tok = strtok(msg, ",");
    if (tok == 0 || strcmp(tok, "notification") != 0) {
        return;
    }

    // Display Integers:
    tm.display(1234);
    delay(1000);

    // Display float:
    tm.display(29.65);
    delay(1000);

    // Display String:
    tm.display("PLAY");
    delay(1000);
    tm.display("STOP");
    tm.setDp(1);
    delay(1000);

    tok = strtok(0, ",");
    if (tok == 0 || strcmp(tok, "RADIO") != 0) {
        debugHandler->print("invalid notification ");
        debugHandler->println(tok);
        return;
    }


    debugHandler->println("gear moving");
}


#endif
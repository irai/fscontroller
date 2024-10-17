
#ifndef ELECTRONICS_H
#define ELECTRONICS_H

#include <Stream.h>

#include <Arduino.h>
#include <WString.h>

const int maxMsgSize = 256;

typedef struct SerialMsg {
  Stream* Port;
  char rxbuf[maxMsgSize];
  unsigned int rxcount;
  char txbuf[maxMsgSize];
  unsigned int txcount;
  unsigned long timeout;
} SerialMsg;

class DigitalPin {
public:
  DigitalPin(int pin, unsigned long debounceTimeout)
    : pin(pin), debounceTime(debounceTimeout), timeout(0), previousState(LOW), state(LOW) {
    pinMode(pin, INPUT_PULLUP); // default to HIGH when not connected; LOW when connected to GND
    state = ~digitalRead(pin); // read the pin and invert the value
    previousState = ~state; // force a change
  }

  void setDebounceTimeout(unsigned long timeout) {
    debounceTime = timeout;
  }

  void setPinMode(uint8_t mode) {
    pinMode(pin, mode);
  }

  bool update() {
    int reading = ~digitalRead(pin); // read the pin and invert the value
    if (reading != previousState) { // set timeout if state changed
      previousState = reading;
      timeout = millis() + debounceTime;
    }

    if (millis() >= timeout) {
      if (reading != state) {
        state = reading;
        return true;
      }
    }

    return false;
  }

  int getState() const {
    return state;
  }

  int getPreviousState() const {
    return previousState;
  }

  String toString() const {
    return "Pin: " + String(pin) + ", State: " + String(state) + ", Last state: " + String(previousState) + ", Timeout: " + String(timeout);
  }

private:
  int pin;
  unsigned long debounceTime;
  unsigned long timeout;
  int previousState;
  int state;
};
extern int delayAnalogRead(uint8_t);
class AnalogPin {
public:
  AnalogPin(int pin, unsigned long debounceTimeout)
    : pin(pin), debounceTime(debounceTimeout), timeout(0), previousState(LOW), state(LOW) {
    pinMode(pin, INPUT);
    previousState = analogRead(pin);
  }

  void setDebounceTimeout(unsigned long timeout) {
    debounceTime = timeout;
  }

  bool update() {
    const int filter = 2;
    int reading = delayAnalogRead(pin);

    // linear smoothing to avoid fluctuations
#define SMOOTHING
#ifdef SMOOTHING
    reading = previousState + ((reading - previousState) / filter);

    // We lose the high and low values with the filter
    // convert the lowest and highest to the minimum and maximum respectivelly.
    if (reading > (1023 - filter * 2)) {
      reading = 1023;
    }
    else if (reading < (filter * 3)) {
      reading = 0;
    }
#endif

    if (reading != previousState) {
      timeout = millis() + debounceTime;
    }

    if (millis() >= timeout) {
      if (reading != state) {
        state = reading;
        return true;
      }
    }
    previousState = reading;

    return false;
  }

  int getState() const {
    return state;
  }

  int getPreviousState() const {
    return previousState;
  }

  String toString() const {
    return "Pin: " + String(pin) + ", State: " + String(state) + ", Last state: " + String(previousState) + ", Timeout: " + String(timeout);
  }

private:
  int pin;
  unsigned long debounceTime;
  unsigned long timeout;
  int previousState;
  int state;
};

typedef struct button {
  DigitalPin pin;
  const char* action;
  void (*function)(SerialMsg*, struct button*); // interceptor function
  const char* variable;        // variable name
  int8_t index;           // variable index or -1
  int setValue;           // value to set or -1
} button;

typedef struct pot {
  AnalogPin pin;
  bool fireLow;  // true if the pin only generates a message when Low
  const char* action;
  // void (*function)(SerialMsg*, struct pot*); // interceptor function
  const char* variable;        // variable name
  int8_t index;           // variable index or -1
  int setValue;           // value to set or -1
} pot;

typedef struct rotary {
  DigitalPin aPin; // A clock pin
  DigitalPin bPin; // B data pin
  const char* action;
  void (*function)(SerialMsg*, struct rotary*, float increment); // interceptor function
  const char* variable;  // variable name
  int8_t index;     // variable index or -1
} rotary;

typedef struct {
  int pin;
} led;


extern void processSwitch(SerialMsg*, button*);
extern void processPot(SerialMsg*, button*);
extern void processPushButton(SerialMsg*, button*);
extern void processRotary(SerialMsg*, rotary*);

extern const int nLedOutputs;
extern led ledOutputs[];
extern const int nSwitchButtons;
extern button switchButtons[];
extern const int npushButtons;
extern button pushButtons[];
extern const int nPotButtons;
extern pot potControls[];
extern const int nRotaryControls;
extern void defaultFunction(SerialMsg*, button*);
extern rotary rotaryControls[];
extern const char* panelName;

// These index positions for statistics count are part of the public msg interface.
const int StatsRxMsgs = 0;
const int StatsTxMsgs = 1;
const int StatsRxErrors = 2;
const int StatsTxErrors = 3;
const int StatsTimeoutErrors = 4;

typedef struct {
  uint16_t stats[5];
} statistics;


extern SerialMsg* NewSerialMsg(Stream* s);

// mandatory function to be implemented by every panel
extern int panelInit();
extern void panelConnect(SerialMsg* s);
extern void panelNotification(char* msg);

const char panelToken[] = "panel";
const char notificationToken[] = "notification";
const char testToken[] = "test";
const char logToken[] = "log";


#endif
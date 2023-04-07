
#ifndef ELECTRONICS_H
#define ELECTRONICS_H

// message types
const uint8_t CONFIG = 0;
const uint8_t BUTTON = 1;
const uint8_t SWITCH = 2;
const uint8_t POT = 3;
const uint8_t ROTARY = 4;
const uint8_t PANEL = 5; 
const uint8_t KEYSTROKES = 6;
const uint8_t LOGLEVEL = 7;
const uint8_t SYNC = 254;       // reserved for additional messages
const uint8_t EXTENSION = 255;  // reserved for additional messages

#include <Arduino.h>
#include <WString.h>

typedef struct cmd {
  int len;
  uint16_t seq[];
} cmd;

typedef struct {
  uint8_t pin;
  int value;       // pin current value
  int savedValue;  // pin saved value
  uint32_t debounceTime;  // the last time the output pin was toggled
} button;

typedef struct {
  uint8_t aPin;  // A or Clock pin
  uint8_t bPin;  // B or data pin
  uint8_t buttonPin;
  uint32_t debounceTime;  // the last time the output pin was toggled
  int aState;
  int aStatePrevious;
  int bStatePrevious;
  int bState;
} rotary;


extern void processSwitch(Stream*, button*);
extern void processPot(Stream* , button* );
extern void processPushButton(Stream* , button* );
extern void processRotary(Stream* , rotary* );
extern const int nSwitchButtons;
extern button switchButtons[];
extern const int npushButtons;
extern button pushButtons[];
extern const int nPotButtons;
extern button potButtons[];
extern const int nRotaryControls;
extern rotary rotaryControls[];
extern const String panelName;

const int StatsRxMsgs = 0;
const int StatsTxMsgs = 1;
const int StatsRxErrors = 2;
const int StatsTxErrors = 3;
const int StatsTimeoutErrors = 4;

typedef struct {
  uint16_t stats[5];
} statistics;




#endif
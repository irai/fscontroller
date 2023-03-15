
#ifndef ELECTRONICS_H
#define ELECTRONICS_H

// message types
const uint8_t CONFIG = 0;
const uint8_t BUTTON = 1;
const uint8_t SWITCH = 2;
const uint8_t POT = 3;
const uint8_t ROTARY = 4;
const uint8_t PANEL = 5;
const uint8_t KEYS = 6;
const uint8_t SYNC = 254;       // reserved for additional messages
const uint8_t EXTENSION = 255;  // reserved for additional messages

#include <Arduino.h>
#include <WString.h>

typedef struct cmd {
  int len;
  unsigned int seq[];
} cmd;

typedef struct button_t {
  String name;
  uint8_t pin;
  cmd* cmd[4];     // On, Off, Next, Prev
  int value;       // pin current value
  int savedValue;  // pin saved value
  int count;
  int max;
  long debounceTime;  // the last time the output pin was toggled
} button;

typedef struct rotary {
  String name;
  void (*focus)(rotary*);
  uint8_t aPin;  // A or Clock pin
  cmd* aKeys;
  uint8_t bPin;  // B or data pin
  cmd* bKeys;
  uint8_t buttonPin;
  cmd* buttonKeys;
  int counter;
  long debounceTime;  // the last time the output pin was toggled
  int aState;
  int aStatePrevious;
  int bStatePrevious;
  int bState;
} rotary;


extern void processSwitch(Stream*, button*);
extern void processPot(Stream* , button* );
extern void processPressureButton(Stream* , button* );
extern void processRotary(Stream* , rotary* );
extern const int nSwitchButtons;
extern button switchButtons[];
extern const int nPressureButtons;
extern button pressureButtons[];
extern const int nPotButtons;
extern button potButtons[];
extern const int nRotaryControls;
extern rotary rotaryControls[];

typedef struct FlightSimulator_t {

} FlightSimulator;




#endif
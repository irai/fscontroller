
#ifndef ELECTRONICS_H
#define ELECTRONICS_H

#include <Stream.h>

#include <Arduino.h>
#include <WString.h>

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


typedef struct SerialMsg {
  Stream *Port;
  char rxbuf[64];
  unsigned int rxcount;
  char txbuf[64];
  unsigned int txcount;
  unsigned long timeout;
} SerialMsg;

extern void processSwitch(SerialMsg*, button*);
extern void processPot(SerialMsg* , button* );
extern void processPushButton(SerialMsg* , button* );
extern void processRotary(SerialMsg* , rotary* );

extern const int nLedOutputs;
extern button ledOutputs[];
extern const int nSwitchButtons;
extern button switchButtons[];
extern const int npushButtons;
extern button pushButtons[];
extern const int nPotButtons;
extern button potButtons[];
extern const int nRotaryControls;
extern rotary rotaryControls[];
extern const char * panelName;

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
extern void panelNotification(char *msg);


#endif
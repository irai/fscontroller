

#include "electronics.h"
#include <Stream.h>

#define DEBOUNCE_TIME 16

// https://lastminuteengineers.com/rotary-encoder-arduino-tutorial/
void processRotary(Stream* s, rotary* r) {
  if (r->debounceTime > millis()) {
    return;
  }
  // If A state changed, then wheel has moved
  if (r->aStatePrevious != r->aState) {
    r->bState = digitalRead(r->bPin);  // for accuracy, must read again after a change to pin A

#ifdef DEBUG
    debugHandler->print("Rotary PIN ");
    debugHandler->print(r->aPin);
    debugHandler->print("=");
    debugHandler->print(r->aState);
    debugHandler->print(" PIN ");
    debugHandler->print(r->bPin);
    debugHandler->print("=");
    debugHandler->print(r->bState);
#endif

    // If the B value is different than A value,
    // the encoder is rotating anti-clockwise
    if (r->bState != r->aState) {
#if DEBUG
      debugHandler->println(" decrease");
#endif
      txRotary(s, r->aPin, -1);

    } else {
      // Encoder is rotating clockwise
#if DEBUG
      debugHandler->println(" increase");
#endif

      txRotary(s, r->aPin, +1);
    }
#if DEBUG
    debugHandler->flush();
#endif
  }
  r->aStatePrevious = r->aState;  // Remember last A
  r->debounceTime = millis() + DEBOUNCE_TIME;
}

void processPot(Stream* s, button* b) {
  if (b->debounceTime > millis()) {
    return;
  }
  
  // exponential smoothing to avoid fluctuations
// ignore outliers
  // reading may oscilate between +8 and -8 volts; ignore
  int value = b->savedValue + ((b->value - b->savedValue) >> 3);



  if (value == b->savedValue) {
    return;
  }

  b->debounceTime = millis() + DEBOUNCE_TIME;
  b->savedValue = value;

#ifdef DEBUG
  debugHandler->print(b->name);
  debugHandler->print(" pin=");
  debugHandler->print(b->pin);
  debugHandler->print(" value=");
  debugHandler->println(value);
  debugHandler->flush();
#endif
  txPot(s, b->pin, b->value);
}

void processSwitch(Stream* s, button* b) {
  if (b->value == b->savedValue || b->debounceTime > millis()) {
    return;
  }
  b->debounceTime = millis() + DEBOUNCE_TIME;
  b->savedValue = b->value;

#ifdef DEBUG
  debugHandler->print(b->name);
  debugHandler->print(" pin=");
  debugHandler->print(b->pin);
  debugHandler->print(" value=");
  debugHandler->println(b->value);
  debugHandler->flush();
#endif

  txSwitch(s, b->pin, b->value);
}

void processPushButton(Stream* s, button* b) {
  if (b->value == b->savedValue || b->debounceTime > millis()) {
    return;
  }
  b->debounceTime = millis() + DEBOUNCE_TIME;
  b->savedValue = b->value;

#ifdef DEBUG
  debugHandler->print(b->name);
  debugHandler->print(" pin=");
  debugHandler->print(b->pin);
  debugHandler->print(" value=");
  debugHandler->println(b->value);
  debugHandler->flush();
#endif

  txButton(s, b->pin, b->value);
}
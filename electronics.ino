

#include "electronics.h"
#include <Stream.h>


// https://lastminuteengineers.com/rotary-encoder-arduino-tutorial/
void processRotary(Stream* s, rotary* r) {
  // If A state changed, then wheel has moved
  if (r->aStatePrevious != r->aState) {
    r->bState = digitalRead(r->bPin);  // for accuracy, must read again after a change to pin A

#ifdef DEBUG
    s->print("Rotary PIN ");
    s->print(r->aPin);
    s->print("=");
    s->print(r->aState);
    s->print(" PIN ");
    s->print(r->bPin);
    s->print("=");
    s->print(r->bState);
    s->print(" ");
    s->println(r->name);
#endif

    // If the B value is different than A value,
    // the encoder is rotating anti-clockwise
    if (r->bState != r->aState) {
      txRotary(s, r->aPin, -1);

    } else {
      // Encoder is rotating clockwise
      txRotary(s, r->aPin, +1);
    }
  }
  r->aStatePrevious = r->aState;  // Remember last A
}

void processPot(Stream* s, button* b) {
    if (b->debounceTime > millis()) {
    return;
  }

  // exponential smoothing to avoid jumps
  int value = b->savedValue +  ((b->value - b->savedValue) >> 2); 

  if (value == b->savedValue) {
    return;
  }

  // reading may oscilate between +1 and -1 volts; ignore
  // https://forum.arduino.cc/t/debounce-a-potentiometer/7509
  // if ((b->value >= b->savedValue - 3 && b->value <= b->savedValue + 3) || b->debounceTime > millis()) {
    // return;
  // }
  b->debounceTime = millis() + 5;
  b->savedValue = value << 2;

#ifdef DEBUG
  debugHandler->print(b->name);
  debugHandler->print(" pin=");
  debugHandler->print(b->pin);
  debugHandler->print(" value=");
  debugHandler->println(value);
#endif
  txPot(s, b->pin, b->value);
}

void processSwitch(Stream* s, button* b) {
  if (b->value == b->savedValue || b->debounceTime > millis()) {
    return;
  }
  b->debounceTime = millis() + 2;
  b->savedValue = b->value;

#ifdef DEBUG
  debugHandler->print(b->name);
  debugHandler->print(" pin=");
  debugHandler->print(b->pin);
  debugHandler->print(" value=");
  debugHandler->println(b->value);
#endif

  txSwitch(s, b->pin, b->value);
}

void processPressureButton(Stream* s, button* b) {
  if (b->value == b->savedValue || b->debounceTime > millis()) {
    return;
  }
  b->debounceTime = millis() + 2;
  b->savedValue = b->value;

#ifdef DEBUG
  debugHandler->print(b->name);
  debugHandler->print(" pin=");
  debugHandler->print(b->pin);
  debugHandler->print(" value=");
  debugHandler->println(b->value);
#endif

  txButton(s, b->pin, b->value);
}
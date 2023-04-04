

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

    if (Debug) {
      debugHandler->print("Rotary PIN ");
      debugHandler->print(r->aPin);
      debugHandler->print("=");
      debugHandler->print(r->aState);
      debugHandler->print(" PIN ");
      debugHandler->print(r->bPin);
      debugHandler->print("=");
      debugHandler->print(r->bState);
    }

    // If the B value is different than A value,
    // the encoder is rotating anti-clockwise
    if (r->bState != r->aState) {
      if (Debug) {
        debugHandler->println(" decrease");
      }
      txRotary(s, r->aPin, -1);

    } else {
      // Encoder is rotating clockwise
      if (Debug) {
        debugHandler->println(" increase");
      }

      txRotary(s, r->aPin, +1);
    }
    if (Debug) {
      debugHandler->flush();
    }
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
  // When using power from Raspberry PI, seen very large oscilations between +50 and -50 volts; ignore
  int value = b->savedValue + ((b->value - b->savedValue) >> 5);

  if (value == b->savedValue) {
    return;
  }

  b->debounceTime = millis() + DEBOUNCE_TIME;
  b->savedValue = b->value;

  if (Debug) {
    debugHandler->print(b->name);
    debugHandler->print(" pin=");
    debugHandler->print(b->pin);
    debugHandler->print(" value=");
    debugHandler->println(b->value);
    debugHandler->flush();
  }
  txPot(s, b->pin, b->value);
}

void processSwitch(Stream* s, button* b) {
  if (b->value == b->savedValue || b->debounceTime > millis()) {
    return;
  }
  b->debounceTime = millis() + DEBOUNCE_TIME;
  b->savedValue = b->value;

  if (Debug) {
    debugHandler->print(b->name);
    debugHandler->print(" pin=");
    debugHandler->print(b->pin);
    debugHandler->print(" value=");
    debugHandler->println(b->value);
    debugHandler->flush();
  }

  txSwitch(s, b->pin, b->value);
}

void processPushButton(Stream* s, button* b) {
  if (b->value == b->savedValue || b->debounceTime > millis()) {
    return;
  }
  b->debounceTime = millis() + DEBOUNCE_TIME;
  b->savedValue = b->value;

  if (Debug) {
    debugHandler->print(b->name);
    debugHandler->print(" pin=");
    debugHandler->print(b->pin);
    debugHandler->print(" value=");
    debugHandler->println(b->value);
    debugHandler->flush();
  }

  txButton(s, b->pin, b->value);
}
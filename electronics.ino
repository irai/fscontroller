

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
      debugHandler->print("Rotary pin=");
      debugHandler->print(r->aPin);
      debugHandler->print(" value=");
      debugHandler->print(r->aState);
      debugHandler->print(" pin=");
      debugHandler->print(r->bPin);
      debugHandler->print(" value=");
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

  // linear smoothing to avoid fluctuations
  // ignore outliers
  // When using power from Raspberry PI, seen very large oscilations between +50 and -50 in the ADC converter; ignore
  int value = b->savedValue + ((b->value - b->savedValue) / 4);
  // int value = b->value;
  if (value == b->savedValue) {
    return;
  }

  if (Debug) {
    debugHandler->print("pot pin=");
    debugHandler->print(b->pin);
    debugHandler->print(" saved=");
    debugHandler->print(b->savedValue);
    debugHandler->print(" value=");
    debugHandler->print(b->value);
    debugHandler->print(" calc=");
    debugHandler->println(value);
    debugHandler->flush();
  }

  b->debounceTime = millis() + DEBOUNCE_TIME;
  b->savedValue = b->value;
  txPot(s, b->pin, b->savedValue);
}

void processSwitch(Stream* s, button* b) {
  if (b->value == b->savedValue || b->debounceTime > millis()) {
    return;
  }
  b->debounceTime = millis() + DEBOUNCE_TIME;
  b->savedValue = b->value;

  if (Debug) {
    debugHandler->print("switch pin=");
    debugHandler->print(b->pin);
    debugHandler->print(" value=");
    debugHandler->println(b->value);
    debugHandler->flush();
  }

  txSwitch(s, b->pin, b->value);
}



#include "electronics.h"

const uint8_t eventOn = 0;
const uint8_t eventOff = 1;
const uint8_t eventNext = 2;
const uint8_t eventPrev = 3;


// https://lastminuteengineers.com/rotary-encoder-arduino-tutorial/
void processRotary(rotary* r) {
  // If A state changed, then wheel has moved
  if (r->aStatePrevious != r->aState) {
    r->bState = digitalRead(r->bPin);  // for accuracy, must read again after a change to pin A

#ifdef DEBUG
    Serial.print("Rotary PIN ");
    Serial.print(r->aPin);
    Serial.print("=");
    Serial.print(r->aState);
    Serial.print(" PIN ");
    Serial.print(r->bPin);
    Serial.print("=");
    Serial.print(r->bState);
    Serial.print(" ");
    Serial.println(r->name);
#endif

    // If the B value is different than A value,
    // the encoder is rotating anti-clockwise
    if (r->bState != r->aState) {
      r->counter--;
      txRotary(r->aPin, -1);

    } else {
      // Encoder is rotating clockwise
      r->counter++;
      txRotary(r->aPin, +1);
    }
  }
  r->aStatePrevious = r->aState;  // Remember last A
}

void processPot(button* b) {
  // reading may oscilate between +1 and -1 volts; ignore
  // https://forum.arduino.cc/t/debounce-a-potentiometer/7509
  if ((b->value >= b->savedValue -1 && b->value <= b->savedValue +1) || b->debounceTime > millis()) {
    return;
  }
  b->debounceTime = millis()+2;
  b->savedValue = b->value;

#ifdef DEBUG
  Serial.print(b->name);
  Serial.print(" pin=");
  Serial.print(b->pin);
  Serial.print(" value=");
  Serial.println(b->value);
#endif
  txPot(b->pin, b->value);
}

void processSwitch(button* b) {
  if (b->value == b->savedValue || b->debounceTime > millis()) {
    return;
  }
  b->debounceTime = millis()+2;
  b->savedValue = b->value;

#ifdef DEBUG
  Serial.print(b->name);
  Serial.print(" pin=");
  Serial.print(b->pin);
  Serial.print(" value=");
  Serial.println(b->value);
#endif

  txSwitch(b->pin, b->value);
}

void processPressureButton(button* b) {
  if (b->value == b->savedValue|| b->debounceTime > millis()) {
    return;
  }
    b->debounceTime = millis()+2;
  b->savedValue = b->value;

#ifdef DEBUG
  Serial.print(b->name);
  Serial.print(" pin=");
  Serial.print(b->pin);
  Serial.print(" value=");
  Serial.println(b->value);
#endif

  txButton(b->pin, b->value);
}
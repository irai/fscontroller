
#include "electronics.h"

#define DEBOUNCE_TIME 16


int delayAnalogRead(uint8_t pin) {
  const int n = 64;  // number of samples to average
  int value = 0;

  analogRead(pin);        // discard first reading
  delayMicroseconds(10);  // IMPORTANT: must delay to minimise fluctuation on the analog port
  analogRead(pin);        // discard second reading
  for (int i = 0; i < n; i++) {
    value += analogRead(pin);
  }
  value = value / n;

  return value;
}

// https://lastminuteengineers.com/rotary-encoder-arduino-tutorial/
void processRotary(SerialMsg* s, rotary* r) {

  // If A state changed, then wheel has moved
  // Some integrated board and rotary switch generate both a HIGH and LOW voltage for a single change of the wheel but others generate a single change.
  // React in software to avoid double count. For example the trim control uses an integrated circuit that generates both HIGH and LOW.
  // r->aState = digitalRead(r->aPin);  // for accuracy, must read again after a change to pin A
  if (r->aStatePrevious != r->aState) {
    r->bState = digitalRead(r->bPin);  // for accuracy, must read again B after a change to A

    if (Debug) {
      debugHandler->print("Rotary aPin=");
      debugHandler->print(r->aPin);
      debugHandler->print(" aValue=");
      debugHandler->print(r->aState);
      debugHandler->print(" bPin=");
      debugHandler->print(r->bPin);
      debugHandler->print(" bValue=");
      debugHandler->print(r->bState);
    }

    // If the B value is different than A value,
    // the encoder is rotating anti-clockwise
    if (r->bState != r->aState) {
      if (Debug) {
        debugHandler->println(" decrease");
      }
      
      txAction(s, r->action, r->variable, r->index, -1);
    } else {
      // Encoder is rotating clockwise
      if (Debug) {
        debugHandler->println(" increase");
      }

      txAction(s, r->action, r->variable, r->index, +1);
    }
    if (Debug) {
      debugHandler->flush();
    }
  }
  r->aStatePrevious = r->aState;  // Remember last A
  r->debounceTime = 0;
}

void processPot(SerialMsg* s, button* b) {
  const int filter = 3;

  // linear smoothing to avoid fluctuations
  int value = b->savedValue + ((b->value - b->savedValue) / filter);

  // We lose the high and low values with the filter
  // convert the lowest and highest to the minimum and maximum respectivelly.
  if (b->value > (1023 - filter*2 ) || value > (1023 - filter*2 )) {
    value = 1023;
  } else if (b->value < (filter *2 ) || value < (filter*2  )) {
    value = 0;
  }

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

  // b->debounceTime = millis() + 1;
  b->debounceTime = 0;
  b->savedValue = value;
  // txPot(s, b->pin, b->savedValue);
  txAction(s, b->action, b->variable, b->index, ((float)b->savedValue)/1023*100); // percentage
}

void processSwitch(SerialMsg* s, button* b) {
  if (b->value == b->savedValue) {
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

  // For rotary switches, we only want to fire on the high value
  if (b->fireLow && b->value == HIGH) {
    return;
  }

  // txSwitch(s, b->pin, b->value);
  if (b->setValue != -9999) {  // magic value to indicate to use the set value
    return txAction(s, b->action, b->variable, b->index, b->setValue);
  } 
  txAction(s, b->action, b->variable, b->index, b->value);
}


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

void defaultRotaryFunction(SerialMsg* s, rotary* r, float increment) {
  txAction(s, r->action, r->variable, r->index, increment);
}

// https://lastminuteengineers.com/rotary-encoder-arduino-tutorial/
void processRotary(SerialMsg* s, rotary* r) {

  // If A state changed, then wheel has moved. Some integrated board and rotary switch generate both a HIGH and LOW voltage 
  // for a single change of the wheel but others generate a single change. React in software to avoid double count. 
  // For example the trim control uses an integrated circuit that generates both HIGH and LOW.
  if (r->aPin.getState() == HIGH) {
    if (Debug) {
      debugHandler->print("rotary a" + r->aPin.toString());
      debugHandler->print(" b" + r->bPin.toString());
    }

    float increment = 1;
    // If the B value is different than A value (HIGH),
    // the encoder is rotating anti-clockwise
    if (r->bPin.getState() != HIGH) {
      increment = -1;
    }

    if (Debug) {
      debugHandler->print(" increment=");
      debugHandler->println(increment);
    }

    if (r->function != NULL) {
      r->function(s, r, increment);
    }
    else {
      defaultRotaryFunction(s, r, increment);
    }

    if (Debug) {
      debugHandler->flush();
    }
  }
}

void processPot(SerialMsg* s, pot* b) {
  // const int filter = 3;

  /**
  // linear smoothing to avoid fluctuations
  int value = b->savedValue + ((b->value - b->savedValue) / filter);

  // We lose the high and low values with the filter
  // convert the lowest and highest to the minimum and maximum respectivelly.
  if (b->value > (1023 - filter * 2) || value > (1023 - filter * 2)) {
    value = 1023;
  }
  else if (b->value < (filter * 2) || value < (filter * 2)) {
    value = 0;
  }

  if (value == b->savedValue) {
    return;
  }
  */

  if (Debug) {
    debugHandler->println("pot " + b->pin.toString());
    debugHandler->flush();
  }

  txAction(s, b->action, b->variable, b->index, ((float)b->pin.getState()) / 1023 * 100); // percentage
}


void defaultButtonFunction(SerialMsg* s, button* b) {
  txAction(s, b->action, b->variable, b->index, b->pin.getState());
}

void processSwitch(SerialMsg* s, button* b) {

  if (Debug) {
    debugHandler->println("switch " + b->pin.toString());
    debugHandler->flush();
  }

  // For rotary switches, we only want to fire on the high value
  if (b->fireLow && b->pin.getState() == HIGH) {
    return;
  }

  //Override function
  if (b->function != NULL) {
    b->function(s, b);
    return;
  }

  if (b->setValue != -9999) {  // magic value to indicate to use the set value
    return txAction(s, b->action, b->variable, b->index, b->setValue);
  }
  txAction(s, b->action, b->variable, b->index, b->pin.getState());
}

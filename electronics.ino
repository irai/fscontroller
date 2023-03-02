

#include "electronics.h"

const uint8_t eventOn = 0;
const uint8_t eventOff = 1;
const uint8_t eventNext = 2;
const uint8_t eventPrev = 3;


// https://lastminuteengineers.com/rotary-encoder-arduino-tutorial/
void processRotary(rotary* r) {
  // If A state changed, then wheel has moved
  if (r->aStatePrevious != r->aState) {
    // If the B value is different than A value,
    // the encoder is rotating anti-clockwise
    r->bState = digitalRead(r->bPin);
      Serial.print("PIN ");
      Serial.print(r->aPin);
      Serial.print("=");
      Serial.print(r->aState);
      Serial.print(" PIN ");
      Serial.print(r->bPin);
      Serial.print("=");
      Serial.print(r->bState);
      Serial.print(" ");
      Serial.print(r->name);
    if (r->bState != r->aState) {
      r->counter--;
      queueKeys(r->bKeys);
      Serial.println(" decrease");
    } else {
      // Encoder is rotating clockwise
      r->counter++;
      queueKeys(r->aKeys);
      Serial.println(" increase");
    }
  }
  r->aStatePrevious = r->aState;  // Remember last A
}

void processPot(button* b) {
  if (b->value <= 3) {  // if value is < 3 assume it is set to zero
    if (b->savedValue != 0) {
      queueKeys(b->cmd[eventOff]);
      Serial.print(b->name);
      Serial.println("is off");
      b->savedValue = 0;
    }
    return;
  }

  if (b->value >= 1023 - 3) {  // if value is > 1020 assume it is set to max
    if (b->savedValue != 1023) {
      queueKeys(b->cmd[eventOn]);
      Serial.print(b->name);
      Serial.println("is at max");
      b->savedValue = 1023;
    }
    return;
  }

  // reading may oscilate between +1 and -1 volts; ignore
  // https://forum.arduino.cc/t/debounce-a-potentiometer/7509
  if (b->value >= b->savedValue - b->count && b->value <= b->savedValue + b->count) {
    return;
  }

  int difference = b->value - b->savedValue;

  if (difference < 0) {
    Serial.print(b->name);
    Serial.print(" dec value ");
    Serial.println(b->value);
    // Serial.println(b->savedValue);
    queueKeys(b->cmd[eventPrev]);
    b->savedValue -= b->count;
    if (b->savedValue <= 0) {  // never set to zero - only when value is zero - see begining of function
      b->savedValue = 1;
    }
  } else if (difference > 0) {
    Serial.print(b->name);
    Serial.print(" inc value");
    Serial.println(b->value);
    queueKeys(b->cmd[eventNext]);
    b->savedValue += b->count;
    if (b->savedValue >= 1023) {  // never set to 1023 - only when value is 1023 - see begining of function
      b->savedValue = 1022;
    }
  }
}

void processSwitch(button* b) {
  if (b->value == b->savedValue) {
    return;
  }
  b->savedValue = b->value;

  Serial.print(b->name);
  Serial.print(" pin=");
  Serial.print(b->pin);
  // Serial.print(" cmd=");
  // Serial.print(b->cmd[eventOn]->seq[0]);
  Serial.print("=");
  Serial.println(b->value);
  if (b->value == HIGH) {
    queueKeys(b->cmd[eventOn]);  // inverted???
  } else {
   queueKeys (b->cmd[eventOff]);  // inverted???
  }
}

void processPressureButton(button* b) {
  if (b->value == b->savedValue) {
    return;
  }

  b->savedValue = b->value;
  Serial.print("pressure ");
  Serial.println(b->value);
  if (b->value == HIGH) {
    if (b->count == 0) {  // first press
      b->count++;
      queueKeys(b->cmd[eventOn]);
      Serial.print(b->name);
      Serial.println(" first pressed");
    } else if (b->count >= b->max) {  // last press in series
      b->count = 0;
      queueKeys(b->cmd[eventOff]);
      Serial.print(b->name);
      Serial.println(" last pressed");
    } else {  // next press
      b->count++;
      queueKeys(b->cmd[eventNext]);
      Serial.print(b->name);
      Serial.println(" released");
    };
  };
}
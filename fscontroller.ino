#include <Keyboard.h>     // Use built-in Keyboard library
#include <HID_Buttons.h>  // Must import AFTER Keyboard.h

#define LIVE 1

#define nonprinting 136  // it's a non-printing key (not a modifier)

#define KEY_F11 68 + nonprinting
#define KEY_F12 69 + nonprinting
#define KEY_PRINTSCREEN 70 + nonprinting
#define KEY_SCROLL_LOCK 71 + nonprinting
#define KEY_PAUSE 72 + nonprinting
#define KEY_INSERT 73 + nonprinting
#define KEY_HOME 74 + nonprinting
#define KEY_PAGE_UP 75 + nonprinting
#define KEY_DELETE 76 + nonprinting
#define KEY_END 77 + nonprinting
#define KEY_PAGE_DOWN 78 + nonprinting
#define KEY_RIGHT 79 + nonprinting
#define KEY_LEFT 80 + nonprinting
#define KEY_DOWN 81 + nonprinting
#define KEY_UP 82 + nonprinting
#define KEY_NUM_LOCK 83 + nonprinting
#define KEYPAD_SLASH 84 + nonprinting
#define KEYPAD_ASTERIX 85 + nonprinting
#define KEYPAD_MINUS 86 + nonprinting
#define KEYPAD_PLUS 87 + nonprinting
#define KEYPAD_ENTER 88 + nonprinting
#define KEYPAD_1 89 + nonprinting
#define KEYPAD_2 90 + nonprinting
#define KEYPAD_3 91 + nonprinting
#define KEYPAD_4 92 + nonprinting
#define KEYPAD_5 93 + nonprinting
#define KEYPAD_6 94 + nonprinting
#define KEYPAD_7 95 + nonprinting
#define KEYPAD_8 96 + nonprinting
#define KEYPAD_9 97 + nonprinting
#define KEYPAD_0 98 + nonprinting
#define KEYPAD_PERIOD 99 + nonprinting

struct keys {
  uint8_t len;
  uint8_t seq[];
};

keys masterAlternatorKeys = { 2, { KEY_LEFT_ALT, 'a' } };
keys masterBatteryKeys = { 2, { KEY_LEFT_ALT, 'b' } };
keys masterKeys = { 1, { 'M' } };
keys avionics = { 1, { KEY_PAGE_UP } };
keys atcKeys = { 2, { KEY_SCROLL_LOCK, KEY_HOME } };
keys gearKeys = { 1, { 'g' } };
keys carbHeatKeys = { 1, { 'h' } };
keys pitotHeatKeys = { 1, { 'H' } };
keys mixtureIncreaseKeys = { 3, { KEY_LEFT_SHIFT, KEY_LEFT_CTRL, KEY_F3 } };
keys mixtureDecreaseKeys = { 3, { KEY_LEFT_SHIFT, KEY_LEFT_CTRL, KEY_F2 } };
// keys mixtureIncreaseKeys = { 2, { KEY_LEFT_CTRL, 'k' } };
// keys mixtureDecreaseKeys = { 2, { KEY_LEFT_ALT, 'k' } };
keys mixtureLeanKeys = { 3, { KEY_LEFT_SHIFT, KEY_LEFT_CTRL, KEY_F1 } };
keys mixtureRichKeys = { 3, { KEY_LEFT_SHIFT, KEY_LEFT_CTRL, KEY_F4 } };
keys fuelPumpKeys = { 2, { KEY_LEFT_CTRL, 'D' } };
keys taxiLightKeys = { 2, { KEY_LEFT_ALT, 'j' } };
keys navLightKeys = { 2, { KEY_LEFT_ALT, 'n' } };
keys strobeLightKeys = { 1, { 'o' } };
keys landingLightKeys = { 2, { KEY_LEFT_CTRL, 'l' } };
keys beaconLightKeys = { 2, { KEY_LEFT_ALT, 'h' } };
keys viewDashboardFirstKeys = { 2, { KEY_LEFT_CTRL, '1' } };
keys throtleIncreaseKeys = { 1, { KEY_F3 } };
keys throtleDecreaseKeys = { 1, { KEY_F2 } };
keys throtleCutKeys = { 1, { KEY_F1 } };
keys viewDashboardNextKeys = { 1, { 'a' } };
// keys trimDown = { 2, { KEY_LEFT_CTRL, 't' } };
// keys trimUp = { 1, { 'T' } };
keys trimDown = {1, { KEYPAD_7 }};  
keys trimUp = {1, { KEYPAD_1 }}; 

const uint8_t eventOn = 0;
const uint8_t eventOff = 1;
const uint8_t eventNext = 2;
const uint8_t eventPrev = 3;

struct button {
  String name;
  uint8_t pin;
  keys* keys[4];
  bool state;
  unsigned long debounce;
  int count;
  int max;
};

const int nButtons = 14;
button buttons[nButtons] = {
  { "nav light", 0, { &navLightKeys, &navLightKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "strobe light", 1, { &strobeLightKeys, &strobeLightKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "landing lights", 2, { &landingLightKeys, &landingLightKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "beacon light", 3, { &beaconLightKeys, &beaconLightKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "taxi lights", 4, { &taxiLightKeys, &taxiLightKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "pitot heat", 5, { &pitotHeatKeys, &pitotHeatKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "fuel pump", 6, { &fuelPumpKeys, &fuelPumpKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "free", 7, { NULL, NULL, NULL, NULL }, 0, 0, 0, 0 },
  { "master", 8, { &masterKeys, &masterKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "avionics", 9, { &avionics, &avionics, NULL, NULL }, 0, 0 },
  { "gear", 10, { &gearKeys, &gearKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "carby heat", 11, { &carbHeatKeys, &carbHeatKeys, NULL, NULL }, 0, 0, 0, 0 }
};

const int nPressureButtons = 2;
button pressureButtons[nPressureButtons] = {
  { "atc", 12, { &atcKeys, &atcKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "view", 13, { &viewDashboardFirstKeys, &viewDashboardNextKeys, NULL, NULL }, 0, 0, 0, 3 }
};

// https://docs.arduino.cc/learn/electronics/potentiometer-basics
const unsigned long potDebounceDelay = 64;  // the debounce time; increase if the output flickers
const int nPotButtons = 2;
button potButtons[nPotButtons] = {
  // { "a1", A1, { &throtleIncreaseKeys, &throtleCutKeys, &throtleIncreaseKeys, &throtleDecreaseKeys }, 0, 0, 0, 1024 },
  { "a2", A2, { &mixtureRichKeys, &mixtureLeanKeys, &mixtureIncreaseKeys, &mixtureDecreaseKeys }, 0, 0, 0, 1024 },
  { "a3", A3, { NULL, NULL, NULL, NULL }, 0, 0, 0, 1024 }
};

// Rotary encoder for trim wheel
// https://lastminuteengineers.com/rotary-encoder-arduino-tutorial/
const uint8_t trimCLKPin = A5;
const uint8_t trimDTPin = A4;
// const uint8_t trimSWPin =6;
int trimCLKState;
int trimCounter = 0;
unsigned long trimDebounceTime = 0;  // the last time the output pin was toggled


void pressKey(keys* keys, int repeat) {
  if (keys == NULL) {
    Serial.println("null keys");
    return;
  }

  unsigned const repeatDelay = 32;
  int i;

  for (i = 0; i < keys->len; i++) {
#ifdef LIVE
    int ret = Keyboard.press(keys->seq[i]);
    if (ret != 1) {
#ifndef LIVE
      Serial.println("error in press");
#endif
      return;
    }
#endif
  }
  // repeat = repeatDelay * repeat;
  if (repeat > 3000) {  // maximum 3000 ms delay
    repeat = 3000;
  }
  if (repeat <= repeatDelay) {
    repeat = repeatDelay;
  }
  delay(repeat);  // xbox does not work without this delay; need time for os to accept key press
#ifndef LIVE
  if (repeat > repeatDelay) {
    Serial.print("delay=");
    Serial.println(repeat);
  }
#endif

  for (i = keys->len; i > 0;) {
    i--;
#ifdef LIVE
    int ret = Keyboard.release(keys->seq[i]);
    if (ret != 1) {
#ifndef LIVE
      Serial.println("error in release");
#endif
      Keyboard.releaseAll();  // attempt to release numpad keys - fix bug of non stop repeating key
      return;
    }
#endif
  }

  // Keyboard.releaseAll(); // releaseAll send release with lower case characters only???
  // delay(32);
}

void processTrim() {
  // Read the current state of CLK
  int clk = digitalRead(trimCLKPin);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (clk != trimCLKState && clk == 1) {
    long t = millis();

    // if (t < trimDebounceTime) {
    // return;
    // }
    trimDebounceTime = t + 16;

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(trimDTPin) != clk) {
      trimCounter--;
      pressKey(&trimDown, 1);
      // delay(16);
      // pressKey(trimDown, sizeof(trimDown));
    } else {
      // Encoder is rotating CW so increment
      trimCounter++;
      pressKey(&trimUp, 1);
      // delay(16);
      // pressKey(trimUp, sizeof(trimUp));
    }

    Serial.print("Counter: ");
    Serial.println(trimCounter);
  }
  trimCLKState = clk;  // Remember last CLK state
}

void processPot(button* b) {
  int i;
  long t = millis();
  // if (t < b->debounce) {
  // return;
  // }
  int x = analogRead(b->pin);

    if (x == 0 && b->count != 0) {
    pressKey(b->keys[eventOff], 1);

    Serial.print(b->name);
    Serial.println("is off");
    b->count = 0;
   return;
  }
  if (x == 1023 && b->count != 1023) {
    pressKey(b->keys[eventOn], 1);
    Serial.print(b->name);
    Serial.println("is at max");
    b->count = 1023;
    return;
  }

  // reading may oscilate between +1 and -1 volts; ignore
  // https://forum.arduino.cc/t/debounce-a-potentiometer/7509
  if (x >= b->count - 64 && x <= b->count + 64) {
    return;
  }
#ifndef LIVE
  Serial.println(x);
#endif


  int difference = x - b->count;
  b->count = x;




  if (difference < 0) {

    Serial.print(b->name);
    Serial.print(" dec by");
    Serial.println(difference);
// for (i=0; i> difference; i -= 10){
    pressKey(b->keys[eventPrev], 1);
// }
  } else {

    Serial.print(b->name);
    Serial.print(" inc by");
    Serial.println(difference);
// for (i=0; i < difference; i+=10){
    pressKey(b->keys[eventNext], 1);
// }
  }
// Keyboard.releaseAll(); // trying to fix repeating bug


}

void processSwitch(button* b) {
  boolean x = digitalRead(b->pin);
  if (x == b->state) {
    return;
  }

  long t = millis();
  // if (t < b->debounce) {
    // return;
  // }
  // b->debounce = t + potDebounceDelay;
  b->state = x;
  Serial.print(b->name);
  Serial.print("pin=");
  Serial.print(b->pin);
  Serial.print(" keys=");
  Serial.print(b->keys[eventOn]->seq[0]);
  Serial.print("=");
  Serial.println(b->state);
  if (b->state == HIGH) {
    pressKey(b->keys[eventOn], 1);
  } else {
    pressKey(b->keys[eventOff], 1);
  }
}

void processPressureButton(button* b) {
  uint8_t x = digitalRead(b->pin);
  if (x == HIGH && x != b->state) {
    b->state = x;
    if (b->count == 0) {
      pressKey(b->keys[eventOn], 1);
#ifndef LIVE
      Serial.print(b->name);
      Serial.println(" pressed");
#endif
    } else {
      pressKey(b->keys[eventOff], 1);
#ifndef LIVE
      Serial.print(b->name);
      Serial.println(" released");
#endif
    };
    b->count++;
    if (b->count > b->max) {
      b->count = 0;
    };
  };
  b->state = x;
}

void setup() {
  int i;
  pinMode(LED_BUILTIN, OUTPUT);  // initialise led builtin as output
  pinMode(A0, INPUT_PULLUP);     // initalise control A0 with digital resistor (built in the board)

  for (i = 0; i < nButtons; i++) {
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }

  for (i = 0; i < nPressureButtons; i++) {
    pinMode(pressureButtons[i].pin, INPUT_PULLUP);
  }

  for (i = 0; i < nPotButtons; i++) {
    pinMode(potButtons[i].pin, INPUT);
    potButtons[i].count = analogRead(potButtons[i].pin);
  }

  // trim wheel
  pinMode(trimCLKPin, INPUT_PULLUP);
  pinMode(trimDTPin, INPUT_PULLUP);
  trimCLKState = digitalRead(trimCLKPin);  // Read the initial state of CLK

  Keyboard.begin();
  Keyboard.releaseAll();  // release any key that is pressed
}

void loop() {
  // System Disabled
  if (digitalRead(A0) != 0) {
    digitalWrite(LED_BUILTIN, LOW);
    return;
  }
  digitalWrite(LED_BUILTIN, HIGH);  // Turn indicator light on.

  int i;
  for (i = 0; i < nButtons; i++) {
    processSwitch(&buttons[i]);
  }

  for (i = 0; i < nPotButtons; i++) {
    processPot(&potButtons[i]);
  }

  for (i = 0; i < nPressureButtons; i++) {
    processPressureButton(&pressureButtons[i]);
  }

  processTrim();
}

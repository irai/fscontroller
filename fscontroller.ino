#include <Keyboard.h>     // Use built-in Keyboard library
#include <HID_Buttons.h>  // Must import AFTER Keyboard.h

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

const uint8_t masterAlternator[] = { KEY_LEFT_ALT, 'a' };
const uint8_t masterBattery[] = { KEY_LEFT_ALT, 'b' };
const uint8_t master[] = { KEY_LEFT_SHIFT, 'm' };
const uint8_t avionics[] = { KEY_PAGE_UP };
const uint8_t atcKeys[] = { KEY_SCROLL_LOCK, KEY_HOME };
const uint8_t gearKeys[] = { 'g' };
const uint8_t carbHeatKeys[] = { KEY_LEFT_ALT, 'n' };
const uint8_t mixturePin = 13;
// const uint8_t mixtureIncreaseKeys[] = { KEY_LEFT_SHIFT, KEY_LEFT_CTRL, KEY_F3 };
// const uint8_t mixtureDecreaseKeys[] = { KEY_LEFT_SHIFT, KEY_LEFT_CTRL, KEY_F2 };
const uint8_t mixtureIncreaseKeys[] = { KEY_LEFT_CTRL, 'k' };
const uint8_t mixtureDecreaseKeys[] = { KEY_LEFT_ALT, 'k' };

const uint8_t navLightKeys[] = { KEY_LEFT_ALT, 'n' };
const uint8_t strobeLightKeys[] = { 'o' };
const uint8_t landingLightKeys[] = { KEY_LEFT_CTRL, 'l' };
const uint8_t beaconLightKeys[] = { KEY_LEFT_ALT, 'h' };
const uint8_t viewDashboardFirstKeys[] = { KEY_LEFT_CTRL, '1' };
const uint8_t viewDashboardNextKeys[] = { 'a' };

const uint8_t trimDown[] = { KEY_LEFT_CTRL, 't' };
const uint8_t trimUp[] = { KEY_LEFT_SHIFT, 't' };
// const uint8_t trimDown[] = { KEYPAD_7 };  // NOTE: This does not seem to work - Apr 22
// const uint8_t trimUp[] = { KEYPAD_1 }; // This key does not seem to work

struct button {
  String name;
  uint8_t pin;
  uint8_t *onKeys;
  int onLen;
  uint8_t *offKeys;
  int offLen;
  bool state;
  unsigned long debounce;
  int count;
  int max;
};

const int nButtons = 10;
button buttons[nButtons] = {
  { "master", 2, master, sizeof(master), master, sizeof(master), 0, 0, 0, 0 },
  { "avionics", 3, avionics, sizeof(avionics), avionics, sizeof(avionics), 0, 0 },
  { "free", 6, NULL, 0, NULL, 0, 0, 0, 0, 0 },
  // { "free", 7, NULL, 0, NULL, 0, 0, 0 ,0,0},
  { "free", 8, NULL, 0, NULL, 0, 0, 0, 0, 0 },
  // { "free", 7, NULL, 0, NULL, 0, 0, 0 ,0,0},
  { "gear", 10, gearKeys, sizeof(gearKeys), gearKeys, sizeof(gearKeys), 0, 0, 0, 0 },
  { "carby heat", 11, carbHeatKeys, sizeof(carbHeatKeys), carbHeatKeys, sizeof(carbHeatKeys), 0, 0, 0, 0 },
  { "nav light", A5, navLightKeys, sizeof(navLightKeys), navLightKeys, sizeof(navLightKeys), 0, 0, 0, 0 },
  { "strobe light", A4, strobeLightKeys, sizeof(strobeLightKeys), strobeLightKeys, sizeof(strobeLightKeys), 0, 0, 0, 0 },
  { "landing lights", A3, landingLightKeys, sizeof(landingLightKeys), landingLightKeys, sizeof(landingLightKeys), 0, 0, 0, 0 },
  { "beacon", A2, beaconLightKeys, sizeof(beaconLightKeys), beaconLightKeys, sizeof(beaconLightKeys), 0, 0, 0, 0 }
};

const int nPressureButtons = 2;
button pressureButtons[nPressureButtons] = {
  { "atc", 9, atcKeys, sizeof(atcKeys), atcKeys, sizeof(atcKeys), 0, 0, 0, 0 },
  { "view", 7, viewDashboardFirstKeys, sizeof(viewDashboardFirstKeys), viewDashboardNextKeys, sizeof(viewDashboardNextKeys), 0, 0, 0, 3 }
};



// Rotary encoder for trim wheel
// https://lastminuteengineers.com/rotary-encoder-arduino-tutorial/
const uint8_t trimCLKPin = 4;
const uint8_t trimDTPin = 5;
// const uint8_t trimSWPin =6;
int trimCLKState;
int trimCounter = 0;
unsigned long trimDebounceTime = 0;  // the last time the output pin was toggled




// https://docs.arduino.cc/learn/electronics/potentiometer-basics

const unsigned long potDebounceDelay = 64;  // the debounce time; increase if the output flickers

const int nPotButtons = 1;
button potButtons[nPotButtons] = {
  { "mixture", A1, mixtureIncreaseKeys, sizeof(mixtureIncreaseKeys), mixtureDecreaseKeys, sizeof(mixtureDecreaseKeys), 0, 0, 0, 1024 }
};

void pressKey(uint8_t keys[], int n, int repeat) {
  unsigned const repeatDelay = 32;
  int i;
  for (i = 0; i < n; i++) {
    Keyboard.press(keys[i]);
  }
  repeat = repeatDelay*repeat;
  if (repeat > 3000) { // maximum 3000 ms delay
    repeat = 3000;
  }
  if (repeat <= repeatDelay ) {
    repeat = repeatDelay;
  }
  delay(repeat);    // xbox does not work without this delay; need time for os to accept key press
  if (repeat > repeatDelay) {
    Serial.print("delay=");
    Serial.println(repeat);
  }

  for (i = n; i > 0;) {
  i--;
  Keyboard.release(keys[i]);
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
      pressKey(trimDown, sizeof(trimDown),1);
      // delay(16);
      // pressKey(trimDown, sizeof(trimDown));

    } else {
      // Encoder is rotating CW so increment
      trimCounter++;
      pressKey(trimUp, sizeof(trimUp),1);
      // delay(16);
      // pressKey(trimUp, sizeof(trimUp));
    }

    Serial.print("Counter: ");
    Serial.println(trimCounter);
  }
  trimCLKState = clk;  // Remember last CLK state


  /**
	// Read the button state
	int btnState = digitalRead(trimSWPin);

	//If we detect LOW signal, button is pressed
	if (btnState == LOW) {
		//if 50ms have passed since last LOW pulse, it means that the
		//button has been pressed, released and pressed again
		if (millis() - lastButtonPress > 50) {
			Serial.println("Button pressed!");
		}

		// Remember last button press event
		lastButtonPress = millis();
	}

	// Put in a slight delay to help debounce the reading
	delay(1);

*/
}
void processPot(button *b) {
  int i;
  long t = millis();
  // if (t < b->debounce) {
  // return;
  // }
  int x = analogRead(b->pin);
  // reading may oscilate between +1 and -1 volts; ignore
  // https://forum.arduino.cc/t/debounce-a-potentiometer/75209
  if (x >= b->count - 10 && x <= b->count + 10) {
    return;
  }
  Serial.println(x);

  int difference = x - b->count;
  if (difference < 0) {
    Serial.print(b->name);
    Serial.print(" dec by");
    Serial.println(difference);
    // for (i = 0; i > difference; i--) {
    pressKey(b->offKeys, b->offLen, difference*-1 /10 +1);
    // Serial.println(i);
    // delay(16);
    // pressKey(b->offKeys, b->offLen);
    // }
  } else {
    Serial.print(b->name);
    Serial.print(" inc by");
    Serial.println(difference);
    // for (i = 0; i < difference; i++) {
    pressKey(b->onKeys, b->onLen, difference / 10 + 1);
    // delay(16);
    // pressKey(b->onKeys, b->onLen);
    // }
  }
  b->count = x;
}

void toggle(button *b) {
  boolean x = digitalRead(b->pin);
  if (x == b->state) {
    return;
  }

  long t = millis();
  if (t < b->debounce) {
    return;
  }
  b->debounce = t + potDebounceDelay;
  b->state = x;
  Serial.print(b->name);
  Serial.print("pin=");
  Serial.print(b->pin);
  Serial.print("=");
  Serial.println(b->state);
  if (b->state == HIGH) {
    pressKey(b->onKeys, b->onLen,1);
  } else {
    pressKey(b->offKeys, b->offLen,1);
  }
}

void setup() {
  int i;
  for (i = 0; i < nButtons; i++) {
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }

  for (i = 0; i < nPressureButtons; i++) {
    pinMode(pressureButtons[i].pin, INPUT_PULLUP);
  }

  pinMode(LED_BUILTIN, OUTPUT);  // initialise led builtin as output
  pinMode(A0, INPUT_PULLUP);     // initalise control A0 with digital resistor (built in the board)

  // trim wheel
  pinMode(trimCLKPin, INPUT_PULLUP);
  pinMode(trimDTPin, INPUT_PULLUP);
  // pinMode(trimSWPin, INPUT_PULLUP);
  trimCLKState = digitalRead(trimCLKPin);  // Read the initial state of CLK

  for (i = 0; i < nPotButtons; i++) {
    pinMode(potButtons[i].pin, INPUT);
    potButtons[i].count = analogRead(potButtons[i].pin);
  }

  Keyboard.begin();
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
    toggle(&buttons[i]);
  }

  for (i = 0; i < nPotButtons; i++) {
    processPot(&potButtons[i]);
  }

  button *b = &pressureButtons[0];
  uint8_t x = digitalRead(b->pin);
  if (x == HIGH && x != b->state) {
    b->state = x;
    if (b->count == 0) {
      pressKey(b->onKeys, b->onLen,1);
      Serial.print(b->name);
      Serial.println(" is first");
    } else {
      pressKey(b->offKeys, b->offLen,1);
      Serial.print(b->name);
      Serial.println(" is next");
    };
    b->count++;
    if (b->count > b->max) {
      b->count = 0;
    };
  };
  b->state = x;

  b = &pressureButtons[1];
  x = digitalRead(b->pin);
  if (x == HIGH && x != b->state) {
    b->state = x;
    if (b->count == 0) {
      pressKey(b->onKeys, b->onLen,1);
      Serial.print(b->name);
      Serial.println(" is first");
    } else {
      pressKey(b->offKeys, b->offLen,1);
      Serial.print(b->name);
      Serial.println(" is next");
    };
    b->count++;
    if (b->count >= b->max) {
      b->count = 0;
    };
  };
  b->state = x;

  processTrim();
}

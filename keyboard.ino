// keyboard.ino implements a simple FIFO queue to send keyboard press/release without using delay().
//
// USB Keyboard speed is limited to 500 keystrokes per second, and some operating systems may limit the speed
// to only 62.5 keystrokes per second. There is nothing you can do about these limits.
// https://www.pjrc.com/teensy/td_keyboard.html
//
// XBOX seems to require a delay of 32ms for each key message. It loses key strokes if less.
#define KEYBOARD_DELAY 32

static cmd* keyboardBuffer[64];
static unsigned int keyboardBufHead = 0;
static unsigned int keyboardBufTail = 0;
static unsigned long keyboardTimer = 0;
static int keyboardPos = 0;
static unsigned int keyboardModifier = 0;

void queueKeys(cmd* keys) {
  if (keys == NULL || keys->len < 1) {
    Serial.println("Keyboard buffer null");
    return;
  }

  if (keyboardBufTail >= sizeof(keyboardBuffer)/sizeof(cmd*)) {
    Serial.println("Keyboard buffer full - ignoring");
    return;
  }

  // add to the end of buffer
  keyboardBuffer[keyboardBufTail] = keys;
  keyboardBufTail++;
}

void sendKey() {
  static int state = 0;

  // empty queue?
  if (keyboardBufHead >= keyboardBufTail) {
    return;
  }

  unsigned long now = millis();
  if (now < keyboardTimer) {
    return;
  }

  if (state == 0) {
    sendPress(now);
    state = 1;
    return;
  }

  sendRelease(now);
  state = 0;
}

void sendPress(unsigned long now) {
  if (!DEBUG) {
    Keyboard.press(keyboardBuffer[keyboardBufHead]->seq[keyboardPos]);
  }
  Serial.print(" press ");
  Serial.print(keyboardBuffer[keyboardBufHead]->seq[keyboardPos]);
  keyboardTimer = now + KEYBOARD_DELAY;

  switch (keyboardBuffer[keyboardBufHead]->seq[keyboardPos]) {
    case KEY_LEFT_CTRL:
    case KEY_RIGHT_CTRL:
    case KEY_LEFT_ALT:
    case KEY_RIGHT_ALT:
    case KEY_LEFT_SHIFT:
    case KEY_RIGHT_SHIFT:
      keyboardModifier = keyboardBuffer[keyboardBufHead]->seq[keyboardPos];
      break;
  }
}

void sendRelease(unsigned long now) {
  switch (keyboardBuffer[keyboardBufHead]->seq[keyboardPos]) {
    case KEY_LEFT_CTRL:
    case KEY_RIGHT_CTRL:
    case KEY_LEFT_ALT:
    case KEY_RIGHT_ALT:
    case KEY_LEFT_SHIFT:
    case KEY_RIGHT_SHIFT:  // don't release modifiers
      break;

    default:
      if (!DEBUG) {
        Keyboard.release(keyboardBuffer[keyboardBufHead]->seq[keyboardPos]);
      }
      Serial.print(" rel ");
      Serial.print(keyboardBuffer[keyboardBufHead]->seq[keyboardPos]);
      keyboardTimer = now + KEYBOARD_DELAY;
  }

  keyboardPos++;
  if (keyboardPos >= keyboardBuffer[keyboardBufHead]->len) {  // last key in sequence?
    if (keyboardModifier != 0) {                              // release any modifiers
      if (!DEBUG) {
        Keyboard.release(keyboardModifier);
      }
      Serial.print(" rel mod ");
      Serial.print(keyboardModifier);
      keyboardTimer = now + KEYBOARD_DELAY;
    }
    Serial.println(".");

    keyboardBufHead++;
    keyboardPos = 0;
    // last queue element? reset queue
    if (keyboardBufHead >= keyboardBufTail) {
      keyboardBufHead = 0;
      keyboardBufTail = 0;
    }
  }
}

void pressKey(cmd* keys, int repeat) {
  if (keys == NULL) {
    Serial.println("null keys");
    return;
  }


  unsigned int keyboardModifier = 0;
  int i;

  for (i = 0; i < keys->len; i++) {
    if (!DEBUG) {
      Keyboard.press(keys->seq[i]);
    }
    Serial.print("press ");
    Serial.print(keys->seq[i]);
    delay(32);

    switch (keys->seq[i]) {
      case KEY_LEFT_CTRL:
      case KEY_RIGHT_CTRL:
      case KEY_LEFT_ALT:
      case KEY_RIGHT_ALT:
      case KEY_LEFT_SHIFT:
      case KEY_RIGHT_SHIFT:
        keyboardModifier = keys->seq[i];
        break;

      default:
        if (!DEBUG) {
          Keyboard.release(keys->seq[i]);
        }
        delay(32);
        Serial.print(" rel ");
        Serial.print(keys->seq[i]);
    }
  }

  if (keyboardModifier != 0) {
    if (!DEBUG) {
      Keyboard.release(keyboardModifier);
    }
    Serial.print(" rel mod ");
    Serial.print(keyboardModifier);
    delay(32);  // xbox does not work without this delay; need time for os to accept key press
  }
  Serial.println(".");
}


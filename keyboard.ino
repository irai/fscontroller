// keyboard.ino implements a simple FIFO queue to send keyboard press/release without using delay().
//
// USB Keyboard speed is limited to 500 keystrokes per second, and some operating systems may limit the speed
// to only 62.5 keystrokes per second. There is nothing you can do about these limits.
// https://www.pjrc.com/teensy/td_keyboard.html
//
// XBOX seems to require a delay of 32ms for each key press message. It loses key strokes if less.
#define KEYBOARD_PRESS_DELAY 32
#define KEYBOARD_RELEASE_DELAY 12  // release delay can be a bit smaller for xbox - 12 ms seems to work

#define RESET_KEYBOARD_BUFFER 0xAB  // magic number to reset keyboard buffer

#include "electronics.h"

// Flight simulator hub send single byte keystrokes that include special keys above 0x80.
// Must translate this to arduino/teensy codes.
struct keymap {
  uint16_t keycode;
  uint8_t fscode;
};

struct keymap kmap[] = {
#ifndef NO_KEYBOARD
  { KEY_LEFT_CTRL, (0 | 0x80) },
  { KEY_LEFT_SHIFT, (1 | 0x80) },
  { KEY_LEFT_ALT, (2 | 0x80) },
  { KEY_LEFT_GUI, (3 | 0x80) },
  { KEY_RIGHT_CTRL, (4 | 0x80) },
  { KEY_RIGHT_SHIFT, (5 | 0x80) },
  { KEY_RIGHT_ALT, (6 | 0x80) },
  { KEY_RIGHT_GUI, (7 | 0x80) },
  { KEY_UP_ARROW, (8 | 0x80) },
  { KEY_DOWN_ARROW, (9 | 0x80) },
  { KEY_LEFT_ARROW, (10 | 0x80) },
  { KEY_RIGHT_ARROW, (11 | 0x80) },
  { KEY_BACKSPACE, (12 | 0x80) },
  { KEY_TAB, (13 | 0x80) },
  { KEY_RETURN, (14 | 0x80) },
  { KEY_ESC, (15 | 0x80) },
  { KEY_INSERT, (16 | 0x80) },
  { KEY_DELETE, (17 | 0x80) },
  { KEY_PAGE_UP, (18 | 0x80) },
  { KEY_PAGE_DOWN, (19 | 0x80) },
  { KEY_HOME, (20 | 0x80) },
  { KEY_END, (21 | 0x80) },
  { KEY_CAPS_LOCK, (22 | 0x80) },
  { KEY_F1, (23 | 0x80) },
  { KEY_F2, (24 | 0x80) },
  { KEY_F3, (25 | 0x80) },
  { KEY_F4, (26 | 0x80) },
  { KEY_F5, (27 | 0x80) },
  { KEY_F6, (28 | 0x80) },
  { KEY_F7, (29 | 0x80) },
  { KEY_F8, (30 | 0x80) },
  { KEY_F9, (31 | 0x80) },
  { KEY_F10, (32 | 0x80) },
  { KEY_F11, (33 | 0x80) },
  { KEY_F12, (34 | 0x80) },
  { KEY_F13, (35 | 0x80) },
  { KEY_F14, (36 | 0x80) },
  { KEY_F15, (37 | 0x80) },
  { KEY_F16, (38 | 0x80) },
  { KEY_F17, (39 | 0x80) },
  { KEY_F18, (40 | 0x80) },
  { KEY_F19, (41 | 0x80) },
  { 0xAA, (42 | 0x80) },  // Marker: 0xAA do not translate
  { 0xAB, (43 | 0x80) },  // magic key to reset keyboard buffer
  { KEY_F22, (44 | 0x80) },
  { KEY_F23, (45 | 0x80) },
  { KEY_F24, (46 | 0x80) },
  { KEY_PRINTSCREEN, (47 | 0x80) },
  { KEY_SCROLL_LOCK, (48 | 0x80) },
  { KEY_PAUSE, (49 | 0x80) },
  { KEY_RIGHT, (50 | 0x80) },
  { KEY_LEFT, (51 | 0x80) },
  { KEY_DOWN, (52 | 0x80) },
  { KEY_UP, (53 | 0x80) },
  { KEY_NUM_LOCK, (54 | 0x80) },
  { KEYPAD_SLASH, (55 | 0x80) },
  { KEYPAD_ASTERIX, (56 | 0x80) },
  { KEYPAD_MINUS, (57 | 0x80) },
  { KEYPAD_PLUS, (58 | 0x80) },
  { KEYPAD_ENTER, (59 | 0x80) },
  { KEYPAD_1, (60 | 0x80) },
  { KEYPAD_2, (61 | 0x80) },
  { KEYPAD_3, (62 | 0x80) },
  { KEYPAD_4, (63 | 0x80) },
  { KEYPAD_5, (64 | 0x80) },
  { KEYPAD_6, (65 | 0x80) },
  { KEYPAD_7, (66 | 0x80) },
  { KEYPAD_8, (67 | 0x80) },
  { KEYPAD_9, (68 | 0x80) },
  { KEYPAD_0, (69 | 0x80) },
  { KEYPAD_PERIOD, (70 | 0x80) }
#endif
};

uint16_t mapFS2KeyboardCode(uint8_t c) {
  if ((c & 0x80) != 0x80) return uint16_t(c);
  uint16_t k = c & 0x7f;
  return kmap[k].keycode;
}

#ifndef KEY_LEFT_CTRL
// Modifiers
#define KEY_LEFT_CTRL 0x80
#define KEY_LEFT_SHIFT 0x81
#define KEY_LEFT_ALT 0x82
#define KEY_LEFT_GUI 0x83
#define KEY_RIGHT_CTRL 0x84
#define KEY_RIGHT_SHIFT 0x85
#define KEY_RIGHT_ALT 0x86
#define KEY_RIGHT_GUI 0x87


#endif

// very simple buffering model for key.
// we assume the buffer will never be full or wrap around. all keys must be transmitted before it reaches
// maximum capacity. we discard keys if the buffer is full.
static cmd* keyboardBuffer[512];
static unsigned int keyboardBufHead = 0;
static unsigned int keyboardBufTail = 0;
static unsigned long keyboardTimer = 0;
static int keyboardPos = 0;
// static unsigned int keyboardModifier[6];
// static int keyboardModifierCounter = 0;

void resetBuffer() {
  for (unsigned int i = keyboardBufHead+1; i < keyboardBufTail; i++) { // delete all queued after buffer head
    free(keyboardBuffer[i]);
  }
  keyboardBufTail = keyboardBufHead;
}

void queueKeys(uint8_t* keys, int n) {
  if (keys == NULL || n < 1) {
    debugHandler->println("Keyboard buffer null");
    return;
  }

  // reset buffer if first byte is a magical reset number
  if (keys[0] == RESET_KEYBOARD_BUFFER) {
    if (Debug_KEYBOARD) {
      debugHandler->println("Keyboard resetting buffer");
    }
    resetBuffer();
    return;
  }

  // assumption: buffer can never overlap - discard when full
  if (keyboardBufTail >= sizeof(keyboardBuffer) / sizeof(cmd*)) {
    debugHandler->println("Keyboard buffer full - ignoring");
    return;
  }

  // add keystrokes to the end of keyboard buffer
  cmd* c = (cmd*)malloc(sizeof(cmd) + n * 2);
  c->len = n;
  keyboardBuffer[keyboardBufTail] = c;
  for (int i = 0; i < n; i++) {
    c->seq[i] = mapFS2KeyboardCode(keys[i]);
  }

  if (Debug_KEYBOARD) {
    debugHandler->print("added keystrokes len=");
    debugHandler->print(n);
    debugHandler->print(" keys=[");
    for (int i = 0; i < n; i++) {
      debugHandler->print(keyboardBuffer[keyboardBufTail]->seq[i]);
      debugHandler->print(",");
    }
    debugHandler->println("]");
  }

  keyboardBufTail++;
}

void sendKeystrokeNonBlocking() {
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
  uint16_t key = keyboardBuffer[keyboardBufHead]->seq[keyboardPos];

#ifndef NO_KEYBOARD
  Keyboard.press(key);
#endif

  if (Debug_KEYBOARD) {
    debugHandler->print(" press key=");
    debugHandler->print((uint8_t)key);
  }

  keyboardTimer = now + KEYBOARD_PRESS_DELAY;
}

void sendRelease(unsigned long now) {
  uint16_t key = (keyboardBuffer[keyboardBufHead]->seq[keyboardPos]);
  keyboardPos++;

  if (keyboardPos < keyboardBuffer[keyboardBufHead]->len) {  // not the last key in sequence?
    switch (key) {
      case KEY_LEFT_CTRL:
      case KEY_RIGHT_CTRL:
      case KEY_LEFT_ALT:
      case KEY_RIGHT_ALT:
      case KEY_LEFT_SHIFT:
      case KEY_RIGHT_SHIFT:  // don't release modifiers
        break;

      default:
#ifndef NO_KEYBOARD
        Keyboard.release(key);  // release single key
#endif

        if (Debug_KEYBOARD) {
          debugHandler->print(" rel ");
          debugHandler->print(key);
        }
        // keyboardTimer = now + KEYBOARD_RELEASE_DELAY;
        keyboardTimer = now + KEYBOARD_PRESS_DELAY;  // use longer delay; it does not work for multiple chars otherwise
    }
    return;
  }

  // If we pass here, this is the last key in sequence
  // release all
#ifndef NO_KEYBOARD
  Keyboard.releaseAll();
#endif
  if (Debug_KEYBOARD) {
    debugHandler->println(" release all");
  }
  keyboardTimer = now + KEYBOARD_RELEASE_DELAY;
  free(keyboardBuffer[keyboardBufHead]);
  keyboardBufHead++;
  keyboardPos = 0;

  // last queue element? reset queue
  if (keyboardBufHead >= keyboardBufTail) {
    keyboardBufHead = 0;
    keyboardBufTail = 0;
  }
}

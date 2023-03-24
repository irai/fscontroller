// keyboard.ino implements a simple FIFO queue to send keyboard press/release without using delay().
//
// USB Keyboard speed is limited to 500 keystrokes per second, and some operating systems may limit the speed
// to only 62.5 keystrokes per second. There is nothing you can do about these limits.
// https://www.pjrc.com/teensy/td_keyboard.html
//
// XBOX seems to require a delay of 32ms for each key press message. It loses key strokes if less.
#define KEYBOARD_PRESS_DELAY 32
#define KEYBOARD_RELEASE_DELAY 12  // release delay can be a bit smaller for xbox - 12 ms seems to work

#include "electronics.h"

// Flight simulator hub send single byte keystrokes that include special keys above 0x80.
// Must translate this to arduino/teensy codes.
struct keymap {
  uint16_t keycode;
  uint8_t fscode;
};

struct keymap kmap[] = {
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
  { KEY_F20, (42 | 0x80) },
  { KEY_F21, (43 | 0x80) },
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
};

uint16_t mapFS2KeyboardCode(uint8_t c) {
  if ((c & 0x80) != 0x80) return uint16_t(c);
  uint16_t k = c & 0x7f;
  return kmap[k].keycode;
}

#ifdef NO_KEYBOARD
// Modifiers
#define KEY_LEFT_CTRL 0x80
#define KEY_LEFT_SHIFT 0x81
#define KEY_LEFT_ALT 0x82
#define KEY_LEFT_GUI 0x83
#define KEY_RIGHT_CTRL 0x84
#define KEY_RIGHT_SHIFT 0x85
#define KEY_RIGHT_ALT 0x86
#define KEY_RIGHT_GUI 0x87

// Misc keys
#define KEY_UP_ARROW 0xDA
#define KEY_DOWN_ARROW 0xD9
#define KEY_LEFT_ARROW 0xD8
#define KEY_RIGHT_ARROW 0xD7
#define KEY_BACKSPACE 0xB2
#define KEY_TAB 0xB3
#define KEY_RETURN 0xB0
#define KEY_MENU 0xED  // "Keyboard Application" in USB standard
#define KEY_ESC 0xB1
#define KEY_INSERT 0xD1
#define KEY_DELETE 0xD4
#define KEY_PAGE_UP 0xD3
#define KEY_PAGE_DOWN 0xD6
#define KEY_HOME 0xD2
#define KEY_END 0xD5
#define KEY_CAPS_LOCK 0xC1
#define KEY_PRINT_SCREEN 0xCE  // Print Screen / SysRq
#define KEY_SCROLL_LOCK 0xCF
#define KEY_PAUSE 0xD0  // Pause / Break

// Numeric keypad
#define KEY_NUM_LOCK 0xDB
#define KEY_KP_SLASH 0xDC
#define KEY_KP_ASTERISK 0xDD
#define KEY_KP_MINUS 0xDE
#define KEY_KP_PLUS 0xDF
#define KEY_KP_ENTER 0xE0
#define KEY_KP_1 0xE1
#define KEY_KP_2 0xE2
#define KEY_KP_3 0xE3
#define KEY_KP_4 0xE4
#define KEY_KP_5 0xE5
#define KEY_KP_6 0xE6
#define KEY_KP_7 0xE7
#define KEY_KP_8 0xE8
#define KEY_KP_9 0xE9
#define KEY_KP_0 0xEA
#define KEY_KP_DOT 0xEB

// Function keys
#define KEY_F1 0xC2
#define KEY_F2 0xC3
#define KEY_F3 0xC4
#define KEY_F4 0xC5
#define KEY_F5 0xC6
#define KEY_F6 0xC7
#define KEY_F7 0xC8
#define KEY_F8 0xC9
#define KEY_F9 0xCA
#define KEY_F10 0xCB
#define KEY_F11 0xCC
#define KEY_F12 0xCD
#define KEY_F13 0xF0
#define KEY_F14 0xF1
#define KEY_F15 0xF2
#define KEY_F16 0xF3
#define KEY_F17 0xF4
#define KEY_F18 0xF5
#define KEY_F19 0xF6
#define KEY_F20 0xF7
#define KEY_F21 0xF8
#define KEY_F22 0xF9
#define KEY_F23 0xFA
#define KEY_F24 0xFB
#endif

static cmd* keyboardBuffer[512];
static unsigned int keyboardBufHead = 0;
static unsigned int keyboardBufTail = 0;
static unsigned long keyboardTimer = 0;
static int keyboardPos = 0;
// static unsigned int keyboardModifier[6];
// static int keyboardModifierCounter = 0;

void queueKeys(uint8_t* keys, int n) {
  if (keys == NULL || n < 1) {
    debugHandler->println("Keyboard buffer null");
    return;
  }

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
  // memcpy(&c->seq, keys, n);

#ifdef DEBUG
  debugHandler->print("debug added keystrokes len=");
  debugHandler->print(n);
  debugHandler->print(" keys=[");
  for (int i = 0; i < n; i++) {
    debugHandler->print(keyboardBuffer[keyboardBufTail]->seq[i]);
    debugHandler->print(",");
  }
  debugHandler->println("]");
#endif

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

#ifdef DEBUG
  debugHandler->print(" press key=");
  debugHandler->print((uint8_t)key);
#endif

  keyboardTimer = now + KEYBOARD_PRESS_DELAY;

  /*
  switch (key) {
    case KEY_LEFT_CTRL:
    case KEY_RIGHT_CTRL:
    case KEY_LEFT_ALT:
    case KEY_RIGHT_ALT:
    case KEY_LEFT_SHIFT:
    case KEY_RIGHT_SHIFT: 
      keyboardModifier[keyboardModifierCounter] = keyboardBuffer[keyboardBufHead]->seq[keyboardPos];
      keyboardModifierCounter++;
      break;
  }
  */
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

#ifdef DEBUG
        debugHandler->print(" rel ");
        debugHandler->print(key);
#endif
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
#ifdef DEBUG
  debugHandler->println(" release all");
#endif
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


const uint8_t MARKER = 0xAA;
const uint8_t MARKER_ASCII = 'A';  // used for debugging purposes


void txPanel(Stream* s, String name) {
  uint8_t buf[3 + name.length()];
  buf[0] = PANEL;
  buf[1] = PANEL_KEYBOARD;
  buf[2] = uint8_t(name.length());
  int n = 3;
  for (unsigned int i = 0; i < name.length(); i++) {
    buf[n++] = name[i];
  }
  WriteMsg(s, (uint8_t*)&buf, n);
}

void txButton(Stream* s, uint8_t id, uint8_t value) {
  s->write(MARKER);
  s->write(3);
  s->write(BUTTON);
  s->write(id);
  s->write(value);
  s->flush();
}

void txSwitch(Stream* s, uint8_t id, uint8_t value) {
  s->write(MARKER);
  s->write(3);
  s->write(SWITCH);
  s->write(id);
  s->write(value);
  s->flush();
}

void txPot(Stream* s, uint8_t id, uint16_t value) {
  s->write(MARKER);
  s->write(4);
  s->write(POT);
  s->write(id);
  s->write(value >> 8);
  s->write(value);
  s->flush();
}

void txRotary(Stream* s, uint8_t id, int8_t value) {
  s->write(MARKER);
  s->write(3);
  s->write(ROTARY);
  s->write(id);
  s->write(value);
  s->flush();
}

static unsigned long nextTime = 0;

void testSerial(Stream* s) {

  unsigned long now = millis();
  if (nextTime > now) {
    return;
  }
  nextTime = now + 1000 * 10;
  s->write('A');
  //   Serial.write('A');
  //   Serial.write('A');
  //   Serial.write('A');
  //   Serial.write('\n');
  //   txButton(1, 1);
  //   txButton(2, 0);
  //   txButton(3, 1);
  //   txSwitch(4, 1);
  //   txPot(5, 999);
  //   txRotary(5, 1);
  //   txRotary(5, -1);
}

const int STATE_MARKER = 1;
const int STATE_LEN = 2;
const int STATE_DATA = 3;

SerialMsg* NewSerialMsg(Stream* s) {
  SerialMsg* h = (SerialMsg*)malloc(sizeof(SerialMsg));
  h->state = STATE_MARKER;
  h->Port = s;
  h->count = 0;
  h->timeout = 0;
  return h;
}

void SerialClose(SerialMsg* h) {
  free(h);
}

int WriteMsg(Stream* h, uint8_t* b, int l) {
  h->write(MARKER);
  h->write(l);
  for (int i = 0; i < l; i++) {
    h->write(b[i]);
  }
  h->flush();
  return l;
}



void resetBuffer(SerialMsg* h, String msg) {
#ifdef DEBUG
  debugHandler->print(msg + ": resetting buffer [");
  for (unsigned int i = 0; i < h->count; i++) {
    debugHandler->print(h->buffer[i], DEC);
    debugHandler->print(",");
  }
  debugHandler->println("]");
  debugHandler->flush();
#endif
  h->dataLen = 0;
  h->count = 0;
  h->timeout = 0;
  h->state = STATE_MARKER;
}

int ReadMsgNonBlocking(SerialMsg* h, uint8_t* b, int l) {
  if (!h->Port->available()) {
    if (h->timeout > 0 && h->timeout <= millis()) {
      resetBuffer(h, "rx timeout");
    }
    return -1;
  }

  // read additional characters
  // return error if buffer is full
  if (h->count >= sizeof(h->buffer) / sizeof(uint8_t)) {
    resetBuffer(h, "buffer full");
    return -1;
  }

  uint8_t c = h->Port->read();
  if (c == -1) {
    return -1;
  }

  debugHandler->print("rx=");
  debugHandler->println(c);
  debugHandler->flush();

#ifdef ENABLE_ASCII_MSG
  // hack to be able to send messages over ide for testing
  // Send a message like: A3100 - len 3, type 1, two bytes
  if (c == 'A') {
    c = MARKER;
  }
  if (c >= '0' && c <= '9') {
    c -= '0';
  }
#endif

  switch (h->state) {
    case STATE_MARKER:
      if (c != MARKER) {
        debugHandler->print("marker=");
        debugHandler->println(c, HEX);
        resetBuffer(h, "invalid marker");
        return -1;
      }
      h->state = STATE_LEN;
      h->timeout = millis() + 1000 *3; // 3 seconds to rx msg
      return 0;

    case STATE_LEN:
      if (c == 0 || c == 0xff) {
        resetBuffer(h, "invalid len");
        return -1;
      }
      if (c > sizeof(h->buffer)/sizeof(uint8_t)) {
        resetBuffer(h, "msg too big");
        return -1;
      }
      h->dataLen = c;
      h->count = 0;
      h->state = STATE_DATA;
      return 0;

    default:  // STATE_DATA
      h->buffer[h->count] = c;
      h->dataLen--;
      h->count++;
      if (h->dataLen > 0) {
        return 0;
      }

      // return the full msg to the caller
      int n = h->count;
      memcpy(b, h->buffer, h->count);
      h->state = STATE_MARKER;
      h->count = 0;
      return n;
  }
}

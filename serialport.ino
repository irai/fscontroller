// Marker
#ifndef DEBUG
const uint8_t MARKER = 0xAA;
#else
const uint8_t MARKER = 'A';
#endif




void txPanel(Stream* s, String name) {
  int n = 0;
  uint8_t buf[3];
  buf[n++] = PANEL;
  for (int i = 0; i < name.length(); i++) {
    buf[n++] = name[i];
  }
  WriteMsg(s, (uint8_t*)&buf, n);
}

void txButton(Stream* s, uint8_t id, uint8_t value) {
#ifndef DEBUG
  s->write(MARKER);
  s->write(3);
  s->write(BUTTON);
  s->write(id);
  s->write(value);
#endif
  return NULL;
}

void txSwitch(Stream* s, uint8_t id, uint8_t value) {
#ifndef DEBUG
  s->write(MARKER);
  s->write(3);
  s->write(SWITCH);
  s->write(id);
  s->write(value);
#endif
  return NULL;
}

void txPot(Stream* s, uint8_t id, uint16_t value) {
#ifndef DEBUG
  s->write(MARKER);
  s->write(4);
  s->write(POT);
  s->write(id);
  s->write(value >> 8);
  s->write(value);
#endif
  return NULL;
}

void txRotary(Stream* s, uint8_t id, int8_t value) {
#ifndef DEBUG
  s->write(MARKER);
  s->write(3);
  s->write(ROTARY);
  s->write(id);
  s->write(value);
#endif
  return NULL;
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





int WriteMsg(Stream* h, uint8_t* b, int l) {
  h->write(MARKER);
  h->write(l);
  for (int i = 0; i < l; i++) {
    h->write(b[i]);
  }
  return l;
}

int findMarker(uint8_t* b, int start, int end, char mark) {
  for (int i = start; i < end; i++) {
    if (b[i] == mark) {
      return i;
    }
  }
  return -1;
}

int ReadMsgNonBlocking(SerialMsg* h, uint8_t* b, int l) {
  int marker = 0;
  int n = 0;

  while (true) {
    marker = findMarker(h->buffer, h->head, h->count, MARKER);

    if (marker != -1) {
      if (marker != h->head) {  // error if frame does not start with marker
        h->head = marker;
#ifdef DEBUG
        Serial.println("invalid header");
#endif
        return -1;
      }
      if (marker + 1 < h->count) {  // do we have a len field?
        n = int(h->buffer[marker + 1]);
        if (marker + 2 + n <= h->count) {
          break;
        }
      }
    }

    // read additional characters
    // return error if buffer is full
    // and we don't have a marker
    if (h->count >= sizeof(h->buffer) / sizeof(uint8_t)) {
      h->head = 0;
      h->count = 0;
#ifdef DEBUG
      Serial.println("reseting buffer");
#endif
      return -1;
    }

    if (!h->Port->available()) {
      return -1;
    }

    h->buffer[h->count] = h->Port->read();
    if (h->buffer[h->count] == -1) {
#ifdef DEBUG
      Serial.println("error reading serial");
#endif
      return -1;
    }
#ifdef DEBUG
    // hack to be able to send messages over ide for testing
    // Send a message like: A3100 - len 3, type 1, two bytes
    if (h->buffer[h->count] >= '0' && h->buffer[h->count] <= '9') {
      h->buffer[h->count] -= '0';
    }
#endif
    h->count++;
  }

  if (n > 0) {
    for (int i = 0; i < n; i++) {
      b[i] = h->buffer[marker + 2 + i];
    }
    // n = copy(b, h->buffer[marker+2:marker+2+n])
    // fmt.Printf("new msg count=%d head=%d %v\n", h->count, h->head, b[0:n])
  }
  h->head = marker + 2 + n;
  if (h->head >= h->count) {  // reset buffer offset if empty
#ifdef DEBUG
    Serial.println("end of buffer");
#endif
    h->count = 0;
    h->head = 0;
  }
  return n;
}
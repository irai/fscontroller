
#include "CRC8.h"

const uint8_t MARKER = 0xAA;
const uint8_t MARKER_ASCII = 'A';  // used for debugging purposes

CRC8 crc8;

uint8_t checksum(uint8_t* buf, uint8_t n) {
  crc8.restart();
  crc8.add(MARKER);  // include header in calculation
  crc8.add(n);
  crc8.add(buf, n);
  return crc8.getCRC();
}


void txPanel(Stream* s, String name) {
  uint8_t buf[3 + 1 +  name.length() + 1 + sizeof(statistics)];
  int n = 0;
  buf[n++] = PANEL;                   // pos 0: msg id
  buf[n++] = 1;                       // pos 1: protocol version 1
  buf[n++] = KEYBOARD_FLAG;           // pos 2: flags
  buf[n++] = uint8_t(name.length());  // pos 3: string len
  for (unsigned int i = 0; i < name.length(); i++) {
    buf[n++] = name[i];
  }
  buf[n++] = sizeof(statistics) / sizeof(uint16_t);  // number of stats included
  for (unsigned int i = 0; i < sizeof(statistics) / sizeof(uint16_t); i++) {
    buf[n++] = stats.stats[i] >> 8;
    buf[n++] = stats.stats[i];
  }
  WriteMsg(s,(uint8_t *)&buf, n);
}

void txButton(Stream* s, uint8_t id, uint8_t value) {
  uint8_t buf[3];
  buf[0] = BUTTON;
  buf[1] = id;
  buf[2] = value;
  WriteMsg(s,(uint8_t *)&buf, sizeof(buf));
}

void txSwitch(Stream* s, uint8_t id, uint8_t value) {
  uint8_t buf[3];
  buf[0] = SWITCH;
  buf[1] = id;
  buf[2] = value;
  WriteMsg(s,(uint8_t *)&buf, sizeof(buf));
}

void txPot(Stream* s, uint8_t id, uint16_t value) {
  uint8_t buf[4];
  buf[0] = POT;
  buf[1] = id;
  buf[2] = value >> 8;
  buf[3] = value;
  WriteMsg(s,(uint8_t *)&buf, sizeof(buf));
}

void txRotary(Stream* s, uint8_t id, int8_t value) {
  uint8_t buf[3];
  buf[0] = ROTARY;
  buf[1] = id;
  buf[2] = value;
  WriteMsg(s,(uint8_t *)&buf, sizeof(buf));
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
  h->write(b, l);
  h->write(checksum(b,l));
  h->flush();
  stats.stats[StatsTxMsgs]++;
  return l;
}



void resetBuffer(SerialMsg* h, String msg) {
  if (Debug) {
    debugHandler->print(msg + ": resetting buffer [");
    for (unsigned int i = 0; i < h->count; i++) {
      debugHandler->print(h->buffer[i], DEC);
      debugHandler->print(",");
    }
    debugHandler->println("]");
    debugHandler->flush();
  }
  h->dataLen = 0;
  h->count = 0;
  h->timeout = 0;
  h->state = STATE_MARKER;
}

int ReadMsgNonBlocking(SerialMsg* h, uint8_t* b, int l) {
  if (!h->Port->available()) {
    if (h->timeout > 0 && h->timeout <= millis()) {
      stats.stats[StatsTimeoutErrors]++;
      resetBuffer(h, "rx timeout");
    }
    return -1;
  }

  // read additional characters
  // return error if buffer is full
  if (h->count >= sizeof(h->buffer) / sizeof(uint8_t)) {
    stats.stats[StatsRxErrors]++;
    resetBuffer(h, "buffer full");
    return -1;
  }

  uint8_t c = h->Port->read();
  if (c == -1) {
    return -1;
  }

  // debugHandler->print("rx=");
  // debugHandler->println(c);
  // debugHandler->flush();

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
        stats.stats[StatsRxErrors]++;
        resetBuffer(h, "invalid marker");
        return -1;
      }
      h->state = STATE_LEN;
      h->timeout = millis() + 1000 * 3;  // 3 seconds to rx msg
      return 0;

    case STATE_LEN:
      if (c == 0 || c == 0xff) {
        resetBuffer(h, "invalid len");
        return -1;
      }
      if (c > sizeof(h->buffer) / sizeof(uint8_t)) {
        resetBuffer(h, "msg too big");
        return -1;
      }
      h->dataLen = c + 1;  // add 1 for checksum
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

      // checksum validation
      uint8_t cs = checksum(h->buffer, h->count - 1);
      if (h->buffer[h->count - 1] != cs) {
        debugHandler->print("invalid checksum=");
        debugHandler->print(h->buffer[h->count - 1]);
        debugHandler->print(" wanted=");
        debugHandler->println(cs);
        resetBuffer(h, "invalid checksum");
        return -1;
      }

      // return the full msg to the caller minus checksum
      int n = h->count - 1;
      memcpy(b, h->buffer, n);
      h->state = STATE_MARKER;
      h->count = 0;
      stats.stats[StatsRxMsgs]++;
      return n;
  }
}

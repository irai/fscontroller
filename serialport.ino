#include "CRC8.h"

CRC8 crc8;

uint8_t checksum(uint8_t* buf, uint8_t n) {
  crc8.restart();
  crc8.add(MARKER);  // include header in calculation
  crc8.add(n);
  crc8.add(buf, n);
  return crc8.getCRC();
}

void txPanel(SerialMsg* s, String name) {
  s.buildMsg("panel");
  s.buildMsg(',');
  s.buildMsg(name);
  // uint8_t buf[3 + 1 + name.length() + 1 + sizeof(statistics)];
  // int n = 0;
  // buf[n++] = PANEL;                   // pos 0: msg id
  // buf[n++] = 1;                       // pos 1: protocol version 1
  // buf[n++] = 0;                       // pos 2: flags
  // buf[n++] = uint8_t(name.length());  // pos 3: string len
  // for (unsigned int i = 0; i < name.length(); i++) {
  //   buf[n++] = name[i];
  // }
  // buf[n++] = sizeof(statistics) / sizeof(uint16_t);  // number of stats included
  for (unsigned int i = 0; i < sizeof(statistics) / sizeof(uint16_t); i++) {
    s.buildMsg(',');
    s.buildMsg(stats.stats[i]);
    // buf[n++] = stats.stats[i] >> 8;
    // buf[n++] = stats.stats[i];
  }
  WriteMsg(s);
}

void txPin(SerialMsg* s, const char* msgid, uint8_t pin, int16_t value) {
  s.buildMsg(msgid);
  s.buildMsg(',');
  s.buildMsg(pin);
  s.buildMsg(',');
  s.buildMsg(value);
  // buf[0] = msgid;
  // buf[1] = pin;
  // buf[2] = value >> 8;  // keep sign bit
  // buf[3] = value;
  WriteMsg(s);
}

void txSwitch(Stream* s, uint8_t pin, uint16_t value) {
  txPin(s, "switch", pin, value);
}
void txPot(Stream* s, uint8_t pin, uint16_t value) {
  txPin(s, "pot", pin, value);
}
void txRotary(Stream* s, uint8_t pin, int8_t value) {
  txPin(s, "rotary", pin, value);
}

SerialMsg* NewSerialMsg(Stream* s) {
  SerialMsg* h = (SerialMsg*)malloc(sizeof(SerialMsg));
  h->Port = s;
  h->count = 0;
  h->timeout = 0;
  return h;
}

void SerialClose(SerialMsg* h) {
  free(h);
}

void buildMsg(SerialMsg* h, char* b) {
  int len = strlen(b);
  if (h->txBufferLen + len < sizeof(h->txBuffer) / sizeof(char)) {
    strcpy(h->buffer + h->txBufferLen, b);
    h->txBufferLen += len;
  }
}

int WriteMsg(SerialMsg* h, buffer * buf, char* b) {
  h->buildMsg(',');
  h->buildMsg(checksum(b, l));
  h->buildMsg('\n');
  h->port.print(h.txBuffer);
  h->port.flush()
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
}

int ReadMsgNonBlocking(SerialMsg* h, char* b, int l) {

  // read additional characters
  // return error if buffer is full
  for (; h->count < sizeof(h->buffer) / sizeof(uint8_t);) {

    if (!h->Port->available()) {
      if (h->timeout > 0 && h->timeout <= millis()) {
        stats.stats[StatsTimeoutErrors]++;
        resetBuffer(h, "rx timeout");
      }
      return -1;
    }

    uint8_t c = h->Port->read();
    if (c == 255) {
      return -1;
    }

    if (c == '\r') {  // skip CR
      continue;
    }
    if (c == '\n') {                 // Stop reading on LF
      h->buffer[h->count++] = 0x00;  // null terminate
      break;
    }
    h->buffer[h->count++] = c;
    if (h->timeout == 0) {
      h->timeout = millis() + 1000;
    }
  }

  if (h->count >= sizeof(h->buffer) / sizeof(uint8_t)) {
    stats.stats[StatsRxErrors]++;
    resetBuffer(h, "msg too big");
    return -1;
  }

  int n = h->count;
  memcpy(b, h->buffer, n);
  h->count = 0;
  h->timeout = 0;
  stats.stats[StatsRxMsgs]++;
  return n;
}

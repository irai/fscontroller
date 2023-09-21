#include <CRC8.h>

CRC8 crc8;

uint8_t checksum(const char* buf, uint8_t n) {
  crc8.restart();
  crc8.add((const uint8_t *) buf, n);
  return crc8.calc();
}

void txPanel(SerialMsg* s, const char * name) {
  buildMsg(s,"panel");
  buildMsg(s,",");
  buildMsg(s,name);
  for (unsigned int i = 0; i < sizeof(statistics) / sizeof(uint16_t); i++) {
    buildMsg(s,",");
    buildMsg(s,stats.stats[i]);
  }
  WriteMsg(s);

}

void txPin(SerialMsg* s, const char* msgid, uint8_t pin, int16_t value) {
  buildMsg(s,msgid);
  buildMsg(s,",");
  buildMsg(s,pin);
  buildMsg(s,",");
  buildMsg(s,value);
  // buf[0] = msgid;
  // buf[1] = pin;
  // buf[2] = value >> 8;  // keep sign bit
  // buf[3] = value;
  WriteMsg(s);
}

void txSwitch(SerialMsg* s, uint8_t pin, uint16_t value) {
  txPin(s, "switch", pin, value);
}
void txPot(SerialMsg* s, uint8_t pin, uint16_t value) {
  txPin(s, "pot", pin, value);
}
void txRotary(SerialMsg* s, uint8_t pin, int8_t value) {
  txPin(s, "rotary", pin, value);
}

SerialMsg* NewSerialMsg(Stream* s) {
  SerialMsg* h = (SerialMsg*)malloc(sizeof(SerialMsg));
  h->Port = s;
  h->rxcount = 0;
  h->timeout = 0;
  h->txcount = 0;
  return h;
}

void SerialClose(SerialMsg* h) {
  free(h);
}

void buildMsg(SerialMsg* h, const char* b) {
  int len = strlen(b);
  if (h->txcount + len < sizeof(h->txbuf) / sizeof(char)) {
    strcpy(h->txbuf + h->txcount, b);
    h->txcount += len;
  }
}

void buildMsg(SerialMsg* h, int b) {
char myBuffer[10];   // Create a character array to store the string

itoa(b, myBuffer, sizeof(myBuffer));
buildMsg(h,myBuffer);
}

void WriteMsg(SerialMsg* h) {
  buildMsg(h,",");
  buildMsg(h,checksum((char *)&h->txbuf, h->txcount));
  buildMsg(h,"\n");
  h->Port->write(h->txbuf, h->txcount);
  h->Port->flush();
  stats.stats[StatsTxMsgs]++;
  h->txcount = 0;
}

void resetBuffer(SerialMsg* h, String msg) {
  if (Debug) {
    debugHandler->print(msg + ": resetting rxbuf [");
    for (unsigned int i = 0; i < h->rxcount; i++) {
      debugHandler->print(h->rxbuf[i], DEC);
      debugHandler->print(",");
    }
    debugHandler->println("]");
    debugHandler->flush();
  }
  h->rxcount = 0;
  h->timeout = 0;
}

int ReadMsgNonBlocking(SerialMsg* h, char* b, int l) {

  // read additional characters
  // return error if rxbuf is full
  for (; h->rxcount < sizeof(h->rxbuf) / sizeof(uint8_t);) {

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
      h->rxbuf[h->rxcount++] = 0x00;  // null terminate
      break;
    }
    h->rxbuf[h->rxcount++] = c;
    if (h->timeout == 0) {
      h->timeout = millis() + 1000;
    }
  }

  if (h->rxcount >= sizeof(h->rxbuf) / sizeof(uint8_t)) {
    stats.stats[StatsRxErrors]++;
    resetBuffer(h, "msg too big");
    return -1;
  }

  int n = h->rxcount;
  memcpy(b, h->rxbuf, n);
  h->rxcount = 0;
  h->timeout = 0;
  stats.stats[StatsRxMsgs]++;
  return n;
}

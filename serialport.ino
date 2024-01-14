#include <CRC8.h>

CRC8 crc8;

uint8_t checksum(const char* buf, uint8_t n) {
  crc8.restart();
  crc8.add((const uint8_t*)buf, n);
  return crc8.calc();
}

// protocol - 
//     version 1.0.0 - string based csv
//             1.1.0 - add support for set, inc, set_n, inc_n actions         
const char serialVersion[] = "serial-1.1.0";

typedef struct SerialMsg {
  Stream* Port;
  char rxbuf[256];
  unsigned int rxcount;
  char txbuf[256];
  unsigned int txcount;
  unsigned long timeout;
} SerialMsg;

void txPanel(SerialMsg* s, const char* name, const char* panelVersion) {
  buildMsgStr(s, panelToken);
  buildMsgStr(s, ",");
  buildMsgStr(s, name);
  buildMsgStr(s, ",");
  buildMsgInt(s, serialVersion);
  buildMsgStr(s, ",");
  buildMsgInt(s, panelVersion);
  for (unsigned int i = 0; i < sizeof(statistics) / sizeof(uint16_t); i++) {
    buildMsgStr(s, ",");
    buildMsgInt(s, stats.stats[i]);
  }
  WriteMsg(s);
}

void txNotification(SerialMsg* s, const char* notification) {
  buildMsgStr(s, notificationToken);
  buildMsgStr(s, ",");
  buildMsgStr(s, notification);
  WriteMsg(s);
}

void txPin(SerialMsg* s, const char* msgid, uint8_t pin, int16_t value) {
  buildMsgStr(s, msgid);
  buildMsgStr(s, ",");
  buildMsgInt(s, pin);
  buildMsgStr(s, ",");
  buildMsgInt(s, value);
  WriteMsg(s);
}

void txAction(SerialMsg* s, const char* msgid, const char* variable, int16_t index, float value) {
  buildMsgStr(s, msgid);
  buildMsgStr(s, ",");
  buildMsgStr(s, variable);
  buildMsgStr(s, ",");
  if (index != -1) {
    buildMsgInt(s, index);
    buildMsgStr(s, ",");
  }
  buildMsgFloat(s, value);
  WriteMsg(s);
}

void txSwitch(SerialMsg* s, uint8_t pin, uint16_t value) {
  txPin(s, "switch", pin, value);
}

// void txPot(SerialMsg* s, uint8_t pin, const char* variable, uint16_t value) {
//   txAction(s, "set_n", variable, 0, value);
// }

// void txRotary(SerialMsg* s, const char* variable, int8_t index, int8_t value) {
//   txAction(s, "set", variable, index value);
// }

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

void buildMsgStr(SerialMsg* h, const char* b) {
  int len = strlen(b);
  if (h->txcount + len < sizeof(h->txbuf) / sizeof(char)) {
    strcpy(h->txbuf + h->txcount, b);
    h->txcount += len;
  }
}

void buildMsgInt(SerialMsg* h, int b) {
  char myBuffer[10];  // Create a character array to store the string

  itoa(b, myBuffer, sizeof(myBuffer));
  buildMsgStr(h, myBuffer);
}

void buildMsgFloat(SerialMsg* h, float b) {
  char myBuffer[32];  // Create a character array to store the string

  dtostrf(b, sizeof(myBuffer) - 1, 6, myBuffer);  // 6 digits decimal precision
  char* startPtr = myBuffer;
  while (*startPtr == ' ') {
    startPtr++;
  }
  buildMsgStr(h, startPtr);
}

void WriteMsg(SerialMsg* h) {
  buildMsgStr(h, ",");
  buildMsgInt(h, checksum((char*)&h->txbuf, h->txcount));
  buildMsgStr(h, "\n");
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
    if (c == '\n') {                  // Stop reading on LF
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

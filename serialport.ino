// message types
const uint8_t CONFIG = 0;
const uint8_t BUTTON = 1;
const uint8_t SWITCH = 2;
const uint8_t POT = 3;
const uint8_t ROTARY = 4;
const uint8_t SYNC = 254;       // reserved for additional messages
const uint8_t EXTENSION = 255;  // reserved for additional messages

void txButton(uint8_t id, uint8_t value) {
#ifndef DEBUG
  Serial.write(BUTTON);
  Serial.write(id);
  Serial.write(value);
  Serial.write(0xff);
  Serial.write(0xdd);  // checksum
#endif
  return NULL;
}

void txSwitch(uint8_t id, uint8_t value) {
#ifndef DEBUG
  Serial.write(SWITCH);
  Serial.write(id);
  Serial.write(value);
  Serial.write(0xff);
  Serial.write(0xdd);  // checksum
#endif
  return NULL;
}

void txPot(uint8_t id, uint16_t value) {
#ifndef DEBUG
  Serial.write(POT);
  Serial.write(id);
  Serial.write(value >> 8);
  Serial.write(value);
  Serial.write(0xdd);  // checksum
#endif
  return NULL;
}

void txRotary(uint8_t id, int8_t value) {
#ifndef DEBUG
  Serial.write(ROTARY);
  Serial.write(id);
  Serial.write(value);
  Serial.write(0xff);
  Serial.write(0xdd);  // checksum
#endif
  return NULL;
}

static unsigned long nextTime = 0;

void testSerial() {

  unsigned long now = millis();
  if (nextTime > now) {
    return;
  }
  nextTime = now + 1000 * 10;
  Serial.write('A');
  Serial.write('A');
  Serial.write('A');
  Serial.write('A');
  Serial.write('\n');
  txButton(1, 1);
  txButton(2, 0);
  txButton(3, 1);
  txSwitch(4, 1);
  txPot(5, 999);
  txRotary(5, 1);
  txRotary(5, -1);
}

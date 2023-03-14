// Marker
const uint8_t MARKER = 0xAA;
// message types
const uint8_t CONFIG = 0;
const uint8_t BUTTON = 1;
const uint8_t SWITCH = 2;
const uint8_t POT = 3;
const uint8_t ROTARY = 4;
const uint8_t PANEL = 5;
const uint8_t SYNC = 254;       // reserved for additional messages
const uint8_t EXTENSION = 255;  // reserved for additional messages

void txPanel(Stream* s, String name) {

#ifndef DEBUG
  s->write(MARKER);
  s->write(name.length() + 1);
  s->write(PANEL);

  for (int i = 0; i < name.length(); i++) {
    s->write(name[i]);
  }
#endif
  return NULL;
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

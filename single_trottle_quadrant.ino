#ifdef SINGLE_THROTTLE_QUADRANT_PANEL
button switchButtons[] = {
  // { 0}, // reserved
  // { 1}, // reserved
  // { 6}, // rotary
  // { 7}, // rotary
};

// https://docs.arduino.cc/learn/electronics/potentiometer-basics
button potButtons[] = {
  {  .pin = A0}, 
  //  { .pin = A1},
  //  { .pin = A2} 
};

rotary rotaryControls[] = {
  // { .aPin = 6, .bPin = 7, .buttonPin = 5}
};


const int nSwitchButtons = sizeof(switchButtons)/sizeof(button);
const int nPotButtons = sizeof(potButtons)/sizeof(button);
const int nRotaryControls = sizeof(rotaryControls)/sizeof(rotary);

const char* panelName = "single.throttle.quadrant";


#endif
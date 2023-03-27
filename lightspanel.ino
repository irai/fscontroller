#ifdef LIGHTS_PANEL
button switchButtons[] = {
  { "", 0},
  { "", 1},
  { "strobe light", 2},
  { "nav light", 3},
  { "taxi lights", 4},
  { "", 5},
  // { "beacon light", 6},
  // { "pitot heat", 7},
  { "", 8},
  { "", 9},
  { "", 10},
  { "", 11},
  { "", 12},
  { "", 13},
  { "", 14},
  { "", 15},
  { "", 16},
  { "", 17},
  { "", 18},
  { "", 19},
  { "", 20},
  { "", 21},
  { "", 22},
  { "", 23},
  { "", 24},
  { "", 25},
  { "", 26},
  { "", 27},
  { "", 28},
  { "", 29},
  { "", 30},
  { "", 31},
  { "", 32},
  { "", 33},
  { "", 34},
  { "", 35},
  { "", 36},
  { "", 37},
  { "", 38},
  { "", 39},
  { "", 40},
  { "", 41},
  { "", 42},
  { "", 43},
  { "", 44},
  { "", 45},
  { "", 46},
  { "", 47},
  { "", 48},
  { "", 49},
  { "", 50},
  { "", 51},
  { "", 52},
  { "", 53}
};

button pushButtons[] = {
  // { "atc", 12},
  // { "view", 13}
};

// https://docs.arduino.cc/learn/electronics/potentiometer-basics
button potButtons[] = {
  { "throtle", A3},
   { "mixture", A4},
   { "propeller", A5}
};

rotary rotaryControls[] = {
  { "trim", 6, 7, 5}
};

const int nSwitchButtons = sizeof(switchButtons)/sizeof(button);
const int npushButtons = sizeof(pushButtons)/sizeof(button);
const int nPotButtons = sizeof(potButtons)/sizeof(button);
const int nRotaryControls = sizeof(rotaryControls)/sizeof(rotary);

const String panelName = "LightsPanel";


#endif
#ifdef LIGHTS_PANEL
button switchButtons[] = {
  { 0},
  { 1},
  { 2},
  { 3},
  {  4},
  {  5},
  // { 6}, // rotary
  // { 7}, // rotary
  {  8},
  {  9},
  {  10},
  {  11},
  {  12},
  {  13},
  {  14},
  {  15},
  {  16},
  {  17},
  {  18},
  {  19},
  {  20},
  {  21},
  {  22},
  {  23},
  {  24},
  {  25},
  {  26},
  {  27},
  {  28},
  {  29},
  {  30},
  {  31},
  {  32},
  {  33},
  {  34},
  {  35},
  {  36},
  {  37},
  {  38},
  {  39},
  {  40},
  {  41},
  {  42},
  {  43},
  {  44},
  {  45},
  {  46},
  {  47},
  {  48},
  {  49},
  {  50},
  {  51},
  {  52},
  {  53}
};

// https://docs.arduino.cc/learn/electronics/potentiometer-basics
button potButtons[] = {
  { "throtle", A3}, // pin 57
  //  { "mixture", A4}, // pin 58
  //  { "propeller", A5} // pin 59
};

rotary rotaryControls[] = {
  { "trim", 6, 7, 5}
};

const int nSwitchButtons = sizeof(switchButtons)/sizeof(button);
const int nPotButtons = sizeof(potButtons)/sizeof(button);
const int nRotaryControls = sizeof(rotaryControls)/sizeof(rotary);

const String panelName = "LightsPanel";


#endif
#ifdef LIGHTS_PANEL
const int nSwitchButtons = 12;
button switchButtons[nSwitchButtons] = {
  { "strobe light", 0, { &strobeLightOnKeys, &strobeLightOffKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "nav light", 1, { &navLightOnKeys, &navLightOffKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "taxi lights", 2, { &taxiLightOnKeys, &taxiLightOffKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "landing lights", 3, { &landingLightOnKeys, &landingLightOffKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "beacon light", 4, { &beaconLightOnKeys, &beaconLightOffKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "pitot heat", 5, { &pitotHeatOnKeys, &pitotHeatOffKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "fuel pump", 6, { &fuelPumpOnKeys, &fuelPumpOffKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "free", 7, { NULL, NULL, NULL, NULL }, 0, 0, 0, 0 },
  { "master", 8, { &masterKeys, &masterKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "avionics", 9, { &avionics, &avionics, NULL, NULL }, 0, 0 },
  { "gear", 10, { &gearDownKeys, &gearUpKeys, NULL, NULL }, 0, 0, 0, 0 },
  { "carby heat", 11, { &carbHeatOnKeys, &carbHeatOffKeys, NULL, NULL }, 0, 0, 0, 0 }
};

const int nPressureButtons = 2;
button pressureButtons[nPressureButtons] = {
  { "atc", 12, { &atcKeys, NULL, NULL, NULL }, 0, 0, 0, 0 },
  { "view", 13, { &viewDashboardFirstKeys, &viewDashboardNextKeys, &viewDashboardNextKeys, NULL }, 0, 0, 0, 3 }
};

// https://docs.arduino.cc/learn/electronics/potentiometer-basics
const int nPotButtons = 3;
button potButtons[nPotButtons] = {
  { "throtle", A1, { &throtleMaxKeys, &throtleCutKeys, &throtleIncreaseKeys, &throtleDecreaseKeys }, 0, 0, 1024 / 100, 1024 },   // in 100 steps - Cesna 172
  { "mixture", A2, { &mixtureRichKeys, &mixtureLeanKeys, &mixtureIncreaseKeys, &mixtureDecreaseKeys }, 0, 0, 1024 / 65, 1024 },  // in 64 stepsC - Cesna 172
  { "propeller", A3, { &propellerHiKeys, &propellerLowKeys, &propellerIncreaseKeys, &propellerDecreaseKeys }, 0, 0, 64, 1024 }
};

const int nRotaryControls = 1;
rotary rotaryControls[nRotaryControls] = {
  { "trim", NULL, A5, &trimDown, A4, &trimUp, 0, NULL, 0, 0, 0 },
};

// HardwareSerial piHandler = Serial;
// Serial_ xboxHandler = Serial;

#endif
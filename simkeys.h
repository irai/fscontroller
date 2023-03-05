#ifndef SIMKEYS_H
#define SIMKEYS_H

// Define missing keys for Arduino. These are defined for Teensy.
#ifndef KEYPAD_MINUS
#define nonprinting 136  // it's a non-printing key (not a modifier)
#define KEY_F11 68 + nonprinting
#define KEY_F12 69 + nonprinting
#define KEY_PRINTSCREEN 70 + nonprinting
#define KEY_SCROLL_LOCK 71 + nonprinting
#define KEY_PAUSE 72 + nonprinting
#define KEY_INSERT 73 + nonprinting
#define KEY_HOME 74 + nonprinting
#define KEY_PAGE_UP 75 + nonprinting
#define KEY_DELETE 76 + nonprinting
#define KEY_END 77 + nonprinting
#define KEY_PAGE_DOWN 78 + nonprinting
#define KEY_RIGHT 79 + nonprinting
#define KEY_LEFT 80 + nonprinting
#define KEY_DOWN 81 + nonprinting
#define KEY_UP 82 + nonprinting
#define KEY_NUM_LOCK 83 + nonprinting
#define KEYPAD_SLASH 84 + nonprinting
#define KEYPAD_ASTERIX 85 + nonprinting
#define KEYPAD_MINUS 86 + nonprinting
#define KEYPAD_PLUS 87 + nonprinting
#define KEYPAD_ENTER 88 + nonprinting
#define KEYPAD_1 89 + nonprinting
#define KEYPAD_2 90 + nonprinting
#define KEYPAD_3 91 + nonprinting
#define KEYPAD_4 92 + nonprinting
#define KEYPAD_5 93 + nonprinting
#define KEYPAD_6 94 + nonprinting
#define KEYPAD_7 95 + nonprinting
#define KEYPAD_8 96 + nonprinting
#define KEYPAD_9 97 + nonprinting
#define KEYPAD_0 98 + nonprinting
#define KEYPAD_PERIOD 99 + nonprinting
#endif


cmd masterAlternatorKeys = { 2, { KEY_LEFT_ALT, 'a' } };
cmd masterAlternatorOnKeys = { 2, { KEY_RIGHT_CTRL, 'a' } }; // custom key
cmd masterAlternatorOffKeys = { 2, { KEY_RIGHT_ALT, 'a' } }; // custom key
cmd masterBatteryKeys = { 2, { KEY_LEFT_ALT, 'b' } };
cmd masterBatteryOnKeys = { 2, { KEY_RIGHT_CTRL, 'b' } }; // custom Key
cmd masterBatteryOffKeys = { 2, { KEY_RIGHT_ALT, 'b' } }; // custom Key
cmd masterKeys = { 1, { 'M' } };
cmd masterOnKeys = { 2, { KEY_RIGHT_CTRL, 'M' } }; // custom Key
cmd masterOffKeys = { 2, { KEY_RIGHT_ALT, 'M' } }; // custom Key
cmd avionics = { 1, { KEY_PAGE_UP } };
cmd avionicsOnKeys = { 2, { KEY_RIGHT_CTRL, KEY_PAGE_UP } }; // custom Key
cmd avionicsOffKeys = { 2, { KEY_RIGHT_ALT, KEY_PAGE_UP } }; // custom Key
cmd atcKeys = { 2, { KEY_SCROLL_LOCK, KEY_HOME } };
cmd gearDownKeys = { 2, { KEY_LEFT_CTRL, 'g' } };
cmd gearUpKeys = { 2, { KEY_RIGHT_ALT, 'g' } };
cmd carbHeatKeys = { 1, { 'h' } };
cmd carbHeatOnKeys = { 2, { KEY_RIGHT_CTRL, 'i' } }; // anti ice in cesna 152
cmd carbHeatOffKeys = { 2, { KEY_RIGHT_ALT, 'i' } };  // anti ice in cesna 152
cmd pitotHeatKeys = { 1, { 'H' } };
cmd pitotHeatOnKeys = { 2, { KEY_RIGHT_CTRL, 'h' } };
cmd pitotHeatOffKeys = { 2, { KEY_RIGHT_ALT,'h' } };
// keys mixtureIncreaseDefaultKeys = { 3, { KEY_LEFT_SHIFT, KEY_LEFT_CTRL, KEY_F3 } };
cmd mixtureIncreaseKeys = { 2, { KEY_RIGHT_CTRL, KEY_F3 } }; // custom key
// keys mixtureDecreaseDefaultKeys = { 3, { KEY_LEFT_SHIFT, KEY_LEFT_CTRL, KEY_F2 } };
cmd mixtureDecreaseKeys = { 2, { KEY_RIGHT_ALT, KEY_F3 } }; // custom key
cmd mixtureLeanKeys = { 3, { KEY_LEFT_SHIFT, KEY_LEFT_CTRL, KEY_F1 } };
cmd mixtureRichKeys = { 3, { KEY_LEFT_SHIFT, KEY_LEFT_CTRL, KEY_F4 } };
cmd fuelPumpKeys = { 2, { KEY_LEFT_ALT, 'p' } };
cmd fuelPumpOnKeys = { 2, { KEY_RIGHT_CTRL, 'p' } }; // custom key
cmd fuelPumpOffKeys = { 2, { KEY_RIGHT_ALT, 'p' } }; // custom key
cmd fuelValveKeys = { 2, { KEY_LEFT_ALT, 'v' } };
cmd fuelTankOff = { 2, { KEY_LEFT_ALT, 'v' } };
cmd fuelTankLeft = { 2, { KEY_RIGHT_CTRL, 'o' } }; // custom key
cmd fuelTankRight = { 2, { KEY_RIGHT_ALT, 'o' } }; // custom key
cmd taxiLightToggleh = { 2, { KEY_LEFT_ALT, 'j' } };
cmd taxiLightOnKeys = { 2, { KEY_RIGHT_CTRL, 't' } }; // custom key
cmd taxiLightOffKeys = { 2, { KEY_RIGHT_ALT, 't' } }; // custom key
cmd navLightToggleKeys = { 2, { KEY_LEFT_ALT, 'n' } };
cmd navLightOnKeys = { 2, { KEY_RIGHT_CTRL, 'n' } }; // custom key
cmd navLightOffKeys = { 2, { KEY_RIGHT_ALT, 'n' } }; // custom key
cmd strobeLightToggleKeys = { 1, { 'o' } };
cmd strobeLightOnKeys = { 2, { KEY_RIGHT_CTRL, 's' } }; // custom key
cmd strobeLightOffKeys = { 2, { KEY_RIGHT_ALT, 's' } }; // custom key
cmd beaconLightToggleKeys = { 2, { KEY_LEFT_ALT, 'h' } };
cmd beaconLightOnKeys = { 2, { KEY_RIGHT_CTRL, 'b' } }; // custom key
cmd beaconLightOffKeys = { 2, { KEY_RIGHT_ALT, 'b' } }; // custom key
cmd landingLightToggleKeys = { 2, { KEY_LEFT_CTRL, 'l' } };
cmd landingLightOnKeys = { 2, { KEY_RIGHT_CTRL, 'l' } }; // custom key
cmd landingLightOffKeys = { 2, { KEY_RIGHT_ALT, 'l' } }; // custom key
cmd viewDashboardFirstKeys = { 2, { KEY_LEFT_CTRL, '1' } };
cmd throtleIncreaseKeys = { 1, { KEY_F3 } };
cmd throtleDecreaseKeys = { 1, { KEY_F2 } };
cmd throtleMaxKeys = { 2, { KEY_LEFT_SHIFT, '0' } };  // no max key available - set a custom key in FS - Throtle 1 Full
cmd throtleCutKeys = { 1, { KEY_F1 } };
cmd viewDashboardNextKeys = { 1, { 'a' } };
cmd trimDown = { 1, { KEYPAD_7 } };
cmd trimUp = { 1, { KEYPAD_1 } };
cmd propellerHiKeys = { 2, { KEY_LEFT_CTRL, KEY_F1 } };
cmd propellerLowKeys = { 2, { KEY_LEFT_CTRL, KEY_F4 } };
cmd propellerIncreaseKeys = { 2, { KEY_LEFT_CTRL, KEY_F3 } };
cmd propellerDecreaseKeys = { 2, { KEY_LEFT_CTRL, KEY_F2 } };
cmd parkingBreakKeys = { 2, { KEY_LEFT_CTRL, KEYPAD_PERIOD } };
cmd magnetoStartKeys = { 2, { KEY_LEFT_ALT, 'G' } };
cmd magnetoLeftKeys = { 2, {  KEY_LEFT_ALT, 'S' } };
cmd magnetoRightKeys = { 2, {  KEY_LEFT_ALT, 'D' } };
cmd magnetoBothKeys = { 2, {  KEY_LEFT_ALT, 'F' } };
cmd magnetoOffKeys = { 2, {  KEY_LEFT_ALT, 'Q' } };
cmd flapsRetractKeys = { 1, { KEY_F5 } };
cmd flaps10Keys = { 2, { KEY_LEFT_ALT, 'f' } };  // not available - set custom key in FS - Flaps 1
cmd flaps20Keys = { 2, { KEY_LEFT_ALT, 'g' } };  // not available - set custom key in FS - Flaps 2
cmd flapsFullKeys = { 1, { KEY_F8 } };
cmd com1StbFreqFocusKeys = { 1, { 'c'} }; // pre condition to set focus
cmd com1StbFreqUpKeys = { 2, { KEY_LEFT_CTRL, KEYPAD_PLUS } };
cmd com1StbFreqDownKeys = { 2, { KEY_LEFT_CTRL, KEYPAD_MINUS } };
cmd com1StbFreqDecFocusKeys = { 2, { 'c', 'c'} }; // pre condition to set focus
cmd com1StbFreqDecUpKeys = { 2, { KEY_LEFT_CTRL, KEYPAD_PLUS } };
cmd com1StbFreqDecDownKeys = { 2, { KEY_LEFT_CTRL, KEYPAD_MINUS } };
cmd com1StbSwapKeys = { 2, { KEY_LEFT_ALT, 'u' } };
cmd increaseAltKeys = { 2, { KEY_LEFT_CTRL, KEY_PAGE_UP } };
cmd decreaseAltKeys = { 2, { KEY_LEFT_CTRL, KEY_PAGE_DOWN } };
cmd incHeadingBugKeys = { 2, { KEY_LEFT_CTRL, KEY_INSERT } };
cmd decHeadingBugKeys = { 2, { KEY_LEFT_CTRL , KEY_DELETE} };
cmd setHeadingBugKeys = { 2, { KEY_LEFT_CTRL, 'H'} };

extern void focusFrequency(rotary *);
extern void focusFrequencyDec(rotary *);

#endif
// Ruud Boer
// June 2019 
// Joystick with Arduino Leonardo
// Rotary Encoder generates pulses which can be used for Flightsim Elevator Trim

// PIN SWITCH  FUNCTION
// 02  Tog DN  JOY 01
// 03  Tog UP  JOY 02
// 04  PB 01   JOY 03
// 05  PB 04   JOY 04
// 06  PB 05   JOY 05
// 07  STICKSW JOY 06
// 08  PB 07   JOY 07
// 09  PB 08   JOY 08
// 10  PB 09   JOY 09
// 11  PB 10   JOY 10  Black button
// 12  ROT SW  JOY 11
// 13  LED
// 14  PB 03   JOY 12  ICSP MISO https://forum.arduino.cc/index.php?topic=241369.0
// 15  PB 02   JOY 13  ICSP SCK  https://forum.arduino.cc/index.php?topic=241369.0
// A0  STICK-X JOY X
// A1  STICK-Y JOY Y
// A2  STICK-X JOY Z
// A3  POTM    JOY Throttle

#define NUM_SWITCHES      13 // the number of switches
#define DEBOUNCE_TIME     10 // ms delay before the push button changes state
#define NUM_ANALOG         4 // the number of analog inputs
#define ENC_CLK_PIN       A4 // rotary encoder CLK input
#define ENC_DATA_PIN      A5 // rotary encoder DATA input
#define ENC_ZERO           0 // value to jump to after pressing encoder switch
#define ROTARY_SLOW      200
#define ROTARY_FAST       10
#define INCREMENT_SLOW     1
#define INCREMENT_FAST     1
#define PULSE_DURATION   100 // [ms] Elevator Trim 

byte switch_pin[NUM_SWITCHES] = {2,3,4,5,6,7,8,9,10,11,12,14,15}; // digital input pins
byte analog_pin[NUM_ANALOG]   = {A0,A1,A2,A3}; // analog input pins X,Y,Z,THROT

#include "Joystick.h"
Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_JOYSTICK,
     15, // +2 for the rotary encoder elevator trim
      0,
   true,  // includeXAxis       1
   true,  // includeYAxis       2
   true,  // includeZAxis       3
  false,  // includeRxAxis      4
  false,  // includeRyAxis      5
  false,  // includeRzAxis      6
  false,  // includeRudder      7
   true,  // includeThrottle    8
  false,  // includeAccelerator
  false,  // includeBrake
  false); // includeSteering

byte enc_clk, enc_clk_old, increment;
byte reading[NUM_SWITCHES], switch_state[NUM_SWITCHES + 2], switch_state_old[NUM_SWITCHES + 2];
int analog_value[NUM_ANALOG];
unsigned long debounce_time[NUM_SWITCHES];
unsigned long enc_rotated_time, time_to_reset_pulse;

void read_switches() {
  for (byte i=0; i<NUM_SWITCHES; i++) {
    reading[i] = !digitalRead(switch_pin[i]);
    if (reading[i] == switch_state[i]) debounce_time[i] = millis() + (unsigned long)DEBOUNCE_TIME;
    else if (millis() > debounce_time[i]) switch_state[i] = reading[i];
  }
}

int read_rotary() {
  byte rotary;
  enc_clk = digitalRead(ENC_CLK_PIN);
  if((enc_clk == 1) && (enc_clk_old == 0)) { // 0->1 transition
    if((millis() - enc_rotated_time) > ROTARY_SLOW) rotary = INCREMENT_SLOW;  
    else if((millis() - enc_rotated_time) > ROTARY_FAST) rotary = INCREMENT_FAST;  
    else rotary = 0;
    enc_rotated_time = millis();
    if(digitalRead(ENC_DATA_PIN) == 0) rotary = -rotary;
  }
  enc_clk_old = enc_clk;
  return rotary;
}

void setup() {
	for (byte i=0; i<NUM_SWITCHES; i++) pinMode(switch_pin[i], INPUT_PULLUP);
	pinMode(ENC_CLK_PIN,  INPUT_PULLUP);
	pinMode(ENC_DATA_PIN, INPUT_PULLUP);
	pinMode(13, OUTPUT);
	digitalWrite(13,0);
	Joystick.begin(false); 
	Joystick.setXAxisRange   (-511, 511);
	Joystick.setYAxisRange   (-511, 511);
	Joystick.setZAxisRange   (-511, 511);
	Joystick.setThrottleRange(   0,1023);
}

void loop() {

  read_switches();

	for (byte i=0; i<NUM_SWITCHES + 2; i++) { // +2 for rotary encoder elevator trim pulses
		if (switch_state[i] != switch_state_old[i]) { // debounced button has changed state
			// this code is executed once after change of state
			digitalWrite(13,switch_state[i]);
			if (switch_state[i]) Joystick.pressButton(i); else Joystick.releaseButton(i);
			switch_state_old[i] = switch_state[i]; // store new state such that the above gets done only once
		}
	}

	for (byte i=0; i<NUM_ANALOG; i++) { // read analog inputs
		analog_value[i] = analogRead(analog_pin[i]);
//    Create curve: value/2 in the middle, value*1.5 at the extremes 
//		if (analog_value[i] < 256) analog_value[i] = analog_value[i] * 1.5;
//		else if (analog_value[i] < 768) analog_value[i] = 256 + analog_value[i] / 2;
//		else analog_value[i] = 640 + (analog_value[i] - 768) * 1.5;
		switch(i) {
			case 0:
				Joystick.setXAxis(511 - analog_value[0]);
			break;
			case 1:
				Joystick.setYAxis(511 - analog_value[1]);
			break;
			case 2:
				Joystick.setZAxis(511 - analog_value[2]);
			break;
			case 3:
				Joystick.setThrottle(analog_value[3]);
			break;
		}
	}

	increment = read_rotary();
  
  if(increment == 1) {
    switch_state[NUM_SWITCHES] = 1;
    time_to_reset_pulse = millis() + (unsigned long)PULSE_DURATION;
    increment = 0;
  }
  if(increment == 255) {
    switch_state[NUM_SWITCHES + 1] = 1;
    time_to_reset_pulse = millis() + (unsigned long)PULSE_DURATION;
    increment = 0;
  }
  if(millis() > time_to_reset_pulse) {
    switch_state[NUM_SWITCHES]     = 0;
    switch_state[NUM_SWITCHES + 1] = 0;
  }

	Joystick.sendState();
}
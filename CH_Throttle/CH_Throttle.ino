// Program used to test the USB Joystick object on the 
// Arduino Leonardo or Arduino Micro.
//
// Matthew Heironimus
// 2015-03-28 - Original Version
// 2015-11-18 - Updated to use the new Joystick library 
//              written for Arduino IDE Version 1.6.6 and
//              above.
// 2016-05-13   Updated to use new dynamic Joystick library
//              that can be customized.
// Andrea Batch 
// 2021-02-26 - Update for old CH throttle 
//              based on Ruud Boer instructable
// 2021-03-01 - Scrap original version and use Ruud Boer 2018 
//              as basis for throttle, and then add PWM pins.
//  Things to note:
//    1. The potentiometer only had 2 of its tabs connected when I cracked it open.
//      I suspect this may have to change, but I am a little wary of doing so.
//    2. Need to check and make sure potentiometer actually works. Use LED.
//    3. Do not forget that the LED leg length matters, lol. Long leg goes to num pin.
//    4. Pot: One pot tab goes to board 5v, another tab goes to GND, 
//        last one (usually middle) goes to, in this case, A0. 
//        Remember to put a resistor between the pins, but I do not remember which ones lols.
//    5. LED goes to digital 13 in the test script. Have not picked which script to use yet tho.
//    6. Potentiometers do not need resistors.
//------------------------------------------------------------

// CONFIG
#define MAX_SWITCHES 12 // the number of BUTTONS (three on front of throttle, three on thumb, four for d-pad)
byte switch_pin[MAX_SWITCHES] = {2,3,4,5,6,7,8,9,10,11,12,13}; // digital input pins -- we will not be using 13 for light
#define DEBOUNCE_TIME 5 // ms delay before the push button changes state
#define RUD_DEBOUNCE_TIME 50 // ms delay before the held rudder toggle changes state; I choose 0.05 seconds bc change val is low.
#define MAX_ANALOG 1 // the number of analog inputs
byte analog_pin[MAX_ANALOG] = {A0}; // analog input pins THROT---for our CH throttle, use 2 analogs as digital for toggle to control sRUDDER!
#define RUD_UPWD_PIN A1 // rotary encoder CLK input -- toggle on front of CH throttle DOWN
#define RUD_DNWD_PIN A2 // rotary encoder DATA input -- toggle on front of CH throttle UP
#define RUD_RESET_LOW 0 // first button pin for ENC reset to min or zero (throttle front LEFT)
#define RUD_RESET_UPR 1 // first button pin for ENC reset to max, second for zero (throttle front CENTER)
#define RUD_RESET_CTL 2 // second button pin for ENC reset to min or max, third for zero (throttle front RIGHT) 
#define RUD_MAX 127 // max value of the rotary encoder
#define RUD_MIN -127 // min value of the rotary encoder
#define RUD_ZERO 0 // value to jump to after pressing encoder switch
// END CONFIG

// DECLARATIONS
#include "Joystick.h"
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_JOYSTICK, MAX_SWITCHES, 0, true, true, true, false, false, false, true, true, false, false, false);
byte reading, rud_reading, rud_dnwd_clk, rud_dnwd_clk_old, rud_upwd_clk, rud_upwd_clk_old;
int analog_value[MAX_ANALOG+1]; // +1 for the rotary encoder value
unsigned long debounce_time[MAX_SWITCHES+1]; // +1 for CLK of rotary encoder
byte switch_state[MAX_SWITCHES];
byte switch_state_old[MAX_SWITCHES];
// END DECLARATIONS

// FUNCTIONS
int read_rotary_encoder() {
  //Unlike Boer, we are using a controller without a clickable analog knob; all we have is the digital toggle on the front.
  //However, we can do something a little different with it: Holding it down sets rudder values lower, holding it up sets them higher.
  //Different from other buttons!
  if (millis() > debounce_time[MAX_SWITCHES]) { // check if enough time has passed
    debounce_time[MAX_SWITCHES] = millis() + (unsigned long)RUD_DEBOUNCE_TIME; //reset counter
    if (!digitalRead(RUD_UPWD_PIN)) return 1; //add to rudder if upward pin read
    if (!digitalRead(RUD_DNWD_PIN)) return -1; //subtract from rudder if downward pin read
  }
  return 0;
}
// END FUNCTIONS

// SETUP
void setup() {
  for (byte i=0; i<MAX_SWITCHES; i++) pinMode(switch_pin[i],INPUT_PULLUP);
  //pinMode(13,OUTPUT); // on board LED -- no longer used. Consider using 3 analogs for LEDs?
  //digitalWrite(13,0);
  pinMode(RUD_UPWD_PIN,INPUT_PULLUP);
  pinMode(RUD_DNWD_PIN,INPUT_PULLUP);

  Joystick.begin(false); 
  Joystick.setRudderRange(RUD_MIN, RUD_MAX);
  Joystick.setThrottleRange(0, 1023);
} // END SETUP

// LOOP
void loop() {
  for (byte i=0; i<MAX_SWITCHES; i++) { // read the switches
    reading = !digitalRead(switch_pin[i]);
    if (reading == switch_state[i]) debounce_time[i] = millis() + (unsigned long)DEBOUNCE_TIME;
    else if (millis() > debounce_time[i]) switch_state[i] = reading;
    if (switch_state[i] != switch_state_old[i]) { // debounced button has changed state
      // this code is executed once after change of state--if we decide to use analog pins for LEDs, change this to another loop for all LEDs
      //digitalWrite(13,switch_state[i]);
      if (switch_state[i]) Joystick.pressButton(i); else Joystick.releaseButton(i);
      //if (i==10) analog_value[4] = RUD_ZERO; //lets not do that here; we only want to do this if all front buttons pressed
      switch_state_old[i] = switch_state[i]; // store new state such that the above gets done only once
    }
  } //END read the switches

    analog_value[MAX_ANALOG-1] = analogRead(A0);
    if (analog_value[MAX_ANALOG-1] < 256) analog_value[MAX_ANALOG-1] = analog_value[MAX_ANALOG-1] * 1.5;
    else if (analog_value[MAX_ANALOG-1] < 768) analog_value[MAX_ANALOG-1] = 256 + analog_value[MAX_ANALOG-1] / 2;
    else analog_value[MAX_ANALOG-1] = 640 + (analog_value[MAX_ANALOG-1] - 768) * 1.5;
    Joystick.setThrottle(analog_value[MAX_ANALOG-1]);
    
  // use 2 to 3 switches simultaneously (left, center, and right throttle-front buttons) to quickly calibrate the rotary encoder 
  // (THIS SETS ROTARY ENCODER TO MIN, MAX, OR ZERO)
  if (!digitalRead(switch_pin[RUD_RESET_LOW]) && !digitalRead(switch_pin[RUD_RESET_UPR]) && !digitalRead(switch_pin[RUD_RESET_CTL])) analog_value[MAX_ANALOG] = RUD_ZERO;
  else if (!digitalRead(switch_pin[RUD_RESET_LOW]) && !digitalRead(switch_pin[RUD_RESET_CTL])) analog_value[MAX_ANALOG] = RUD_MIN;
  else if (!digitalRead(switch_pin[RUD_RESET_UPR]) && !digitalRead(switch_pin[RUD_RESET_CTL])) analog_value[MAX_ANALOG] = RUD_MAX;
  analog_value[MAX_ANALOG] = analog_value[MAX_ANALOG] + read_rotary_encoder();
  
  
  Joystick.setRudder(analog_value[MAX_ANALOG]);
  Joystick.setThrottle(analog_value[MAX_ANALOG-1]);
  
  Joystick.sendState();
  delay(10);
} // END LOOP

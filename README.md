# frankenthrottle
Resuscitating my 1990s 15-pin CH throttle by tearing its brain out and replacing it with an Arduino Leonardo.

## Instructions
1. Cut the wires close to the board in the throttle. You will feel sad about this if it does, in theory, still work on ancient PCs.
2. Follow Heironimus' library installation instructions [here](https://github.com/MHeironimus/ArduinoJoystickLibrary). I also added a fork of his repo as a subdir here for reference.
3. Flash the Leonardo with one of the test scripts here to make sure that your throttle's potentiometer still works. You will need to wire it up to a breadboard appropriately; see the [Arduino docs](https://www.arduino.cc/en/tutorial/potentiometer) for reference. **Important: Make sure you flash it to the right COM for this step and the next. I don't want to be responsible for bricking someone's expensive hardware!**
4. Flash your Leonardo with the CH_Throttle script in this repo (**note: this is not yet done and you would currently be better off using one of Ruud Boer's scripts, a fork of which I have also added here for reference, and modifying it for the CH**).
5. Connect the wires per the schematic that I am definitely going to add and not forget once I finish bringing my throttle back from the dead.

# Wires
**Front (away from user) buttons**
1. Green: GROUND
2. Red (Stripe): LEFT Pinky Button (from down-throttle POV)
3. Orange (Stripe): CENTER Ring Button  
4. White (Stripe): RIGHT Bird Button (from down-throttle POV)

**Front (away from user) toggle**
5. Black: Index Toggle UP (bareward)
6. Yellow: Index Toggle DOWN (wireward)

**``Thumbstick'' padcone**
They did something clever with the thumb``stick;'' it looks like we have white, blue, brown, and grey being used for the thumbstick, and there is probably some kind of level combo they used to derive diagonals.
1. White: UP
2. Blue: NEAR (toward user)
3. Purple: FAR direction (away from user)
4. Grey: DOWN

**Thumb buttons** 
1. Brown: TOP button
2. Red: NEAR button
3. Orange (Solid): BOTTOM buttom

**Potentiometer/Throttle**
1. Yellow: 5v power in
2. Blue: Analog pin out
3. Unassigned: There is a third, unused tab for ground; I added the ground. You will probably need to as well.

## Reference material
Shamelessly borrowed most of this from:
* [Matthew Heironimus](https://www.instructables.com/Arduino-LeonardoMicro-as-Game-ControllerJoystick/)
* [Ruud Boer](https://rudysmodelrailway.wordpress.com/2018/07/09/a-super-smooth-joystick-and-throttle-part-3-wiring-and-software-with-download/)

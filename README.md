# frankenthrottle
Resuscitating my 1990s 15-pin CH throttle by tearing its brain out and replacing it with an Arduino Leonardo.

## Instructions
1. Cut the wires close to the board in the throttle. You will feel sad about this if it does, in theory, still work on ancient PCs.
2. Follow Heironimus' library installation instructions [here](https://github.com/MHeironimus/ArduinoJoystickLibrary). I also added a fork of his repo as a subdir here for reference.
3. Use one of the test scripts here to make sure that your throttle's potentiometer still works. You will need to wire it up to a breadboard appropriately; see the [Arduino docs](https://www.arduino.cc/en/tutorial/potentiometer) for reference.
4. Flash your Leonardo with the CH_Throttle script in this repo (**note: this is not yet done and you would currently be better off using one of Ruud Boer's scripts and modifying it for the CH**).
5. Connect the wires per the schematic that I am definitely going to add and not forget once I finish bringing my throttle back from the dead.

## Reference material
Shamelessly borrowed most of this from:
* [Matthew Heironimus](https://www.instructables.com/Arduino-LeonardoMicro-as-Game-ControllerJoystick/)
* [Ruud Boer](https://rudysmodelrailway.wordpress.com/2018/07/09/a-super-smooth-joystick-and-throttle-part-3-wiring-and-software-with-download/)

# Lights
## Note
The power supply for the light is... missing somewhere. Because v1.3 is powered with a USB dongle, please take care not to set maximum brightness. The Arduino will die of overcurrent and stuff in an errornous state, which is tricky to fix (you have to quickly upload new code before the damn thing freeze again)

## Changelog
### 1.3.1 - Hotfix
**Changes**:
* The old board is burnt (after plugging 11 V). New board is used (Arduino Uno R4 WiFi)
* The input reader broke. For some reason, reading it into serial first make it works 

### 1.3 - New functional version
**Usage**:
* Plug it in your computer, and turn on ArduinoIDE
* When the system is plugged in, the lights will turn green and delay for a second. This confirm the board is alive and well
* The system will be changing between blue and red when in idle
* To armed for battle, hold the joystick to the left (that is, against the arrow / body) until the lights turns green
* The system is now armed
  * To cancel, push the joystick to the left
  * To begin, push it to the right
* When the battle start, the light will turn red, indicate it has entered the battle state
* During battle, push left to go back to idle, push right to go to victory
* The light will slowly run out until when 10 seconds left, where it will flash. This is normal
* When the time run out, it will go to victory state where the light flash in many colors
* To go back to idle when in victory state, simply push left

**Changes**:
* Implement a timer class, moving the timing aspect out of main(),
* Created 3 new functions for changing lights,
* Fixed to the correct number of lights,
* Reimplement the decreasing light with time,
* Introduce serial output (USB) to be monitor with Arduino IDE,
* Implement a function to read joystick input, bringing it out of the code (could be improve, since the code right now rely on delay in main() to avoid accidental cancel. Making a delay that works right in the function should helps much better),
* Documentation a lot. Lots of comments to explain things now
* If still using Serial for logging purpose, please make it complete. I did print out some part, and some... not

**Known issues / Improvements**:
* The arduino sometimes escape from victory state back into idle state. This might have caused by the poor thing dying of lights drawing too much current. Need to fix this at some point
* ***Really need to find the power supply. The USB can only handle so much power***
* Screens, since now the timer is missing. Perhaps developing another set of functions to control it. Please do not cram everything into void loop(), for the love of whoever is gonna read and expand your code in the future
* Fix the joystick reading function to make it better. It needs delay inside loop() right now to prevent false reading, where if you hold it too long, it will cause the system to arm, then cancel (because it read too fast). I tried to implement the delay inside the function, but it doesn't work as intended
* Include more lighting effects, perhaps objectifying the light controlling stuffs into a single class that holds a pointer to the created array
* Overall clean up of the code


### 1.2 - broken
It works at some point, but an update to FastLED library broke it. No longer works as of 05/30/2025
Changes:
* Reprogrammed to match FSM model (using int state instead, allow for switching between state)
* Fix the timing issue (using millis() instead of using loops to keep time)
* Allow for ending match on demand

### 1.1 - Legacy
The original code from 2024. Is broken in the sense that it doesn't really keep track of time properly

## Documentation
The original documentation: Arena/Lightings/Light Proposal.docx

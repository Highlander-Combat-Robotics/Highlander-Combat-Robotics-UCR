#include <Arduino.h>
#include <FastLED.h>
const int NUM_LEDS = 110;
const int LED_PIN = 7;
const int MAX_BRIGHTNESS = 150;  // To avoid overcurrent the Arduino. 180 seems to be the best for usb

CRGB leds[NUM_LEDS];
int state;  // 0 idle, 1 arm, 2 ready, 3 battle, 4 epic
const unsigned long battleTime = 240000;  // 4 minutes = 240 s. Change this value (in milliseconds) to change arena time

// Timer object to do timing
class Timer {
  unsigned long refTime;  // Keep track of start time
  unsigned long timeDuration;  // Keep how long does the user want to time

  public:
    Timer(): refTime(millis()), timeDuration(0) {}  // Default constructor
    void start();
    int currTime() const;
    void setDuration(const unsigned long);
    int timeLeft() const;
};

// Set the light to the color of input CRGB object
void setLight(const CRGB& desireColor) {
  CRGB* curr = leds;
  CRGB* end = leds + NUM_LEDS - 1;
  // Check if the light has already been set to the desire color
  // Avoid expensive loops
  if (leds[0] == desireColor) return;

  while (curr != end) {
    *curr = desireColor;
    curr++;
  }
  
  FastLED.setBrightness(MAX_BRIGHTNESS);
  FastLED.show();
}

// Set the light to the color of input CRGB object
void setLight(const CRGB& desireColor, const int numOfLights) {
  CRGB* curr;
  CRGB* end = leds + NUM_LEDS - 1;  // Point to the end of the array

  CRGB* offArrayMin = end - (NUM_LEDS - numOfLights);

  // Set the lights in the off array off
  curr = end;
  while (curr != offArrayMin) {
    *curr = CRGB (0, 0, 0);
    curr--;
  }

  FastLED.show();

  if (numOfLights <= 0) return;  // for safety

  // Check if the light has already been set to the desire color
  // Avoid expensive loops
  if (leds[0] == desireColor) return;

  // Set the curr at the beginning
  curr = leds;

  // Iterate from beginning up
  while (curr != end) {
    *curr = desireColor;
    curr++;
  }
  
  FastLED.setBrightness(MAX_BRIGHTNESS);
  FastLED.show();
}

// Chasing light effect
void chaseLight(const CRGB& desireColor) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = desireColor;
    FastLED.show();
  }
  FastLED.setBrightness(MAX_BRIGHTNESS);
}

// Read analog input and return digital out from -1 or 1 (based on joystick)
int readJoystickInput() {
  unsigned long start = millis();
  unsigned long delayTime = 10;
  if (analogRead(A0) < 400) {
    delay(delayTime);
    if (millis() - start > delayTime) return -1;
  }
  else if (analogRead(A0) > 800) {
    delay(delayTime);
    if (millis() - start > delayTime) return 1;
  }
  // If not return before, then default output
  return 0;
}

void setup() {
  // For output to computer to keep track of time (if needed)
  Serial.begin(19200);
  Serial.println("Arduino started");

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  setLight(CRGB(0,255,0));
  state = 0;
  delay(1000);  // Delay 1 second to confirm set up finished
}

void loop() {
  // Input: 
  // - Left (< 400): arm the arena / cancel battle
  // - Right (> 800): start the arena
  // - Middle: do nothing / idle

  // IDLING
  if (state == 0) {
    Serial.println("idling");
    delay(100);
    while (state == 0) {
      // Read input and pump to next state if joystick left is detected
      if (readJoystickInput() == -1) state = 1;

      // Otherwise, change light between each seconds
      if ((millis()/1000)%2==0) {
        chaseLight(CRGB (255,0,0));
        if (readJoystickInput() == -1) state = 1;
        continue;
      }

      if ((millis()/1000)%2 == 1) {
        chaseLight(CRGB (0,0,255));
        if (readJoystickInput() == -1) state = 1;
        continue;
      }
    }
  }


  // ARM
  if (state == 1) {
    // Set light to green, confirming board hasn't died
    setLight(CRGB (0,255,0));
    state = 2;
    delay(1000);
    Serial.println("ready to start");
  }

  // READY TO START
  while (state == 2) {
    // Wait for input
    // Left: cancel
    // Right: start battle
    if (readJoystickInput() == -1) state = 0;
    else if (readJoystickInput() == 1) state = 3;
  }
  
  // IN BATTLE
  if (state == 3) {
    // Create timer object and initialize it with desire battle tine
    Timer timer;
    timer.setDuration(battleTime);
    timer.start();

    // Set light to 
    setLight( CRGB (255, 0, 0) );
    delay(500);

    // Enter the loop, change light to indicate remaining time
    // Constantly checking the input
    // The "continue" are so that it skips checking other condition if one is true
    // Avoid stacking bunch of else if, hard to read
    while (state == 3) {
      Serial.print("t: ");
      Serial.println(timer.timeLeft());

      // Joystick to the left will cancel the battle
      if (readJoystickInput() == -1) {state = 0; break;}

      // Joystick to the right will force win
      if (readJoystickInput() == 1) {state = 4; break;}
      
      // Set to epic if there is no time left
      if (timer.timeLeft() <= 0) {state = 4; break;}

      // 0 - 2 seconds: Show red light
      if (timer.currTime() <= 2) {
        setLight( CRGB (255, 0, 0), (static_cast<int>( ( static_cast<float>(timer.timeLeft() - 10) / ( (battleTime - 10000) / 1000) ) * NUM_LEDS )) );
        continue;
      }

      // NOTE: Change the divide number to change blinking speed
      // - Higher value -> slower blink
      // 3 seconds left: Flashing red quickly 
      if (timer.timeLeft() <= 5) {
        if ((millis()/500)%2==0) setLight( CRGB (255, 0, 0) );
        else setLight( CRGB (0, 0, 0) );
        continue;
      }
      
      if (timer.timeLeft() <= 10) {  // 10 seconds left: Flashing red slowly
        if ((millis()/1000)%2==0) setLight( CRGB (255, 0, 0) );
        else setLight( CRGB (0, 0, 0) );
        continue;
      }
      
      // Set light to blue for most part of the match
      setLight( CRGB (0,0,255) , (static_cast<int>( ( static_cast<float>(timer.timeLeft() - 10) / ( (battleTime - 10000) / 1000) ) * NUM_LEDS )) );
    }
    delay(200);
  }
  
  // WIN
  if (state == 4) {
    setLight ( CRGB (238,210,0) );
    
    while (state == 4) {
      // Listen for signal to reset
      if (readJoystickInput() == -1) {state = 0; break;}

      // Change the light in different colors
      if ((millis()/1000)%9==0) setLight(CRGB (255, 255, 255));
      else if ((millis()/300)%8==0) setLight(CRGB (255, 0, 0));
      else if ((millis()/300)%7==0) setLight(CRGB (255, 127, 0));
      else if ((millis()/300)%6==0) setLight(CRGB (255, 255, 0));
      else if ((millis()/300)%5==0) setLight(CRGB (0, 255, 0));
      else if ((millis()/300)%4==0) setLight(CRGB (0, 0, 255));
      else if ((millis()/300)%3==0) setLight(CRGB (75, 0, 130));
      else if ((millis()/300)%2==0) setLight(CRGB (143, 0, 255));
      else setLight(CRGB (143, 154, 255));
    }
  }
}

// Definitions for Clock functions

// Start the timing at current point of time
void Timer::start() {
  refTime = millis();
}

// Return the duration of time passed since start in seconds
int Timer::currTime() const {
  return ( (millis()-refTime) / 1000 );
}

// Set duration of the timer in milliseconds
void Timer::setDuration(const unsigned long milliWant) {
  timeDuration = milliWant;
}

// Return the amount of time left in seconds
int Timer::timeLeft() const {
  return ( ((refTime + timeDuration) - millis()) / 1000 );
}

// Edited (May 29): A lot. See changelog
// Edited (April 24): Reprogrammed to match FSM model, fix the timing issue, allow for ending match on demand
//if you're looking at this code after May 22, 2024, that means you're probably curious about the code. It's quite dog, so please make improvements to this code
// rooms for improvement that the creator of this code wants to see: make it more in lines with a FSM, make better I/O interface that connects with timer better, make it so that you can go to finish on demand instead of needing to wait,
//GO HIGHLANDERS!

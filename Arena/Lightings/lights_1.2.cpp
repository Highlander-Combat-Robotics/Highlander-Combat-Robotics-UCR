#include <Arduino.h>
#include <FastLED.h>
#define NUM_LEDS 150
#define LED_PIN 7

CRGB leds[NUM_LEDS];
int state;  // 0 idle, 1 arm, 2 ready, 3 battle, 4 epic
unsigned long battleTime = 240000;  // 4 minutes. Change this value to change arena time

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  // Confirming light
  FastLED.showColor(CRGB (255,255,255),128);
  state = 0;
  delay(1000);  // Delay 1 second to confirm set up finished
}

void loop() {
  // Input: 
  // - Left (< 400): arm the arena / cancel battle
  // - Right (> 800): start the arena
  // - Middle: do nothing / idle
  while (state == 0) {
    if (analogRead(A0) < 400) {
      unsigned long time = millis();
      while (millis() - time <= 210) {
        if (millis() - time > 200) state = 1;
      }
    }
    if ((millis()/1000)%2==0) FastLED.showColor(CRGB (255,0,0),128);
    else FastLED.showColor(CRGB (0,0,255),128);
  }

  if (state == 1) {
    FastLED.showColor(CRGB (0,255,0),128);
    state = 2;
  }

  while (state == 2) {
    if (analogRead(A0) < 400) {
      unsigned long time = millis();
      while (millis() - time <= 210) {
        if (millis() - time > 200) state = 0;
      }
    }
    else if (analogRead(A0) > 800) {
        unsigned long time = millis();
        while (millis() - time <= 210) {
          if (millis() - time > 200) state = 3;
        }
      }
  }
  
  if (state == 3) {
    unsigned long startTime = millis();
    long timeLeft;
    while (state == 3) {
      timeLeft = battleTime - (millis() - startTime);
      if (analogRead(A0) < 400) {
        unsigned long time = millis();
        while (millis() - time <= 210) {
          if (millis() - time > 200) state = 0;
        }
      } else if (timeLeft <= 0) {state = 4; break;}
      if (timeLeft >= (battleTime-2000)) FastLED.showColor(CRGB (255,0,0),128);
      else if (timeLeft <= 3000) {
        if ((millis()/500)%2==0) FastLED.showColor(CRGB (255,0,0),128);
        else FastLED.showColor(CRGB (0,0,0),0);
      } else if (timeLeft <= 10000) {
        if ((millis()/1000)%2==0) FastLED.showColor(CRGB (238,210,0),128);
        else FastLED.showColor(CRGB (0,0,0),0);
      } else if (timeLeft > 0) FastLED.showColor(CRGB (0,0,255),128);
    }
  }
  
  if (state == 4) {
    FastLED.showColor(CRGB (238,210,0),128);
  }
  while (state == 4) {
    if (analogRead(A0) < 400) {
      unsigned long time = millis();
      while (millis() - time <= 210) {
        if (millis() - time > 200) state = 0;
      }
    }
    if ((millis()/1000)%9==0) FastLED.showColor(CRGB (255, 255, 255),128);
    else if ((millis()/300)%8==0) FastLED.showColor(CRGB (255, 0, 0),128);
    else if ((millis()/300)%7==0) FastLED.showColor(CRGB (255, 127, 0),128);
    else if ((millis()/300)%6==0) FastLED.showColor(CRGB (255, 255, 0),128);
    else if ((millis()/300)%5==0) FastLED.showColor(CRGB (0, 255, 0),128);
    else if ((millis()/300)%4==0) FastLED.showColor(CRGB (0, 0, 255),128);
    else if ((millis()/300)%3==0) FastLED.showColor(CRGB (75, 0, 130),128);
    else if ((millis()/300)%2==0) FastLED.showColor(CRGB (143, 0, 255),128);
    else FastLED.showColor(CRGB (143, 154, 255),128);
  }
}

// Edited (April 24): Reprogrammed to match FSM model, fix the timing issue, allow for ending match on demand
//if you're looking at this code after May 22, 2024, that means you're probably curious about the code. It's quite dog, so please make improvements to this code
// rooms for improvement that the creator of this code wants to see: make it more in lines with a FSM, make better I/O interface that connects with timer better, make it so that you can go to finish on demand instead of needing to wait,
//GO HIGHLANDERS!

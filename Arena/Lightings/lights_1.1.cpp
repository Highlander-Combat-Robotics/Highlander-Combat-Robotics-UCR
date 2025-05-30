#include <Arduino.h>
#include <FastLED.h>
#define NUM_LEDS 150
#define LED_PIN 7

CRGB leds[NUM_LEDS];
bool epic;


void setup() {
   FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
   epic = 0;
}

void loop() {
  if (analogRead(A0) > 800 && (epic == 0)){
    for(int i =  0 ; i < 108; i++){ //1 second in delay
        leds[i] = CRGB (255,0,0); //red
        FastLED.show();
    }
    
    for (int j = 0; j < 4; ++j){ //countdown to round start for loop
      delay(1000);
    }
  
    for(int i = 0; i < 108; ++i){
        leds[i] = CRGB (255, 255 ,255); //white
        FastLED.show();
       }
    for (int j = 0; j < 59; ++j){ //round for loop, 1 second early
       delay(1000);
    }

    for(int i = 0; i < 108; ++i){
        leds[i] = CRGB (238, 210 , 0); //yellow 
        FastLED.show();
       }
    for (int j = 0; j < 59; ++j){ 
       delay(1000);
    }

    for(int i = 0; i < 108; ++i){
        leds[i] = CRGB (0, 0 , 255); //blue 5 seconds late 20 seconds early
        FastLED.show();
       }
    for (int j = 0; j < 49; ++j){ //round for loop
       delay(1000);
    }

    for (int j = 0; j < 5; ++j){ //fix this countdown
      for(int i =  0 ; i < 108; i++){
        leds[i] = CRGB (255,0,0); //red
        FastLED.show();
      }
      delay(250);
      for(int i =  0 ; i < 108; i++){
        leds[i] = CRGB (0,0,0); //lights are off
        FastLED.show();
      }
      delay(250);
    }
   
   epic = 1; //set victory lights
} 
  
 else if ((analogRead(A0) < 800) && (epic == 0)) {
  for(int i = 0; i < 41; ++i){ 
    leds[i] = CRGB (0,0,255); //blue
    FastLED.show();
    }
    for(int i =  41 ; i < 96; i++){
    leds[i] = CRGB (255,0,0); //red
    FastLED.show();
    }
    for(int i =  96 ; i < 108; i++){
    leds[i] = CRGB (0,0,255); //blue
    FastLED.show();
    }
  }

  else if (epic == 1){ // in order to stop getting into this loop, you must hard reset (sorry for the really owful code)
  for(int i = 0; i<= 108; ++i){
    leds[i] = CRGB (255, 255, 255); // colors should be based on matrix from timer
    FastLED.show();
  }
  delay(1);
    for(int i = 0; i<= 108; ++i){
    leds[i] = CRGB (255, 0, 0); // colors should be based on matrix from timer
    FastLED.show();
  }
  delay(1);
  for(int i = 0; i<= 108; ++i){
    leds[i] = CRGB (255, 127, 0);
    FastLED.show();
   
  }
   delay(1);
   for(int i = 0; i<= 108; ++i){
    leds[i] = CRGB (255, 255, 0);
    FastLED.show();
   
  }
   delay(1);

 for(int i = 0; i<= 108; ++i){
    leds[i] = CRGB (0, 255, 0);
    FastLED.show();
    
  }
delay(1);
for(int i = 0; i<= 108; ++i){
    leds[i] = CRGB (0, 0, 255);
    FastLED.show();
  }
delay(1);

for(int i = 0; i<= 108; ++i){
    leds[i] = CRGB (75, 0, 130);
    FastLED.show();
    
  }
  delay(1);

for(int i = 0; i<= 108; ++i){
    leds[i] = CRGB (143, 0, 255);
    FastLED.show();
    
  }
  delay(1);
 }
}

//if you're looking at this code after May 22, 2024, that means you're probably curious about the code. It's quite dog, so please make improvements to this code
// rooms for improvement that the creator of this code wants to see: make it more in lines with a FSM, make better I/O interface that connects with timer better, make it so that you can go to finish on demand instead of needing to wait,
//GO HIGHLANDERS!

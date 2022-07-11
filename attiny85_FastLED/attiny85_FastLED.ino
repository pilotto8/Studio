#include "Settings.h"

//CRGB custom = CHSV(28, 170, 255);
byte counter = 4;
byte index;

void setup() {
  bus.begin(4800);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);

}

void loop() {
  if (bus.available()) {
    if (counter == 4){
      index = bus.read();
      leds[index] = 0;
    }
    else {
      leds[index] += bus.read() << ((counter - 1) * 8);
    }
    if (!counter){
      FastLED.show();
      FastLED.delay(12);
      counter = 4;
    }
    else{
      counter--;
    } 
  }
}

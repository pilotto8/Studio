#include "Settings.h"

//CRGB custom = CHSV(28, 170, 255);
byte save_par[2];
byte index;


void setup() {
  bus.begin(4800);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  for (int i = 0; i < 100; i++){
    leds[i] = CHSV(HUE_RED, 255, 100);
  }
  FastLED.show();
}

void loop() {
  if (bus.available()){
    while (bus.available()) {
      index = bus.read();
      if (index >= 100){
        save_par[0] = bus.read();
        save_par[1] = bus.read();
      }
      else{
        leds[index] = CHSV (save_par[0], save_par[1], bus.read());
        FastLED.show();
        FastLED.delay(12);
      }
    }
  }
}

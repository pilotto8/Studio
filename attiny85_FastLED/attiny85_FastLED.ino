#include "Settings.h"

//CRGB custom = CHSV(28, 170, 255);
byte save_par[2];
byte index;


void setup() {
  //bus.begin(4800);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  for (float offset = -1; offset <= 2; offset += 0.075){
    byte i;
    float c;
    for(i = 0, c = 4.712; i < 100; i++, c += 0.314){
      float brightness = offset + sin(c);
      if (brightness < 0){
        brightness = 0;
      }
      else if (brightness > 1){
        brightness = 1;
      }
      leds[i] = CHSV(28, 170, brightness * 255);
    }
    FastLED.show();
  }
}

void loop() {
  /*if (bus.available()){
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
  }*/
  
}

#include "Settings.h"

void setup(){
    Serial.begin(115200);
    randomSeed(analogRead(A0));

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(BRIGHTNESS);
}

void loop(){
    if (new_config){
        animationHandle();
    }
}

void serialEvent(){
    if (Serial.available() > 0){
        Serial.readBytes(led_config, 4);
        new_config = 1;
    }
}
#include "Settings.h"

void setup(){
    Serial.begin(115200);

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(BRIGHTNESS);
}

void loop(){
    if (Serial.available() > 0){
        Serial.readBytes(led_config, 4);
        new_config = 1;
        //Serial.println((char)led_config[hue] + (char)led_config[saturation] + (char)led_config[brightness]);
    }
    if (new_config){
        animationHandle();
    }
    
}

/*void serialEvent(){
    
}*/
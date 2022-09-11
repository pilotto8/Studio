#include "Settings.h"

void setup(){
    Serial.begin(57600);
    pinMode(WAKE_SERIAL, OUTPUT);
    pinMode(INTERRUPT, INPUT);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT), serialEvent, RISING);
    randomSeed(analogRead(MICROPHONE));

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(BRIGHTNESS);
}

void loop(){
    //Serial.println(analogRead(MICROPHONE));

    if (new_config){
        animationHandle();
    }
    if (serial_call){
        new_config = 1;
        serial_call = 0;
    }
}

void serialEvent(){
    if (Serial.available() > 0){
        Serial.readBytes(led_config, 4);
        serial_call = 1;
    }
}
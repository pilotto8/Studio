#include "Settings.h"

void setup(){
    Serial.begin(57600);
    pinMode(WAKE_SERIAL, OUTPUT);
    pinMode(INTERRUPT, INPUT);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT), serialEvent, RISING);
    randomSeed(analogRead(MICROPHONE));

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(BRIGHTNESS);
    digitalWrite(WAKE_SERIAL, 0);
}

void loop(){
    if (new_config){
        animationHandle();
    }
}

void serialEvent(){
    //serial_call = 1;
    Serial.read();
    Serial.read();
    if (Serial.available() > 0){
        Serial.readBytes(led_config, 4);
        new_config = 1;
    }
}
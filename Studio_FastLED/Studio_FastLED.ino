#include "Settings.h"

void setup(){
    Serial.begin(1000000);
    pinMode(WAKE_SERIAL, OUTPUT);
    pinMode(INTERRUPT, INPUT);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT), interrupted, RISING);
    randomSeed(analogRead(MICROPHONE));

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(BRIGHTNESS);
    digitalWrite(WAKE_SERIAL, 0);
}

void loop(){
    if (!serial_call){
        if (change_color){
            change_color = 0;
            changeColor();
        }
        if (new_config){
            animationHandle();
        }
        if (new_config != 1 && queue){
            queue = 0;
            new_config = 1;
            configUnqueue();
        }
    }
    
    if (!serial_call && millis() - comunication_timespan >= 5000 && new_config == 0){
        comunication_timespan = millis();
        FastLED.show();
        FastLED.delay(1000 / UPDATES_PER_SECOND);
        /*if (comunication_failed){
            eepromDownload();
            new_config = 1;
            comunication_failed = 0;
        }*/
    }

    serialEvent();
}

void serialEvent(){
    byte data_len = Serial.available();
    if (data_len > 0){
        serial_call = 0;
        Serial.readBytes(led_config_queue, data_len);
        Serial.flush();
        for(byte i = 0; i < 4; i++){
            Serial.println((int)led_config_queue[i]);
        }
        Serial.println("");
        if (led_config_queue[animation] == led_config[animation]){
            change_color = 1;
            configUnqueue();
        }
        else if (led_config_queue[animation] >= wave_on) {
            configUnqueue();
        }
        else {
            queue = 1;
        }

        /*if (comunication_timespan == 0){
            comunication_failed = 0;
            if (led_config[animation] % 2 == 0){
                eepromUpload();
                Serial.println("Saved");
            }
        }*/
        comunication_timespan = millis();
    }
}

void interrupted(){
    serial_call = 1;
}
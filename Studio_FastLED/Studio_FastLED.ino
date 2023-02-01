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
    //Just to add starting data
    eeprom.update(0, 28);
    eeprom.update(1, 170);
    eeprom.update(2, 255);
    eeprom.update(3, 2);
}

void loop(){
    if (change_color){
        change_color = 0;
        changeColor();
    }
    if (new_config){
        animationHandle();
    }
    else if (queue){
        queue = 0;
        new_config = 1;
        led_config = led_config_queue;
    }

    if (millis() - comunication_timespan >= 5000 && !new_config){
        comunication_timespan = millis();
        FastLED.show();
        if (comunication_failed){
            eepromDownload();
            newConfig = 1;
        }
    }
}

void serialEvent(){
    //serial_call = 1;
    if (Serial.available() > 0){
        byte i;
        Serial.read();
        //Serial.read();
        for (i = 0; i < 4; i++){
            led_config[i] = Serial.read();
        }
        //Serial.readBytes(led_config_queue, 4);
        if (led_config_queue[animation] == led_config[animation]){
            change_color = 1;
            led_config = led_config_queue;
        }
        else {
            new_config = 1;
        }
        if (led_config_queue[animation] % 2 == 1 || led_config_queue[animation] >= wave_on){
            led_config = led_config_queue;
            queue = 0; //ner dubbio
        }
        else if (!queue && !change_color){
            queue = 1;
            led_config[animation]++;
        }
        if (comunication_timespan == 0){
            comunication_failed = 0;
            if (led_config[animation] % 2 == 0){
                eepromUpload();
            }
        }
        comunication_timespan = millis();
    }
}
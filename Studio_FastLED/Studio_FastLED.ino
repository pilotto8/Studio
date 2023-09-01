#include "Settings.h"

void setup(){
    Serial.begin(57600);
    pinMode(WAKE_SERIAL, OUTPUT);
    pinMode(INTERRUPT, INPUT);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT), interrupted, RISING);
    randomSeed(analogRead(MICROPHONE));

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(BRIGHTNESS);
    digitalWrite(WAKE_SERIAL, 0);
    //Just to add starting data
    /*EEPROM.update(0, 28);
    EEPROM.update(1, 170);
    EEPROM.update(2, 255);
    EEPROM.update(3, sin_on);*/
    /*eepromDownload();
    for (byte i = 0; i < 20; i++){
        leds[i] = CHSV(led_config[hue], led_config[saturation], led_config[value]);
        singleRow(i);
    }*/
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
        configUnqueue();
    }

    /*if (!serial_call && millis() - comunication_timespan >= 5000 && new_config == 0){
        comunication_timespan = millis();
        FastLED.show();
        FastLED.delay(1000 / UPDATES_PER_SECOND);
        if (comunication_failed){
            eepromDownload();
            new_config = 1;
        }
    }*/
    if (serial_call){
        digitalWrite(WAKE_SERIAL, 1);
    }
}

void serialEvent(){
    //serial_call = 1;
    if (Serial.available() > 0){
        byte i;
        Serial.read();
        //Serial.read();
        //configUnqueue();
        //Serial.readBytes(led_config_queue, 4);
        if (led_config_queue[animation] == led_config[animation]){
            change_color = 1;
            configUnqueue();
        }
        else {
            new_config = 1;
        }
        if (led_config_queue[animation] % 2 == 1 || led_config_queue[animation] >= wave_on){
            for (byte i = 0; i < 4; i++){
                led_config[i] = led_config_queue[i];
            }
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
        serial_call = 0;
        digitalWrite(WAKE_SERIAL, 0);
    }
}

void interrupted(){

}
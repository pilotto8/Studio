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
    if (change_color){
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
}

void serialEvent(){
    //serial_call = 1;
    if (Serial.available() > 0){
        Serial.readBytes(led_config_queue, 5);
        if (led_config_queue[4] == led_config[4]){
            change_color = 1;
            led_config = led_config_queue;
        }
        else {
            new_config = 1;
        }
        if (led_config_queue[4] % 2 == 1){
            led_config = led_config_queue;
            queue = 0; //ner dubbio
        }
        else if (!queue && !change_color){{
            queue = 1;
            led_config[4]++;
        }
    }
}
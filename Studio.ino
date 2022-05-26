#include "Settings.h"
#define LUCE 1
#define OLED 0
#define RTC 0
void setup() {
    Serial.begin(115200);
    pinMode(SRCLK, OUTPUT);
    pinMode(RCLK, OUTPUT);
    pinMode(DATA_OUT, OUTPUT);
    pinMode(DATA_IN, INPUT_PULLUP);
    
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);

    #if LUCE
        FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
        FastLED.setBrightness(BRIGHTNESS);
        setupPalette();
        do {
            RGBhandle();
            FastLED.show();
            FastLED.delay(1000 / UPDATES_PER_SECOND);
        }
        while(!animation_finished);
        UPDATES_PER_SECOND = 0;
    #endif
    
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    
    #if OLED
    oledInit();
    #endif
    
    #if RTC
    rtc.begin();
    if (rtc.lostPower()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    rtc.disable32K();
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);
    rtc.writeSqwPinMode(DS3231_OFF);
    rtc.disableAlarm(2);
    #endif
    pushBits();
}


void loop() {
    /*if (!digitalRead(DATA_IN)){
        swi = !swi;
        digitalWrite(6, swi);
        while(!digitalRead(DATA_IN));
        delay(60); 
    }*/
    if (!digitalRead(DATA_IN)){
        if (millis() - debounce > 30){
            if (button == 0){
                button = findButton();
                Serial.println(button);
                switch (button){
                    case 1 ... 4:{

                        break;
                    }
                    case 5 ... 7:{
                        
                        break;
                    }
                }
            }
            
        }
    }
    else{
        if (button){
            button = 0;
            setBits(9, 15, 0);
            pushBits();
        }
        debounce = millis();
    }
}

// This function sets up a palette of purple and green stripes.
/*void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}*/


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
/*const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};*/

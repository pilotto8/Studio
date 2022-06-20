#include "Settings.h"
#define LUCE 0
#define OLED 1
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

#define scrolling_time 10
#define debounce 50
void loop() {
    #if LUCE
        RGBhandle();
    #endif

    if (!digitalRead(DATA_IN)){
        if (millis() - last_millis > debounce){
            if (!button){
                button = findButton();
                switch (button){
                    case 1 ... 3:{
                        #if LUCE
                        if (button == center){
                            animation = !animation;
                        }
                        #endif
                        if (button == center){
                            button_pulse = center;
                        }
                        break;
                    }
                    case 4 ... 7:{
                        pushPlugState(button - 3, 2); /// State 2 means to invert the current state
                        break;
                    }
                }
            }
            if (button == up || button == down){
                if (button_pulse == 0 && int((millis() - last_millis + debounce) / 10) % scrolling_time == 0){
                    button_pulse = button;
                }
                else{
                    button_pulse = 0;
                }
            }
        }
    }
    else {
        last_millis = millis();
        if (button){
            button = 0;
            button_pulse = 0;
            setBits(9, 15, 0);
        }
        if (reg_update){
            pushBits();
            reg_update = 0;
        }
    }

    #if OLED
        loadInterface();
    #endif
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

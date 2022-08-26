#include <FastLED.h>

#define NUM_LEDS    100
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG
CRGB leds[NUM_LEDS];
//CRGBPalette16 currentPalette;
//TBlendType    currentBlending;

// PIN
#define LED_PIN 9

// FastLED
byte led_config[4];
bool new_config;

enum parameters{
    hue,
    saturation,
    value,
    animation
};
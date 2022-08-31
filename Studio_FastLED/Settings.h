#include <FastLED.h>

#define NUM_LEDS    100
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 80
//CRGBPalette16 currentPalette;
//TBlendType    currentBlending;

// PIN
#define LED_PIN 9

// FastLED
byte led_config[4];
byte new_config;

enum parameters{
    hue,
    saturation,
    value,
    animation
};

// Animatinos
float offset;
struct{
    byte speed;
    byte value;
    byte sign;
}led_star[100];
//float degree;
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
#define INTERRUPT 2
#define WAKE_SERIAL 3
#define MICROPHONE A0

// FastLED
byte led_config[5];
byte new_config;

enum parameters{
    empty,
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

// Serial
bool serial_call;
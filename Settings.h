//#include <stdio.h>
// LEDs
#include <FastLED.h>
#define NUM_LEDS    100
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG
CRGB leds[NUM_LEDS];
int UPDATES_PER_SECOND = 80;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
byte animation = 0;
byte previousAnimation = -1;
bool animation_finished = 0;
int LEDbuffer[21];
enum modes {
    decrease = -1,
    increase = 1        
};

// PIN
#define LED_PIN 9
#define RCLK 10
#define SRCLK 7
#define DATA_OUT 11
#define DATA_IN 8

// Oled
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define NUM_ROWS 5

enum interfaces{
    home_inter = 1,
    settings_inter,
    light_inter,
    plug_inter,
    clock_inter,
    oled_inter
};
bool oled_update;
byte interface = settings_inter;
byte prev_interface = home_inter;

struct {
    String name;
    int* pointer;
    byte interface;
    byte min;
    byte max;
}element_list[10];
byte element_total;
byte element_selected;
bool selector = 1;
int temp;
bool title_list;


// Registers and buttons
#define CLR(x,y) (x&=(~(1<<y)))
#define SET(x,y) (x|=(1<<y))
byte regState[2];
byte button;
byte button_pulse;

bool reg_update;
enum buttons{
    null,
    up,
    center,
    down,
    bD,
    bC,
    bB,
    bA
};
unsigned long int last_millis;

// EEPROM
#include <EEPROM.h>
#include "Settings.h"
#define OLED 1
#define RTC 1
int* NEW_PARAMETER[] = {0}; // Just for developing purposes. If there are new parameters just put them here one time

void setup() {
    Serial.begin(115200);
    /*bus.begin(4800);
    sendPreset(100, 100);
    sendBrightness(20, 255);*/
    pinMode(SRCLK, OUTPUT);
    pinMode(RCLK, OUTPUT);
    pinMode(DATA_OUT, OUTPUT);
    pinMode(DATA_IN, INPUT_PULLUP);
    
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    
    #if OLED
    oledInit();
    #endif

    
    #if RTC
    rtc.begin();
    rtc.disable32K();
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);
    rtc.writeSqwPinMode(DS3231_OFF);
    rtc.disableAlarm(2);
    #endif

    
    if (NEW_PARAMETER[0] != 0){
        for(int i = 0; NEW_PARAMETER[i]; i++){ 
            eepromUpdate(NEW_PARAMETER[i]);
        }
    }
    eepromDownload();
    regState[0] = EEPROM.read(0);
    regState[1] = EEPROM.read(1);
    pushBits();
    for (int i = 4; i < 9; i++){
        digitalWrite(i - 2, readBits(i));
    }
}


void loop() {
    buttonsHandle();

    #if OLED
        loadInterface();
    #endif
}

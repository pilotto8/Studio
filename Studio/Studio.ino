#include "Settings.h"
#define OLED 1
#define RTC 1
#define MOOVEMENT 1
volatile byte* NEW_PARAMETER[] = {

0}; // Just for developing purposes. If there are new parameters just put them here one time


// Debug flags
#define shift_setup 0

void setup() {
    Serial.begin(57600);
    pinMode(SRCLK, OUTPUT);
    pinMode(RCLK, OUTPUT);
    pinMode(DATA_OUT, OUTPUT);
    pinMode(DATA_IN, INPUT_PULLUP);
    
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);

    pinMode(MW_DATA, INPUT);
    pinMode(MICROPHONE, INPUT);
    //pinMode(LED_BUILTIN, OUTPUT);

    pinMode(WAKE_SERIAL, OUTPUT);
    pinMode(INTERRUPT, INPUT);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT), serialInterrupt, RISING);
    
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
    //rtc.setAlarm2(rtc.now(),DS3231_A2_PerMinute);
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    #endif

    
    #if !shift_setup
    if (NEW_PARAMETER[0] != 0){
        for(byte i = 0; NEW_PARAMETER[i]; i++){ 
            eepromUpdate(NEW_PARAMETER[i]);
        }
    }
    
    eepromDownload();
    
    regState[0] = EEPROM.read(0);
    regState[1] = EEPROM.read(1);
    if (regState[1] << 1 != 0){ //Recovery from register corruption (buttons wuldn't works)
        regState[1] >>= 7;
        regState[1] <<= 7;
    }
    pushBits();
    for (byte i = 4; i < 9; i++){
        digitalWrite(i - 2, readBits(i));
    }

    update_clock = millis() + 200;
    no_interaction = millis();
    no_moovement = millis();
    #endif
}


void loop() {
    buttonsHandle();

    #if OLED
        loadInterface();
    #endif

    #if RTC
        if (millis() - update_clock >= 200){
            update_clock = millis();
            checkTimer();
            update_clock_data = 1;
        }
    #endif

    if (interface != home_inter && millis() - no_interaction >= 30000){
        selector = 1;
        if (interface == alarm_inter){
            resume_registers();
        }
        interface = home_inter;
    }

    #if MOOVEMENT
        checkMoovement();
    #endif
}

void serialInterrupt(){

}
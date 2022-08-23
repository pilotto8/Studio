void getTime(){
    now = rtc.now();
}


void addTimer(byte time_span, byte plugs){
    byte i, c;
    if (num_timer >= 10){
        num_timer = 10; // Just to be safe
        return;
    }
    for (c = 0; c < num_timer && time_span > alarm_timer[c].time_span; c++){}
    //c++;
    if (time_span == alarm_timer[c].time_span){
        alarm_timer[c].plugs |= plugs;
    }
    else {
        
        if (c == 0){
            alarm_timer[c].time_span = time_span;
            alarm_timer[c].plugs = plugs;
            rtc.clearAlarm(1);
            setTimer();
        }
        else {
            for (i = num_timer - 1; i > c + 1; i--){
                alarm_timer[i + 1] = alarm_timer[i];
            }
            alarm_timer[c].time_span = time_span;
            alarm_timer[c].plugs = plugs;
        }
        num_timer++;
    }
    
}

void adjustTimer(){
    byte i;
    for (i = 0; i < 10 && i < num_timer; i++){
        if (alarm_timer[i].time_span > 0){
            alarm_timer[i].time_span--;
        }
    }
}

void setTimer(){
    getTime();
    rtc.setAlarm1(rtc.now() + TimeSpan(alarm_timer[0].time_span * 60), DS3231_A1_Date);
    Serial.println("alarm_timer");
}

void deleteTimer(){
    byte i;
    if (num_timer <= 0){
        num_timer = 0; // Just to be safe
        return;
    }
    for (i = 0; i < num_timer; i++){
        alarm_timer[i] = alarm_timer[i + 1];
    }
    num_timer--;
}

byte prev_minute;

void checkTimer(){
    if (rtc.alarmFired(1)) {
        rtc.clearAlarm(1);
        Serial.println("chitemmuerte");
        for (byte i = 0; i < 4; i++){
            if (readBits(&alarm_timer[0].plugs, i)){
                pushPlugState(i, 2);
            }
        }
        deleteTimer();
        if (num_timer > 0){
            setTimer();
        }
        savePlugState();
    }

    if (now.minute() != prev_minute) {
        prev_minute = now.minute();
        adjustTimer();
    }
}

void saveTempData(){
    temp_minute = now.minute();
    temp_hour = now.hour();
    temp_day = now.day();
    temp_month = now.month();
    temp_year = now.year();
}
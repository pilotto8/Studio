void checkMoovement(){
    if (LEDbuttonTrigg() || digitalRead(MW_DATA) && (millis() - no_moovement <= 12 * 60000)){
        no_moovement = millis();
        if (moovement_state == 0){
            moovement_state = 1;
            sendLightData(1);
        }
    }
     else if (moovement_state == 1){
        if (millis() - no_moovement >= moove_timer * 60000 || LEDbuttonTrigg()){
            moovement_state = 0;
            sendLightData(0);
            for (byte i = 0; i < 4; i++){
                pointerPlug(3 - i);
                if (*plug_trigg){
                    pushPlugState(i, 0);
                }
            }
        }
    }
}

bool LEDbuttonTrigg(){
    if (!digitalRead(LED_BUTTON)){
        if (!led_button){
            if (millis() - last_millis_1 > debounce){
                if (moovement_state == 1){
                    no_moovement = millis() - 12 * 60000;
                }
                led_button = 1;
                return 1;
            }
        }
    }
    else {
        if (led_button){
            led_button = 0;
        }
        last_millis_1 = millis();
    }
    return 0;
}
void checkMoovement(){
    if (micTrigg() /*|| digitalRead(MW_DATA) && millis() - no_moovement <= 12 * 60000*/){
        //digitalWrite(LED_BUILTIN, 1); // Just for testing
        no_moovement = millis();
        if (moovement_state == 0){
            moovement_state = 1;
            sendLightData(1);
        }
    }
     else if (moovement_state == 1){
        //digitalWrite(LED_BUILTIN, 0); // Just for testing
        if (millis() - no_moovement >= moove_timer * 60000 || micTrigg()){
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

bool micTrigg(){
    if (digitalRead(MICROPHONE)){
        if (millis() - no_microphone < 1000 && millis() - no_microphone > 300){
            no_microphone = millis() + 1000;
            return 1;
        }
        else {
            no_microphone = millis();
            tap_counter++;
            return 0;
        }
    }
}

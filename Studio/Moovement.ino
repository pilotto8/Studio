void checkMoovement(){
    if (digitalRead(MW_DATA)){
        digitalWrite(LED_BUILTIN, 1); // Just for testing
        no_moovement = millis();
        if (moovement_state == 0){
            moovement_state = 1;
            sendLightData(1);
        }
    }
     else if (moovement_state == 1){
        digitalWrite(LED_BUILTIN, 0); // Just for testing
        if (millis() - no_moovement >= moove_timer * 60000){
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
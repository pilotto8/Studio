void checkMoovement(){
    if (!moovement_state){
        if (LEDbuttonTrigg() || (digitalRead(MW_DATA) && (millis() - no_moovement < NO_MOOVE_TIMER))){
            moovement_state = 1;
            sendLightData(1);
            no_moovement = millis();
        }
        /*else if (digitalRead(MW_DATA)){
            sendLightData(0, 255, 255, 6);
        }*/

        if (interface != sleep_inter && millis() - no_moovement >= NO_MOOVE_TIMER){
            interface = sleep_inter;
        }
    }
    else {
        if (digitalRead(MW_DATA)){
            no_moovement = millis();
        }
        if (LEDbuttonTrigg() || millis() - no_moovement >= moove_timer * 60000){
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
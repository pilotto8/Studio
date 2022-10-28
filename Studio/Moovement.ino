void checkMoovement(){
    if (!moovement_state){
        if (LEDbuttonTrigg() || (digitalRead(MW_DATA) && (millis() - no_moovement < 720000))){ //720000ms = 12min
            someone_here:
            moovement_state = 1;
            sendLightData(1);
            no_moovement = millis();
            ///break; //to try!!!!!!!!
        }
        else if (moove_wake && (millis() - no_moovement >= 730000)){ //For now 10 seconds to leave
            if (digitalRead(MW_DATA)){
                mill_wake_ignore = millis() / 1000;
                sendLightData(0, 255, 255, 6);
                if (millis() / 1000 - mill_wake >= 4){
                    goto someone_here;
                }
            }
            else if (millis() / 1000 - mill_wake_ignore >= 1){
                mill_wake = millis() / 1000;
            }
            else if (millis() / 1000 - mill_wake_ignore == 4){
                sendLightData(0, 0, 0, 0);
            }
        }

        if (interface != sleep_inter && millis() - no_moovement >= 720000){
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
            if (moove_wake){
                mill_wake = millis() / 1000;
            }
            for (byte i = 0; i < 4; i++){
                pointerPlug(3 - i);
                if (*plug_trigg){
                    pushPlugState(i, 0);
                }
            }
        }
    }
}
void checkMoovement(){
    if (digitalRead(MW_DATA)){
        digitalWrite(LED_BUILTIN, 1);
        no_moovement = millis();
        if (moovement_state == 0){
            moovement_state = 1;
            sendLightData();
        }
    }
     else if (moovement_state == 1){
        digitalWrite(LED_BUILTIN, 0);
        if (millis() - no_moovement >= moove_timer * 60000){
            moovement_state = 0;
            sendLightData(light_hue, light_saturation, 0, light_animation);
        }
    }
}


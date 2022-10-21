byte findButton(){
    byte i = 9;
    byte c = 4;
    
    setSReg(13, 15, 1);
    while(c >= 1){
        setSReg(0, i, i + c - 1, 0);
        pushBits();
        setSReg(0, i, i + c - 1, 1);
        if (digitalRead(DATA_IN)){
            i += c;
        }
        c /= 2;
    }
    if (!(i % 2)){
        setSReg(0, i, i + 1, 0);
        pushBits();
    }
    reg_update = 0;
    return i - 8;
}

void buttonsHandle(){
    if (!digitalRead(DATA_IN)){
        if (millis() - last_millis > debounce){
            if (!button){
                button = findButton();
                switch (button){
                    case 1 ... 3:{
                        if (button == center){
                            button_pulse = center;
                        }
                        break;
                    }
                    case 4 ... 7:{
                        if (interface == alarm_inter){
                            setBits(&temp_plugs, button - 4, 2);
                            setSReg(0, button + 1, 2);
                        }
                        break;
                    }
                }
            }
            
            if (button == up || button == down){
                if (button_pulse == 0 && int((millis() - last_millis + debounce) / 10) % scrolling_time == 0){
                    button_pulse = button;
                }
                else if (int((millis() - last_millis + debounce) / 10) % scrolling_time != 0){
                    button_pulse = 0;
                }
            }

            if (button > 3 && interface != alarm_inter){
                if (millis() - last_millis >= long_press){
                    freeze_registers();
                    temp_plugs = 0;
                    setBits(&temp_plugs, button - 4, 1);
                    setSReg(0, button + 1, 1);
                    byte timerFound = findTimerPlug(button - 4);
                    if (timerFound){
                        temp_time_span = alarm_timer[timerFound - 1].time_span;
                    }
                    else {
                        temp_time_span = 1;
                    }
                    interface = alarm_inter;
                }
            }

        }
        no_interaction = millis();
        no_moovement = millis();

        if (interface == sleep_inter){ // To turn on the lights on interactions
            moovement_state = 1;
            sendLightData(1);
        }

    }
    else {
        if (reg_update){ // Update shift registers (it is here to prevent data corruption from electric arcs)
            pushBits();
            reg_update = 0;
        }


        if (button){
            setSReg(0, 9, 15, 0); // Clear button's bits
            if (button > 3){
                if (interface != alarm_inter){ // To be able to select the power plugs
                    if (millis() - last_millis < long_press){
                        pushPlugState(button - 4, 2); /// State 2 means to invert the current state
                        savePlugState();
                    }
                }
            }
            button = 0;
            button_pulse = 0;
        }

        last_millis = millis();
    }
}

bool LEDbuttonTrigg(){
    if (!digitalRead(LED_BUTTON)){
        if (!led_button){
            if (millis() - last_millis_1 > debounce){
                if (moovement_state == 1){
                    no_moovement = millis() - 720000;
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
void sendLightData(byte hue, byte saturation, byte value, byte animation){
    if (data_light[0] != hue || data_light[1] != saturation || data_light[2] != value || data_light[3] != animation){
        digitalWrite(WAKE_SERIAL, 1);
        data_light[0] = hue;
        data_light[1] = saturation;
        data_light[2] = value;
        data_light[3] = animation;
        serial_call = 1;
        mill_serial_call = millis();
    }
    
}

void sendLightData(byte state){
    if (light_profile != prev_light_profile){
        prev_light_profile = light_profile;
        pointerProfile(light_profile);
    }
    sendLightData(*light_hue, *light_saturation, *light_value, *light_animation * 2 + !state);
}

void pushSerial(){
    Serial.write(data_light[0]);
    Serial.write(data_light[1]);
    Serial.write(data_light[2]);
    Serial.write(data_light[3]);
    digitalWrite(WAKE_SERIAL, 0);
}

void pointerProfile(byte profile){
    switch (profile){
        case 0:{
            light_hue = &light_hue_0;
            light_saturation = &light_saturation_0;
            light_value = &light_value_0;
            light_animation = &light_animation_0;
            break;
        }
        case 1:{
            light_hue = &light_hue_1;
            light_saturation = &light_saturation_1;
            light_value = &light_value_1;
            light_animation = &light_animation_1;
            break;
        }
        case 2:{
            light_hue = &light_hue_2;
            light_saturation = &light_saturation_2;
            light_value = &light_value_2;
            light_animation = &light_animation_2;
            break;
        }
        /*case 3:{
            light_hue = &light_hue_3;
            light_saturation = &light_saturation_3;
            light_value = &light_value_3;
            light_animation = &light_animation_3;
            break;
        }*/
    }
}
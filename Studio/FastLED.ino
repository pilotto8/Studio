void sendLightData(byte hue, byte saturation, byte value, byte animation){
    if (data_light[0] != hue || data_light[1] != saturation || data_light[2] != value || data_light[3] != animation){
        SET(PORTB, 5);
        data_light[0] = hue;
        data_light[1] = saturation;
        data_light[2] = value;
        data_light[3] = animation;
        pushSerial();
        CLR(PORTB, 5);
    }
    
}

void sendLightData(byte state){
    pointerProfile(light_profile);
    sendLightData(*light_hue, *light_saturation, *light_value, *light_animation * 2 + !state);
}

void pushSerial(){
    byte i;
    Serial.write(0);// 2 bytes to avoid bit corruptions
    //Serial.write(0);
    for (i = 0; i < 4; i++){
        Serial.write(data_light[i]);
    }
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
void sendLightData(byte hue, byte saturation, byte value, byte animation){
    SET(PORTB, 5);
    if (Serial){
        Serial.write(hue);
        Serial.write(saturation);
        Serial.write(value);
        Serial.write(animation);
    }
    CLR(PORTB, 5);
}

void sendLightData(byte state){
    pointerProfile(light_profile);
    sendLightData(*light_hue, *light_saturation, *light_value, *light_animation * 2 + !state);
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
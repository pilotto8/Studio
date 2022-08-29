void sendLightData(byte hue, byte saturation, byte value, byte animation){
    if (Serial){
        Serial.write(hue);
        Serial.write(saturation);
        Serial.write(value);
        Serial.write(animation);
    }
    
}

void sendLightData(byte state){
    sendLightData(light_hue, light_saturation, light_value, light_animation * 2 + !state);
}


/*
Serial.print((char)28);
Serial.print((char)170);
Serial.print((char)255);
Serial.print((char)1);
*/
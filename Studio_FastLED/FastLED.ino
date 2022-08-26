void animationHandle(){
    switch (led_config[animation]){
        byte i;
        case 1:{
            for (i = 0; i < NUM_LEDS; i++){
                leds[i] = CHSV(led_config[hue], led_config[saturation], 255);
            }
            new_config = 0;
            break;
        }
    }
    FastLED.show();
}
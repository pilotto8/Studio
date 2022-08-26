void animationHandle(){
    switch (led_config[animation]){
        byte i;
        case 0:{
            for (i = 0; i < NUM_LEDS; i++){
                leds[i] = CHSV(led_config[hue], led_config[saturation], led_config[value]);
            }
            new_config = 0;
            break;
        }
    }
    FastLED.show();
}
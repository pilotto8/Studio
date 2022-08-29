void animationHandle(){
    switch (led_config[animation]){
        case 0 ... 1:{
            for (byte i = 0; i < 20; i++){
                leds[i] = CHSV(led_config[hue], led_config[saturation], led_config[value]);
                singleRow(i);
            }
            new_config = 0;
            break;
        }
        case 2:{
            if (new_config == 1){
                new_config = 2;
                offset = -0.25;

            }
            if (offset <= 1.30){
                byte i;
                float c;
                for(i = 0, c = 4.712; i < 20; i++, c += 0.314){
                  float brightness = offset + sin(c) / 4;
                  if (brightness < 0){
                    brightness = 0;
                  }
                  else if (brightness > 1){
                    brightness = 1;
                  }
                  leds[i] = CHSV(led_config[hue], led_config[saturation], brightness * led_config[value]);
                  singleRow(i);
                }
                offset += 0.075;
            }
            else {
                new_config = 0;
            }
            break;
        }
        case 3:{
            if (new_config == 1){
                new_config = 2;
                offset = 1.25;

            }
            if (offset >= -0.30){
                byte i;
                float c;
                for(i = 0, c = 4.712; i < 20; i++, c += 0.314){
                  float brightness = offset + sin(c) / 4;
                  if (brightness < 0){
                    brightness = 0;
                  }
                  else if (brightness > 1){
                    brightness = 1;
                  }
                  leds[i] = CHSV(led_config[hue], led_config[saturation], brightness * led_config[value]);
                  singleRow(i);
                }
                offset -= 0.075;
            }
            else {
                new_config = 0;
            }
            break;
        }
    }
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void singleRow(byte i){
    leds[i + 40] = leds[i];
    leds[i + 80] = leds[i];
    leds[39 - i] = leds[i];
    leds[79 - i] = leds[i];
}
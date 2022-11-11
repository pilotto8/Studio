void animationHandle(){
    switch (led_config[animation]){
        case single_tone_on ... single_tone_off:{
            for (byte i = 0; i < 20; i++){
                leds[i] = CHSV(led_config[hue], led_config[saturation], led_config[value]);
                singleRow(i);
            }
            new_config = 0;
            break;
        }
        case sin_on:{
            if (new_config == 1){
                new_config = 2;
                //offset = -0.25;
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
                offset = 1.25;
                new_config = 0;
            }
            break;
        }
        case sin_off:{
            if (new_config == 1){
                new_config = 2;
                //offset = 1.25;
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
                offset = -0.25;
                new_config = 0;
            }
            break;
        }
        case led_star_on:{
            if (new_config == 1){
                new_config = 2;
                for (byte i = 0; i < 100; i++){
                    leds[i] = CRGB::Black;
                }
            }
            byte i, c;
            for (i = 0, c = 0; i < 100; i++){
                if (led_star[i].value > 0){
                    if (led_star[i].value < led_star[i].speed){
                        led_star[i].value = 0;
                    }
                    else if ((led_star[i].value >= 255 - led_star[i].speed) && !led_star[i].sign){
                        led_star[i].value = 255;
                        led_star[i].sign = 1;
                        c++;
                    }
                    else {
                        if (led_star[i].sign){
                            led_star[i].value -= led_star[i].speed;
                        }
                        else {
                            led_star[i].value += led_star[i].speed;
                        }
                        c++;
                    }
                    leds[i] = CHSV(led_config[hue], led_config[saturation], led_star[i].value * led_config[value] / 255);
                }
            }
            if (c < 25){
                do {
                    i = random(100);
                }
                while (led_star[i].value != 0);
                led_star[i].speed = random(1, 5);
                led_star[i].value = led_star[i].speed;
                led_star[i].sign = 0;
            }
            break;
        }
        case led_star_off:{
            byte i, c;
            for (i = 0, c = 0; i < 100; i++){
                if (led_star[i].value > 0){
                    if (led_star[i].value < led_star[i].speed){
                        led_star[i].value = 0;
                    }
                    else if ((led_star[i].value >= 255 - led_star[i].speed) && !led_star[i].sign){
                        led_star[i].value = 255;
                        led_star[i].sign = 1;
                        c++;
                    }
                    else {
                        if (led_star[i].sign){
                            led_star[i].value -= led_star[i].speed;
                        }
                        else {
                            led_star[i].value += led_star[i].speed;
                        }
                        c++;
                    }
                    leds[i] = CHSV(led_config[hue], led_config[saturation], led_star[i].value * led_config[value] / 255);
                }
            }
            if (c == 0){
                new_config = 0;
            }
            break;
        }
        case wave_on:{
            if (new_config == 1){
                new_config = 2;
                pointer = 0;
            }
            leds[(pointer - 1) + 10] = 0;
            leds[(pointer - 1) + 9] = 0;
            leds[pointer + 10] = CHSV(led_config[hue], led_config[saturation], led_config[value] * (10 / (10 - pointer)));
            leds[pointer + 9] = leds[pointer + 10];
            singleRow(pointer + 10);
            singleRow(pointer + 9);
            pointer++;
            if (pointer == 10){
                new_config = 0;
            }
            break;
        }
        case wave_off:{
            if (new_config == 1){
                new_config = 2;
                pointer = 10;
            }
            leds[(pointer + 1) + 10] = 0;
            leds[(pointer + 1) + 9] = 0;
            leds[pointer + 10] = CHSV(led_config[hue], led_config[saturation], led_config[value] * (10 / pointer));
            leds[pointer + 9] = leds[pointer + 10];
            singleRow(pointer + 10);
            singleRow(pointer + 9);
            pointer--;
            if (pointer == 0){
                new_config = 0;
            }
            break;
        }
    }
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void singleRow(byte i){
    leds[39 - i] = leds[i];
    leds[i + 40] = leds[i];
    leds[79 - i] = leds[i];
    leds[i + 80] = leds[i];
}

void changeColor(){
    switch (led_config[4]){
        case 0:{
            new_config = 1;
            break;
        }
        case 2:{
            for (byte i = 0; i < 20; i++){
                leds[i] = CHSV(led_config[hue], led_config[saturation], led_config[value]);
                singleRow(i);
            }
            break;
        }
        case 4:{
            for (byte i = 0; i < 100; i++){
                if (led_star[i].value != 0){
                    leds[i] = CHSV(led_config[hue], led_config[saturation], led_star[i].value * led_config[value] / 255);
                }
            }
            break;
        }
    }
}

/*byte rowNeightbour(byte i, byte direction){
    byte offset = i / 20;
    i %= 20;
    if (offset % 2){
        direction = !direction;
    }
    if (direction){
        return ((i + 1) % 20) + offset * 20;
    }
    else {
        return ((i + 19) % 20) + offset * 20;
    }
}

byte colNeightbour(byte i, byte direction){
    byte offset = i / 20;
    i %= 20;
    return 0;
}*/
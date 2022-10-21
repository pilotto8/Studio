void pushBits(){
    CLR(PORTD, 7);
    shiftOut(DATA_OUT, RCLK, MSBFIRST, regState[0]);
    shiftOut(DATA_OUT, RCLK, MSBFIRST, regState[1]);
    SET(PORTD, 7);
}

void setSReg(bool select, byte a, byte b, byte bit){
    if (b > 15){
        b = 15;
    }
    if (a < 0){
        a = 0;
    }

    byte temp[2] = { 0 };
    byte i;
    for (i = 0; i < 16; i++){
        temp[i / 8] <<= 1;
        if (i >= a && i <= b){
            temp[i / 8]++;
        }
    }
    if (bit == 1){
        if (select){
            temp_regState[0] |= temp[0];
            temp_regState[1] |= temp[1];
        }
        else {
            regState[0] |= temp[0];
            regState[1] |= temp[1];
        }
        
    }
    else if (bit == 0){
        if (select){
            temp_regState[0] -= (temp_regState[0] & temp[0]);
            temp_regState[1] -= (temp_regState[1] & temp[1]);
        }
        else {
            regState[0] -= (regState[0] & temp[0]);
            regState[1] -= (regState[1] & temp[1]);
        }
        
    }
    else if (bit == 2){
        if (select){
            temp_regState[0] ^= temp[0];
            temp_regState[1] ^= temp[1];
        }
        else {
            regState[0] ^= temp[0];
            regState[1] ^= temp[1];
        }
        
    }
    if (!select){
        reg_update = 1;
    }
}

void setSReg(bool select, byte a, byte bit){
    setSReg(select, a, a, bit);
}

bool readSReg(bool select, byte a){
    byte temp;
    if (select){
        temp = temp_regState[a / 8] << (a % 8);
    }
    else{
        temp = regState[a / 8] << (a % 8);
    }
    if (temp >> 7){
        return 1;
    }
    return 0;
}


void setBits(byte* byt, byte a, byte bit){
    byte temp = 1;
    temp <<= 7 - a; 
    if (bit == 1){
        *byt |= temp;
    }
    else if (bit == 0){
        *byt -= (*byt & temp);
    }
    else if (bit == 2){
        *byt ^= temp;
    }
}

void setBits(byte* byt, byte a, byte b, byte bit){
    a %= 8;
    b %= 8;
    for (; a <= b; a++){
        setBits(byt, a, bit);
    }
}


bool readBits(byte* byt, byte a){
    byte temp = *byt << a;
    if (temp >> 7){
        return 1;
    }
    return 0;
}

void freeze_registers(){
    temp_regState[0] = regState[0];
    temp_regState[1] = regState[1];
    regState[0] = 0;
    regState[1] = 0;
    reg_freezed = 1;
    reg_update = 1;
}

void resume_registers(){
    regState[0] = temp_regState[0];
    regState[1] = temp_regState[1];
    reg_freezed = 0;
    reg_update = 1;
}


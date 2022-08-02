void pushBits(){
    CLR(PORTD, 7);
    shiftOut(DATA_OUT, RCLK, MSBFIRST, regState[0]);
    shiftOut(DATA_OUT, RCLK, MSBFIRST, regState[1]);
    SET(PORTD, 7);
}

void setBits(byte a, byte b, bool bit){
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
    if (bit){
        regState[0] |= temp[0];
        regState[1] |= temp[1];
    }
    else{
        regState[0] -= (regState[0] & temp[0]);
        regState[1] -= (regState[1] & temp[1]);
    }
    reg_update = 1;
}

void setBits(byte a, bool bit){
    setBits(a, a, bit);
}

bool readBits(byte a){
    byte temp = regState[a / 8] << (a % 8);
    if (temp >> 7){
        return 1;
    }
    return 0;
}
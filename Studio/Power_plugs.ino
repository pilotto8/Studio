void pushPlugState(byte plug, byte state){
    if (state == 2){
        state = !readBits(plug + 5);
    }
    setBits(plug + 5, state);
    digitalWrite(plug + 3, state);
}

void savePlugState(){
    EEPROM.update(0, regState[0]);
    EEPROM.update(1, regState[1]);
}
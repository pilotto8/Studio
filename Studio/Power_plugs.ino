void pushPlugState(byte plug, byte state){
    if (state == 2){
        state = !readBits(plug + 4);
    }
    setBits(plug + 4, state);
    digitalWrite(plug + 2, state);
    EEPROM.update(0, regState[0]);
    EEPROM.update(1, regState[1]);
}

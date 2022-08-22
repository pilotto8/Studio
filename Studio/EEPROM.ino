byte* address[]{&prova, 0};

void eepromUpdate(byte* pointer){
    byte i;
    for (i = 0; address[i] != 0; i++){
        if (pointer == address[i]){
            EEPROM.update(i + eeprom_offset, *pointer);
            parExecutor(i);
            return;
        }
    }
}

void eepromDownload(){
    byte i;
    for (i = 0; address[i] != 0; i++){
        *address[i] = EEPROM.read(i + eeprom_offset);
        parExecutor(i);
    }
}

void parExecutor(byte i){ 
    switch (i){
        case 2:{
            
            break;
        }
    }
}
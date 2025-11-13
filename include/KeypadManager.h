#ifndef KEYPADMANAGER_H
#define KEYPADMANAGER_H

#include "I2CKeyPad.h" // <-- ¡El header de la librería que encontraste!
#include <Wire.h>

#define NO_KEY '\0'

#define ROWS 4
#define COLS 4

class KeypadManager {
public:
    KeypadManager(byte i2cAddress);
    void begin();
    char readKey();
    String getAmount();

private:
    byte _i2cAddress;
    
    char keys[ROWS][COLS] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
    };
    
    I2CKeyPad* _keypad; // <-- ¡Cambiamos el tipo de clase!
    char _lastKey;      // <-- Necesario para la lógica de esta librería
};

#endif
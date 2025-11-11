#ifndef KEYPADMANAGER_H
#define KEYPADMANAGER_H

#include <IoAbstraction.h>
#include <IoAbstractionWire.h> // Para el soporte I2C
#include <Keypad.h>

#define ROWS 4
#define COLS 4

class BasicMatrixKeyboard;

class KeypadManager {
public:
    KeypadManager(byte i2cAddress);
    void begin();
    // readKey() ahora solo lee una tecla (el resto de la lógica va en getAmount())
    char readKey(); 
    String getAmount();

private:
    char keys[ROWS][COLS] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
    };
    
    // Aquí declaramos el expansor I2C (el PCF8574)
    IoAbstractionRef _pcf8574Io; 
    
    // Matriz de pines de la librería IoAbstraction.
    // Usamos los IDs de los pines abstractos (0 a 7)
    pinid_t rowPins[ROWS] = {0, 1, 2, 3}; 
    pinid_t colPins[COLS] = {4, 5, 6, 7};
    
    // Instancia del Keypad
    BasicMatrixKeyboard * _keypad;
};

#endif
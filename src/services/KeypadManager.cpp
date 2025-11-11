#include "KeypadManager.h"
#include <Arduino.h>

KeypadManager::KeypadManager(byte i2cAddress) 
    : _i2cAddress(i2cAddress), _lastKey(NO_KEY) {
    
    // --- CORRECCIÓN 1: El constructor ---
    // El orden correcto es (address, wire)
    _keypad = new I2CKeyPad(_i2cAddress, &Wire);
}

void KeypadManager::begin() {
    // Wire.begin() se llama en main.cpp
    _keypad->begin();
    
    // --- CORRECCIÓN 2: makeKeymap no existe ---
    // Solo le pasamos el puntero a nuestro array de 'keys'
    _keypad->loadKeyMap((char*)keys); 
    
    Serial.println("[Keypad] I2CKeyPad (robtillaart) inicializado.");
}

/**
 * @brief Lee una tecla.
 * Adaptamos la lógica para que solo devuelva una pulsación nueva.
 */
char KeypadManager::readKey() {
    if (_keypad->isPressed()) {
        char key = _keypad->getKey();
        if (key != _lastKey) { 
            _lastKey = key; 
            return key;
        }
    } else {
        _lastKey = NO_KEY;
    }
    return NO_KEY;
}

/**
 * @brief Captura el monto.
 * ¡Esta función NO necesita cambios!
 * La adaptamos en readKey() para que funcione igual.
 */
String KeypadManager::getAmount() {
    String amount = "";
    char key = NO_KEY;
    _lastKey = NO_KEY; 
    
    Serial.println("[Keypad] Esperando ingreso. Finalizar con #.");
    
    while (key != '#') {
        key = readKey(); 
        
        if (key != NO_KEY) {
            if (isDigit(key) || key == '.') {
                amount += key;
                Serial.print(key);
            } 
            else if (key == '*') {
                if (amount.length() > 0) {
                    amount.remove(amount.length() - 1);
                    Serial.print("\b \b"); 
                }
            }
            
            if (key != '#') {
                delay(200); 
            }
        }
    }
    
    Serial.println();
    return amount;
}
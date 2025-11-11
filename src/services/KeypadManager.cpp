#include "KeypadManager.h"
#include <Arduino.h>

KeypadManager::KeypadManager(byte i2cAddress) 
    : _i2cAddress(i2cAddress), _lastKey(NO_KEY) {
    // Constructor correcto (address, wire)
    _keypad = new I2CKeyPad(_i2cAddress, &Wire);
}

void KeypadManager::begin() {
    _keypad->begin();
    
    // --- CORRECCIÓN 1 ---
    // La función loadKeyMap() que usé era de la librería incorrecta.
    // La eliminamos, ya que no hace lo que pensábamos.
    // _keypad->loadKeyMap((char*)keys); 
    
    Serial.println("[Keypad] I2CKeyPad (robtillaart) inicializado.");
    Serial.println("[Keypad] Mapeo de teclas manual activado.");
}

/**
 * @brief Lee una tecla.
 * ¡Aquí está la magia!
 */
char KeypadManager::readKey() {
    // Revisa si hay una tecla presionada
    if (_keypad->isPressed()) {
        
        // --- CORRECCIÓN 2: Mapeo Manual ---
        // 1. Obtener el ÍNDICE (0-15) de la librería
        int keyIndex = _keypad->getKey(); 

        // 2. Convertir el índice lineal (0-15) a coordenadas (fila, col)
        int row = keyIndex / COLS; // División entera (ej. 7 / 4 = 1)
        int col = keyIndex % COLS; // Módulo/Resto (ej. 7 % 4 = 3)

        // 3. Obtener el CARÁCTER de nuestro mapa
        // (ej. si keyIndex es 7, esto es keys[1][3] que es 'B')
        char key = keys[row][col];
        // --- FIN CORRECCIÓN 2 ---

        // 4. Lógica de debounce (evitar repeticiones si se deja presionado)
        if (key != _lastKey) {
            _lastKey = key; // Guardar la tecla actual
            return key;     // Devolver la nueva tecla
        }
    } else {
        // Si no hay nada presionado, resetear
        _lastKey = NO_KEY;
    }

    return NO_KEY; // No hay tecla nueva
}

/**
 * @brief Captura el monto.
 * ¡Esta función NO necesita cambios!
 * Sigue funcionando porque ya arreglamos readKey().
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
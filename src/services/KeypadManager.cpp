#include "KeypadManager.h"
#include <Arduino.h> // Necesario para Serial.println(), delay(), etc.

// La definición de BasicMatrixKeyboard está en la librería, 
// así que incluimos el archivo necesario aquí (la implementación)
#include <Keypad.h> 

// Usamos el namespace de IoAbstraction para ser explícitos
using namespace ioa; 

// --- Implementación del Constructor ---
KeypadManager::KeypadManager(byte i2cAddress) {
    // 1. Inicializar la abstracción del PCF8574 (dirección, 8 pines)
    // Usamos ioFrom8574 para crear la referencia I/O
    _pcf8574Io = ioFrom8574(i2cAddress);
    
    // 2. Crear la instancia del Keypad con los pines abstractos
    // Pasamos el puntero de la abstracción I/O que creamos
    _keypad = new BasicMatrixKeyboard(
        _pcf8574Io, 
        (char*)keys, 
        rowPins, 
        colPins, 
        ROWS, 
        COLS
    );
}

// --- Implementación del Método de Inicialización ---
void KeypadManager::begin() {
    // Configurar todos los pines del PCF8574
    // Filas (P0-P3) como entrada con pull-up (el expansor lo maneja internamente)
    _pcf8574Io->initPin(0, INPUT_PULLUP);
    _pcf8574Io->initPin(1, INPUT_PULLUP);
    _pcf8574Io->initPin(2, INPUT_PULLUP);
    _pcf8574Io->initPin(3, INPUT_PULLUP);

    // Columnas (P4-P7) como salida
    _pcf8574Io->initPin(4, OUTPUT);
    _pcf8574Io->initPin(5, OUTPUT);
    _pcf8574Io->initPin(6, OUTPUT);
    _pcf8574Io->initPin(7, OUTPUT);

    // Inicializar el teclado
    _keypad->initialise();
    Serial.println("[Keypad] IoAbstraction inicializado.");
}

// --- Implementación del Método de Lectura Directa ---
char KeypadManager::readKey() {
    // "Tick" necesario para que IoAbstraction lea el estado actual del PCF8574 por I2C
    _pcf8574Io->giveATick(); 
    return _keypad->getChar();
}

// --- Implementación del Método de Captura de Monto ---
String KeypadManager::getAmount() {
    String amount = "";
    char key = NO_KEY;
    
    Serial.println("[Keypad] Esperando ingreso. Finalizar con #.");
    
    while (key != '#') {
        key = readKey(); 
        
        if (key != NO_KEY) {
            // Permitir solo dígitos y el punto decimal
            if (isDigit(key) || key == '.') {
                amount += key;
                Serial.print(key);
            } 
            // Usamos '*' como borrado/corrección (backspace)
            else if (key == '*') {
                if (amount.length() > 0) {
                    amount.remove(amount.length() - 1);
                    Serial.print("\b \b"); 
                }
            }
            
            // Si la tecla no es el final (#), esperar para evitar rebotes
            if (key != '#') {
                delay(200); 
            }
        }
    }
    
    Serial.println();
    return amount;
}
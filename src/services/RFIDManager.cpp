#include "RFIDManager.h"
#include <Arduino.h> // Necesario para funciones básicas como Serial.println() y String

// --- Implementación del Constructor ---
// Inicializa la instancia privada de MFRC522 con los pines SS y RST
RFIDManager::RFIDManager(byte ssPin, byte rstPin) 
    : _mfrc522(ssPin, rstPin) {}

// --- Implementación del Método de Inicialización ---
void RFIDManager::begin() {
    // SPI.begin() debe ir en el setup() del main.cpp
    _mfrc522.PCD_Init();
}

// --- Implementación del Método de Lectura Principal ---
String RFIDManager::checkAndReadCard() {
    // Revisa si hay una nueva tarjeta y si puede leer su serial (UID)
    if (_mfrc522.PICC_IsNewCardPresent() && _mfrc522.PICC_ReadCardSerial()) {
        
        // El UID se lee y se convierte a String usando el método auxiliar
        String uidStr = uidToString(_mfrc522.uid.uidByte, _mfrc522.uid.size);
        
        // Detener la comunicación con la tarjeta actual para permitir nuevas lecturas
        _mfrc522.PICC_HaltA(); 

        return uidStr;
    }
    // Si no hay tarjeta, retorna una cadena vacía
    return "";
}

// --- Implementación del Método Auxiliar Privado ---
/**
 * Convierte un arreglo de bytes del UID a una cadena hexadecimal legible.
 */
String RFIDManager::uidToString(byte *buffer, byte bufferSize) {
    String uidStr = "";
    for (byte i = 0; i < bufferSize; i++) {
        // Añadir '0' si el byte es menor a 0x10 (para formato 0A:1B:...)
        if (buffer[i] < 0x10) {
            uidStr += "0";
        }
        uidStr += String(buffer[i], HEX);
        // Añadir ':' entre bytes, excepto el último
        if (i < bufferSize - 1) {
            uidStr += ":";
        }
    }
    uidStr.toUpperCase();
    return uidStr;
}
#ifndef RFIDMANAGER_H
#define RFIDMANAGER_H

#include <SPI.h> // Necesaria para el protocolo de comunicación
#include <MFRC522.h> // Librería principal del lector

/**
 * @class RFIDManager
 * @brief Gestiona la inicialización y lectura de tarjetas RFID/NFC con el MFRC522.
 */
class RFIDManager {
public:
    // Constructor: Requiere los pines SS y RST (Slave Select y Reset)
    RFIDManager(byte ssPin, byte rstPin);

    // Método de inicialización
    void begin();

    // Método para la lógica principal de lectura.
    // Retorna una cadena (String) con el UID si encuentra una tarjeta,
    // o una cadena vacía si no hay tarjeta nueva.
    String checkAndReadCard();

private:
    // Instancia privada de la librería MFRC522
    MFRC522 _mfrc522;

    // Método auxiliar privado para construir el UID
    String uidToString(byte *buffer, byte bufferSize);
};

#endif
#ifndef OLEDMANAGER_H
#define OLEDMANAGER_H

// Incluimos las librerías necesarias para la clase
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/**
 * @class OLEDManager
 * @brief Gestiona toda la lógica de visualización en la pantalla OLED.
 */
class OLEDManager {
public:
    // Constructor: Requiere una referencia a la instancia de Adafruit_SSD1306
    OLEDManager(Adafruit_SSD1306 &display);

    // Métodos públicos (API para usar desde el main.cpp)
    void begin();
    void showWelcomeMessage(String rc522_status);
    void showPrompt(String promptLine1, String promptLine2);
    void showMessage(String value, uint8_t size);
    void showAmountToPay(float amount);
    void showCardUID(String uid);
    void showAmountEntry(String amountStr);

private:
    // Referencia privada a la instancia de la pantalla para poder manipularla.
    Adafruit_SSD1306 &_display; 
};

#endif
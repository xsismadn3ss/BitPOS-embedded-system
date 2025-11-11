#include "OLEDManager.h"
#include <Arduino.h>

// implementar constructor
OLEDManager::OLEDManager(Adafruit_SSD1306 &display) : _display(display) {}

// implementacion de métodos

/**
 * Inicializa la pantalla.
 * NOTA: Wire.begin(D2, D1); debe ir antes en el setup() del main.cpp.
 */
void OLEDManager::begin()
{
    if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println("OLED fail. Check conection");
        while (true)
            ; // bloquear flujo si la pantalla no funciona
    }
    _display.clearDisplay();
    _display.setTextColor(SSD1306_WHITE);
    _display.display();
}

void OLEDManager::showPrompt(String promptLine1, String promptLine2)
{
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setCursor(0, 0);
    _display.println(promptLine1);
    _display.println(promptLine2);
    _display.display();
}

void OLEDManager::showMessage(String message, uint8_t size)
{
    _display.clearDisplay();
    _display.setTextSize(size);
    _display.setCursor(0, 0);
    _display.println(message);
    _display.display();
}

/**
 * Muestra el mensaje inicial de bienvenida y estado del RC522.
 */
void OLEDManager::showWelcomeMessage(String rc522_status)
{
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setCursor(0, 0);
    _display.println("Iniciando...");
    _display.println(rc522_status);
    _display.display();
}

/**
 * Muestra el monto a pagar y la instrucción de acercar tarjeta.
 */
void OLEDManager::showAmountToPay(float amount) {
    _display.clearDisplay();
    _display.setTextSize(2); 
    _display.setCursor(0,0);
    _display.print("$");
    _display.println(amount, 2); 

    _display.setTextSize(1); 
    _display.setCursor(0, 24);
    _display.println("Acercar tarjeta");
    _display.println("C: Cancelar");
    _display.display();
}
/**
 * Muestra el UID de la tarjeta detectada.
 */
void OLEDManager::showCardUID(String uid)
{
    _display.clearDisplay();
    _display.setTextSize(2);
    _display.setCursor(0, 0);
    _display.println("UID:");
    _display.setTextSize(1);
    _display.setCursor(0, 24);
    _display.println(uid);
    _display.display();
}

/**
 * Mostrar entrada ingresada
 */
void OLEDManager::showAmountEntry(String amountStr) {
    _display.clearDisplay();
    _display.setTextSize(2); 
    _display.setCursor(0,0);
    _display.print("$");
    _display.println(amountStr); 

    _display.setTextSize(1); 
    _display.setCursor(0, 24);
    _display.println("B:Borrar C:Aceptar");
    _display.println("*:Punto Decimal");
    _display.display();
}
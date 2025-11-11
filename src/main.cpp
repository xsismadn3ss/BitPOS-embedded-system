#include <Arduino.h>
#include <Wire.h>
#include <SPI.h> 
#include <Adafruit_SSD1306.h>

// Incluir servicios
#include "services/StateManager.h"
#include "services/OLEDManager.h"
#include "services/RFIDManager.h"
#include "services/KeypadManager.h"

#define KEYPAD_TEST_MODE 0

// --- Definición de Pines y Constantes ---

// Pines I2C
#define OLED_SDA    D2
#define OLED_SCL    D1
#define KEYPAD_I2C_ADDRESS 0x20

// Pines SPI para RC522
#define RC522_SS    D8
#define RC522_RST   D4

// --- Instanciación de Librerías y Clases ---

// 1. Instancia de la librería OLED (Hardware)
// La librería Adafruit_SSD1306 requiere que se defina la instancia aquí.
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// 2. Instancias de nuestros Managers de Hardware
OLEDManager oled(display);
RFIDManager rfid(RC522_SS, RC522_RST);
KeypadManager keypad(KEYPAD_I2C_ADDRESS);

// 3. Instanciamos el StateManager (La lógica de negocio/flujo)
StateManager appManager(oled, rfid, keypad);

void setup() {
    Serial.begin(115200);
    
    // Iniciar bus I2C para OLED y KeyPad
    Wire.begin(OLED_SDA, OLED_SCL); 
    SPI.begin();                    // SPI para RC522

    // Iniciar managers de los componentes
    oled.begin(); 
    rfid.begin(); 
    keypad.begin(); 
    
    #if KEYPAD_TEST_MODE
        Serial.println("\n[SYSTEM] *** MODO TEST KEYPAD ACTIVADO ***");
        oled.showPrompt("MODO TEST KEYPAD", "Revisar Monitor");
    #else
        Serial.println("\n[SYSTEM] Project TPO Ready.");
        // (El StateManager se encargará de la pantalla de bienvenida)
    #endif
}

void loop() {
    #if KEYPAD_TEST_MODE
        // --- MODO TEST ---
        char key = keypad.readKey();
        
        if (key != NO_KEY) {
            Serial.print("[KEYPAD TEST] Tecla presionada: ");
            Serial.println(key);
            
            // Mostramos en la OLED también
            oled.showPrompt("Tecla presionada:", String(key));
            
            // Pequeño delay para evitar lecturas múltiples (debounce)
            delay(200); 
        }

    #else
        // --- MODO PRODUCCIÓN ---
        appManager.run();
    
    #endif
    
    delay(10);
}
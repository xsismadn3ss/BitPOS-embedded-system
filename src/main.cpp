#include <Arduino.h>
#include <Wire.h>
#include <SPI.h> 
#include <Adafruit_SSD1306.h>

// Incluir servicios
#include "services/StateManager.h"
#include "services/OLEDManager.h"
#include "services/RFIDManager.h"
#include "services/KeypadManager.h"
#include "services/WifiManager.h"
#include "services/CurrencyService.h"
#include "services/PaymentService.h"

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
WiFiManager wifi(WIFI_SSID, WIFI_PASS);
CurrencyService currency(wifi, CURRENCY_API_URL);
PaymentService paymentSvc(wifi, PAYMENT_API_URL, MERCHANT_WALLET_ID);

// 3. Instanciamos el StateManager (La lógica de negocio/flujo)
StateManager appManager(oled, rfid, keypad, wifi, currency, paymentSvc);

void setup() {
    Serial.begin(115200);
    
    Wire.begin(OLED_SDA, OLED_SCL); 
    SPI.begin();                    

    // Iniciar managers BÁSICOS
    oled.begin(); 
    rfid.begin(); 
    keypad.begin(); 
    
    // --- LÓGICA DE CONEXIÓN ROBUSTA ---
    oled.showPrompt("Conectando", "WiFi..."); // Informar al usuario
    
    if (!wifi.connect()) {
        // ¡FALLÓ LA CONEXIÓN!
        Serial.println("\n[SYSTEM] FATAL: Falla de WiFi. Deteniendo.");
        oled.showPrompt("ERROR DE RED", "Reiniciar TPV");
        
        // Detener todo. No podemos continuar.
        while(true) {
            delay(1000);
        }
    }
    
    // Si llegamos aquí, SÍ nos conectamos
    oled.showPrompt("TPV Conectado", "");
    delay(2000); // Pequeña pausa para ver el mensaje

    // --- FIN DE LÓGICA DE CONEXIÓN ---

    
    #if KEYPAD_TEST_MODE
        Serial.println("\n[SYSTEM] *** MODO TEST KEYPAD ACTIVADO ***");
        oled.showPrompt("MODO TEST KEYPAD", "Revisar Monitor");
    #else
        Serial.println("\n[SYSTEM] Project TPO Ready.");
        // (El StateManager mostrará la bienvenida)
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
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

// servicios
#include "services/OLEDManager.h"
#include "services/RFIDManager.h"

// --- definir pines y constantes ---

#define OLED_SDA    D2
#define OLED_SCL    D1
#define OLED_ADDR   0x3C
#define OLED_WIDTH  128
#define OLED_HEIGHT 64
#define OLED_RESET  -1

// Pines para SPI
// SCK=D5, MOSI=D7, MISO=D6 son los pines SPI predeterminados del ESP8266
#define RC522_SS    D8 // Slave Select
#define RC522_RST   D4 // Reset

// instanciar dependencia de display
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

// instanciar managers
OLEDManager oled(display);
RFIDManager rfid(RC522_SS, RC522_RST);

float currentAmount = 0.0;

void setup() {
  Serial.begin(115200);
    Serial.println("\n[SYSTEM] Iniciando Proyecto IoT...");

    // 1. Inicialización de los buses de comunicación
    Wire.begin(OLED_SDA, OLED_SCL); // I2C para OLED
    SPI.begin();                    // SPI para RC522

    // 2. Inicialización de los Managers
    oled.begin();
    rfid.begin();

    // 3. Inicialización de la lógica de negocio
    oled.showWelcomeMessage("RFID OK"); 
    
    // Inicializar semilla para números aleatorios (monto)
    randomSeed(analogRead(A0)); 

    Serial.println("[SYSTEM] Listo.");
}

void loop() {
  // 1. Generar Monto (Simulación del Keypad)
    // Genera un número entero entre 100 y 5000, y lo divide por 100 para tener 2 decimales (1.00 a 50.00)
    currentAmount = random(100, 5001) / 100.0;
    Serial.print("\n[TX] Monto a cobrar: $");
    Serial.println(currentAmount, 2);

    // 2. Mostrar monto y esperar tarjeta (Diagrama de Colaboración)
    oled.showAmountToPay(currentAmount);

    String uid;
    // Bucle para esperar la lectura de una tarjeta
    while (uid.length() == 0) {
        // La lógica de lectura está encapsulada en RFIDManager
        uid = rfid.checkAndReadCard();
        delay(200); 
    }
    
    // 3. Tarjeta detectada
    Serial.print("[TX] Tarjeta Pagadora Detectada - UID: ");
    Serial.println(uid);

    // 4. Mostrar UID en pantalla y mensaje de transacción realizada
    oled.showCardUID(uid);
    delay(500);
    oled.showMessage("Pago realizado con exito", 1);
    delay(1000);

    // --- AQUI VA LA FUTURA LÓGICA DE CONEXIÓN A INTERNET ---
    // En este punto, tendrías 'currentAmount' y 'uid' listos para enviar
    // a un servidor (e.g., usando un WiFiManager o un MQTTManager).
    // Por ejemplo: wifiManager.sendTransaction(uid, currentAmount);

    Serial.println("[SYSTEM] Transacción simulada finalizada. Esperando 5s para nueva TX...");
    delay(5000); // Esperar 5 segundos antes de generar el siguiente monto
}
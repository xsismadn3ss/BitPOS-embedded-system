#include "WiFiManager.h"
#include <Arduino.h>

WiFiManager::WiFiManager(const char* ssid, const char* password)
    : _ssid(ssid), _password(password) {}

/**
 * @brief Verifica el estado actual de la conexión.
 */
bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

/**
 * @brief Intenta conectarse al WiFi (bloqueante).
 * Se llama una vez desde el setup() en main.cpp.
 */
bool WiFiManager::connect(unsigned long timeout) {
    if (isConnected()) {
        return true; // Ya estábamos conectados
    }
    
    Serial.print("[WiFi] Conectando a ");
    Serial.println(_ssid);

    WiFi.mode(WIFI_STA); // Modo Estación (cliente)
    WiFi.begin(_ssid, _password);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED) {
        // Revisar si se agotó el tiempo
        if (millis() - start > timeout) {
            Serial.println("\n[WiFi] Fallo de conexion (Timeout).");
            WiFi.mode(WIFI_OFF); // Apagar la radio si fallamos
            return false;
        }
        delay(500);
        Serial.print(".");
    }

    Serial.println("\n[WiFi] Conectado.");
    Serial.print("[WiFi] IP: ");
    Serial.println(WiFi.localIP());
    return true;
}
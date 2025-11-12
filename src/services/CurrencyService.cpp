#include "CurrencyService.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include  <WiFiClientSecure.h>

CurrencyService::CurrencyService(WiFiManager& wifiManager, const char* baseUrl)
    : _wifi(wifiManager) {
    // Construimos la URL completa una sola vez en el constructor
    _convertUrl = String(baseUrl) + "/convert/";
}

long CurrencyService::convertUsdToBtc(float usdAmount) {
    
    // 1. Guardia: Verificar WiFi
    if (!_wifi.isConnected()) {
        Serial.println("[CurrencySvc] Error: Sin conexion WiFi.");
        return 0;
    }

    StaticJsonDocument<64> requestDoc;
    requestDoc["amount"] = usdAmount;
    String requestPayload;
    serializeJson(requestDoc, requestPayload);

    WiFiClientSecure client;
    HTTPClient http;
    client.setInsecure();

    if (!http.begin(client, _convertUrl)) {
        Serial.println("[CurrencySvc] Error al iniciar HTTP.");
        return 0;
    }
    http.addHeader("Content-Type", "application/json");

    // 4. Enviar Petición POST
    int httpResponseCode = http.POST(requestPayload);

    // 5. Procesar la respuesta
    if (httpResponseCode == HTTP_CODE_OK) { // HTTP 200
        String responsePayload = http.getString();
        Serial.println("[CurrencySvc] Respuesta: " + responsePayload);

        StaticJsonDocument<128> responseDoc;
        DeserializationError error = deserializeJson(responseDoc, responsePayload);

        if (error) {
            Serial.println("[CurrencySvc] Error al parsear JSON de respuesta.");
            http.end();
            return 0;
        }

        // --- ¡AQUÍ ESTÁ LA MAGIA! ---
        
        // 1. Obtener el valor en BTC como 'double' (con todos los decimales)
        double btcValue = responseDoc["value"]; 
        
        // 2. Convertir BTC a Satoshis (1 BTC = 100,000,000 SATs)
        //    Usamos 100000000.0 para forzar la matemática de 'double'
        long satoshis = (long)(btcValue * 100000000.0);
        
        http.end();
        Serial.println("[CurrencySvc] Convertido a: " + String(satoshis) + " SATS");
        return satoshis;

    } else {
        Serial.println("[CurrencySvc] Error HTTP: " + String(httpResponseCode));
        http.end();
        return 0; // <-- ¡CAMBIO!
    }
}
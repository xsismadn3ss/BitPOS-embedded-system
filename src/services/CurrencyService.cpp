#include "CurrencyService.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

CurrencyService::CurrencyService(WiFiManager& wifiManager, const char* baseUrl)
    : _wifi(wifiManager) {
    // Construimos la URL completa una sola vez en el constructor
    _convertUrl = String(baseUrl) + "/convert/";
}

double CurrencyService::convertUsdToBtc(float usdAmount) {
    
    // 1. Guardia: Verificar WiFi
    if (!_wifi.isConnected()) {
        Serial.println("[CurrencySvc] Error: Sin conexion WiFi.");
        return 0.0;
    }

    // 2. Preparar el JSON de la solicitud
    StaticJsonDocument<64> requestDoc; // Pequeño, solo para {"amount": 123.45}
    requestDoc["amount"] = usdAmount;
    String requestPayload;
    serializeJson(requestDoc, requestPayload);

    // 3. Preparar el cliente HTTP
    WiFiClient client;
    HTTPClient http;

    Serial.println("[CurrencySvc] POST a: " + _convertUrl);
    Serial.println("[CurrencySvc] Payload: " + requestPayload);

    if (!http.begin(client, _convertUrl)) {
        Serial.println("[CurrencySvc] Error al iniciar HTTP.");
        return 0.0;
    }
    http.addHeader("Content-Type", "application/json");

    // 4. Enviar Petición POST
    int httpResponseCode = http.POST(requestPayload);

    // 5. Procesar la respuesta
    if (httpResponseCode == HTTP_CODE_OK) { // HTTP 200
        String responsePayload = http.getString();
        Serial.println("[CurrencySvc] Respuesta: " + responsePayload);

        StaticJsonDocument<128> responseDoc; // Pequeño, solo para {"value": 0.123}
        DeserializationError error = deserializeJson(responseDoc, responsePayload);

        if (error) {
            Serial.println("[CurrencySvc] Error al parsear JSON de respuesta.");
            http.end();
            return 0.0;
        }

        // ¡Éxito! Obtenemos el valor como 'double' para precisión
        double btcValue = responseDoc["value"]; 
        http.end();
        return btcValue;

    } else {
        Serial.println("[CurrencySvc] Error HTTP: " + String(httpResponseCode));
        http.end();
        return 0.0;
    }
}
#include "PaymentService.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

PaymentService::PaymentService(WiFiManager& wifiManager, const char* baseUrl, const char* merchantWalletId)
    : _wifi(wifiManager), _merchantWalletId(merchantWalletId) {
    // Construimos la URL completa al endpoint de pagos
    _paymentUrl = String(baseUrl) + "/payments";
}

bool PaymentService::processPayment(String cardUID, double btcAmount) {
    
    // 1. Guardia: Verificar WiFi
    if (!_wifi.isConnected()) {
        Serial.println("[PaymentSvc] Error: Sin conexion WiFi.");
        return false;
    }

    // 2. Preparar el JSON de la solicitud (CreatePaymentDTO)
    StaticJsonDocument<128> requestDoc; // Buffer pequeño es suficiente
    requestDoc["cardUID"] = cardUID;
    requestDoc["merchantWalletId"] = _merchantWalletId;
    requestDoc["amount"] = btcAmount;

    String requestPayload;
    serializeJson(requestDoc, requestPayload);

    // 3. Preparar el cliente HTTP
    WiFiClient client;
    HTTPClient http;

    Serial.println("[PaymentSvc] POST a: " + _paymentUrl);
    Serial.println("[PaymentSvc] Payload: " + requestPayload);

    if (!http.begin(client, _paymentUrl)) {
        Serial.println("[PaymentSvc] Error al iniciar HTTP.");
        return false;
    }
    http.addHeader("Content-Type", "application/json");
    // (Aquí irían futuros headers de autenticación si fueran necesarios)

    // 4. Enviar Petición POST
    int httpResponseCode = http.POST(requestPayload);

    // 5. Procesar la respuesta
    // Como dijiste, solo nos importa el código de estado.
    http.end(); // Liberar recursos

    Serial.println("[PaymentSvc] Codigo HTTP: " + String(httpResponseCode));

    // 6. Retornar éxito (HTTP 200-299 se consideran éxito)
    return (httpResponseCode >= 200 && httpResponseCode < 300);
}
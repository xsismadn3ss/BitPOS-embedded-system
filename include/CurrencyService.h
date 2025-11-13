#ifndef CURRENCYSERVICE_H
#define CURRENCYSERVICE_H

#include <Arduino.h>
#include "WiFiManager.h" // Para verificar la conexión

/**
 * @class CurrencyService
 * @brief Gestiona la conversión de moneda hablando con la API de BitPOS.
 */
class CurrencyService {
public:
    /**
     * @param wifiManager Referencia al gestor de WiFi.
     * @param baseUrl La URL base de la API (ej. "http://bitpos.duckdns.org/api/currency")
     */
    CurrencyService(WiFiManager& wifiManager, const char* baseUrl);

    /**
     * @brief Llama al endpoint /convert para obtener el valor de USD en BTC.
     * @param usdAmount El monto en USD (float, ej: 12.50).
     * @return El valor equivalente en BTC (double, ej: 0.000123) o 0.0 si falla.
     */
    long convertUsdToBtc(float usdAmount);

private:
    WiFiManager& _wifi;
    String _convertUrl; // URL completa al endpoint /convert
};

#endif
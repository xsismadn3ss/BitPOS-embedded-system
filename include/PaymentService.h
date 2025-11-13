#ifndef PAYMENTSERVICE_H
#define PAYMENTSERVICE_H

#include <Arduino.h>
#include "WiFiManager.h" // Para verificar la conexión

/**
 * @class PaymentService
 * @brief Gestiona el envío de la transacción de pago final a la API de BitPOS.
 */
class PaymentService {
public:
    /**
     * @param wifiManager Referencia al gestor de WiFi.
     * @param baseUrl La URL base de la API (ej. "http://bitpos.duckdns.org/api/tx")
     * @param merchantWalletId El ID de la wallet del comerciante (fijo para este TPV).
     */
    PaymentService(WiFiManager& wifiManager, const char* baseUrl, int merchantWalletId);

    /**
     * @brief Llama al endpoint /payments para registrar el pago.
     * @param cardUID El UID de la tarjeta del cliente.
     * @param btcAmount El monto final en BTC (usa double por precisión).
     * @return true si la API reportó éxito (HTTP 200-299), false si falló.
     */
    bool processPayment(String cardUID, double btcAmount);

private:
    WiFiManager& _wifi;
    String _paymentUrl; // URL completa al endpoint /payments
    int _merchantWalletId; // ID de la wallet del comerciante
};

#endif
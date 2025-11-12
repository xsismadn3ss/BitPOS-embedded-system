#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <ESP8266WiFi.h>

/**
 * @class WiFiManager
 * @brief Gestiona una conexión de WiFi persistente para el ESP8266.
 */
class WiFiManager {
public:
    // Constructor: Almacena las credenciales
    WiFiManager(const char* ssid, const char* password);

    /**
     * @brief Intenta conectarse al WiFi.
     * @param timeout Tiempo máximo de espera en milisegundos.
     * @return true si se conectó, false si falló.
     */
    bool connect(unsigned long timeout = 10000); // 10 seg. de timeout

    /**
     * @brief Verifica si estamos conectados (no-bloqueante).
     */
    bool isConnected();

private:
    const char* _ssid;
    const char* _password;
};

#endif
#include "StateManager.h"
#include "OLEDManager.h"
#include "RFIDManager.h"
#include "KeypadManager.h"
#include "AmountEntryHandler.h"
#include "WifiManager.h"
#include "CurrencyService.h"
#include "PaymentService.h"

// --- Constructor e Inicialización ---
StateManager::StateManager(OLEDManager &oled,
                           RFIDManager &rfid,
                           KeypadManager &keypad,
                           WiFiManager &wifi,
                           CurrencyService &currencySvc,
                           PaymentService &paymentSvc)
    : _oled(oled), _rfid(rfid),
      _keypad(keypad),
      _wifi(wifi),
      _currencySvc(currencySvc),
      _paymentSvc(paymentSvc),
      _currentState(STATE_IDLE),
      _currentAmount(0.0) {}

// --- Método Público Principal (El único llamado desde loop()) ---
void StateManager::run()
{
    // La responsabilidad de run() es SÓLO seleccionar la función a ejecutar.
    switch (_currentState)
    {
    case STATE_IDLE:
        handleIdle();
        break;
    case STATE_ENTERING_AMOUNT:
        handleEnteringAmount();
        break;
    case STATE_WAITING_RFID:
        handleWaitingRFID();
        break;
    case STATE_TRANSACTION_COMPLETE:
        handleTransactionComplete();
        break;
    }
}

// --- Lógica Privada de los Estados ---

void StateManager::handleIdle()
{
    _oled.showMessage("Presiona \n'A' para \niniciar", 2);

    char key = _keypad.readKey();

    if (key == 'A')
    {
        _currentState = STATE_ENTERING_AMOUNT;
        Serial.println("[STATE] -> ENTERING_AMOUNT (TX iniciada)");

        _amountHandler.reset();
        _oled.showAmountEntry(_amountHandler.getString());
    }
}

void StateManager::handleEnteringAmount()
{
    // 1. Leer la tecla (no bloqueante)
    char key = _keypad.readKey();
    if (key == NO_KEY)
    {
        return; // No hay tecla, no hacer nada. Salir.
    }

    // 2. Procesar la tecla en el manejador
    bool amountUpdated = _amountHandler.processKey(key);

    // 3. Revisar si el manejador ha finalizado (presionó '#')
    if (_amountHandler.isComplete())
    {
        _currentAmount = _amountHandler.getAmount();

        if (_currentAmount > 0.0)
        {
            _currentState = STATE_WAITING_RFID;
            Serial.println("[STATE] -> WAITING_RFID");
            _oled.showAmountToPay(_currentAmount); // Mostrar monto final
        }
        else
        {
            // Si el monto es 0 o inválido, regresar a IDLE
            _currentState = STATE_IDLE;
            // (El siguiente loop mostrará el mensaje de IDLE)
        }
    }
    // 4. Si la tecla fue válida (actualizó el monto), redibujar la pantalla
    else if (amountUpdated)
    {
        // ¡ACTUALIZACIÓN EN TIEMPO REAL!
        _oled.showAmountEntry(_amountHandler.getString());
    }
    // (Si la tecla fue inválida, ej: 'B' o un segundo '.', no hace nada)
}

void StateManager::handleWaitingRFID()
{

    // 1. REVISAR SI SE CANCELA
    // Ahora este estado también escucha al teclado.
    char key = _keypad.readKey();
    if (key == 'C')
    {
        _currentState = STATE_IDLE;
        Serial.println("[STATE] -> IDLE (Cancelada por usuario)");

        // Limpiamos todo para la próxima transacción
        _amountHandler.reset();
        _currentAmount = 0.0;
        _uid = "";

        // Dar retroalimentación visual
        _oled.showPrompt("Transaccion", "CANCELADA");
        delay(2000); // Mostrar el mensaje por 2 segundos
        return;      // Salir de la función
    }

    // 2. REVISAR SI HAY TARJETA (Lógica original)
    // Si no se presionó 'C', buscamos una tarjeta.
    _uid = _rfid.checkAndReadCard();

    if (_uid.length() > 0)
    {
        _currentState = STATE_TRANSACTION_COMPLETE;
        Serial.print("[STATE] -> COMPLETE. UID: ");
        Serial.println(_uid);
    }
}

void StateManager::handleTransactionComplete() {
    
    bool exito = false; // Variable para rastrear el éxito final
    
    // --------------------------------------------------------
    // PASO 1: Convertir USD a BTC
    // --------------------------------------------------------
    _oled.showPrompt("Procesando...", "Convirtiendo USD...");
    Serial.println("[STATE] Iniciando transaccion. Convirtiendo a BTC...");

    // Llamamos al primer servicio
    double btcAmount = _currencySvc.convertUsdToBtc(_currentAmount);

    // --------------------------------------------------------
    // PASO 2: Validar conversión y Procesar Pago
    // --------------------------------------------------------
    if (btcAmount == 0.0) {
        // Falló la conversión (ya sea por WiFi, API o JSON)
        Serial.println("[STATE] Error en la conversion de moneda.");
        _oled.showPrompt("Error", "Conversion Fallida");
        
    } else {
        // Conversión exitosa, proceder al pago
        Serial.println("[STATE] Conversion OK. Monto en BTC: " + String(btcAmount, 8)); // 8 decimales para BTC
        _oled.showPrompt("Procesando", "Pago...");

        // Llamamos al segundo servicio
        exito = _paymentSvc.processPayment(_uid, btcAmount);
    }

    // --------------------------------------------------------
    // PASO 3: Mostrar resultado final
    // --------------------------------------------------------
    if (exito) {
        _oled.showPrompt("Transaccion", "REALIZADA");
        Serial.println("[STATE] Transaccion Exitosa.");
    } else {
        // Muestra "FALLIDA" si falló la conversión (btcAmount == 0.0) 
        // O si falló el pago (exito == false)
        _oled.showPrompt("Transaccion", "FALLIDA");
        Serial.println("[STATE] Transaccion Fallida.");
    }
    
    // Pausa para que el usuario pueda leer el mensaje
    delay(3000); // 3 segundos
    
    // --------------------------------------------------------
    // PASO 4: Reiniciar el estado
    // --------------------------------------------------------
    _uid = "";
    _currentAmount = 0.0;
    _amountHandler.reset();
    _currentState = STATE_IDLE; 
}
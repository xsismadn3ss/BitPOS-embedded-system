#include "StateManager.h"
#include "OLEDManager.h" 
#include "RFIDManager.h"
#include "KeypadManager.h"

// --- Constructor e Inicialización ---
StateManager::StateManager(OLEDManager& oled, RFIDManager& rfid, KeypadManager& keypad)
    : _oled(oled), _rfid(rfid), _keypad(keypad), 
      _currentState(STATE_IDLE), _currentAmount(0.0) {}

// --- Método Público Principal (El único llamado desde loop()) ---
void StateManager::run() {
    // La responsabilidad de run() es SÓLO seleccionar la función a ejecutar.
    switch (_currentState) {
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

void StateManager::handleIdle() {
    _oled.showPrompt("Listo.", "Presione 'A' para iniciar TX"); // Mensaje de bienvenida

    char key = _keypad.readKey();
    
    if (key == 'A') {
        _currentState = STATE_ENTERING_AMOUNT;
        Serial.println("[STATE] -> ENTERING_AMOUNT (TX iniciada)");
        _oled.showAmountToPay(0.0); // Preparar OLED para la entrada
    } 
}

void StateManager::handleEnteringAmount() {
    // Bloquea aquí hasta que el usuario termine de ingresar el monto y presione '#'
    String montoStr = _keypad.getAmount(); 
    _currentAmount = montoStr.toFloat();

    if (_currentAmount > 0.0) {
        _currentState = STATE_WAITING_RFID;
        Serial.println("[STATE] -> WAITING_RFID");
        _oled.showAmountToPay(_currentAmount); // Mostrar el monto final ingresado
    } else {
        _currentState = STATE_IDLE;
        _oled.showPrompt("Monto invalido.", "Presione 'A' para iniciar TX");
    }
}

void StateManager::handleWaitingRFID() {
    _uid = _rfid.checkAndReadCard();
    
    if (_uid.length() > 0) {
        _currentState = STATE_TRANSACTION_COMPLETE;
        Serial.print("[STATE] -> COMPLETE. UID: ");
        Serial.println(_uid);
    }
}

void StateManager::handleTransactionComplete() {
    _oled.showCardUID(_uid);
    
    // Aquí iría la llamada al WiFiManager, pero por ahora solo esperamos
    // wifiManager.sendTransaction(_currentAmount, _uid);

    delay(5000); 
    
    // Reiniciar y limpiar variables
    _uid = "";
    _currentAmount = 0.0;
    _currentState = STATE_IDLE; 
}
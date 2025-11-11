#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <Arduino.h>

// Declaraciones adelantadas de nuestros Managers para evitar inclusiones recursivas
class OLEDManager;
class RFIDManager;
class KeypadManager;

// Definición de los estados
enum AppState {
    STATE_IDLE,
    STATE_ENTERING_AMOUNT,
    STATE_WAITING_RFID,
    STATE_TRANSACTION_COMPLETE
};

/**
 * @class StateManager
 * @brief Gestiona la máquina de estados de la aplicación, implementando
 * el Principio de Responsabilidad Única para el flujo del programa.
 */
class StateManager {
public:
    // Constructor: Recibe referencias a todos los Managers que necesita coordinar
    StateManager(OLEDManager& oled, RFIDManager& rfid, KeypadManager& keypad);

    // El método principal que reemplaza el 'switch' en el loop()
    void run();

private:
    // Referencias privadas a los Managers
    OLEDManager& _oled;
    RFIDManager& _rfid;
    KeypadManager& _keypad;

    // Estado actual de la aplicación
    AppState _currentState;

    // Variables de estado que deben persistir entre llamadas a run()
    float _currentAmount;
    String _uid;

    // Métodos privados para manejar la lógica de cada estado
    void handleIdle();
    void handleEnteringAmount();
    void handleWaitingRFID();
    void handleTransactionComplete();
};

#endif
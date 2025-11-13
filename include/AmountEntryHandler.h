#ifndef AMOUNTENTRYHANDLER_H
#define AMOUNTENTRYHANDLER_H

#include <Arduino.h>

class AmountEntryHandler
{
public:
    AmountEntryHandler();

    /**
     * @brief Procesa una tecla presionada.
     * @return true si el string del monto cambió, false si no.
     */
    bool processKey(char key);

    /**
     * @brief Resetea el manejador a su estado inicial ("0").
     */
    void reset();

    /**
     * @brief Obtiene el string actual del monto (ej. "12.5").
     */
    String getString() const;

    /**
     * @brief Obtiene el monto final como float.
     */
    float getAmount() const;

    /**
     * @brief Retorna true si se presionó la tecla '#' (finalizar).
     */
    bool isComplete() const;

private:
    String _amountStr;
    bool _hasDecimal;
    bool _isComplete;
};

#endif
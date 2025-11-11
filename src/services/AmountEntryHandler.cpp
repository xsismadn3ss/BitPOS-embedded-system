#include "AmountEntryHandler.h"

AmountEntryHandler::AmountEntryHandler()
{
    reset();
}

void AmountEntryHandler::reset()
{
    _amountStr = "0"; // Iniciar en "0"
    _hasDecimal = false;
    _isComplete = false;
}

String AmountEntryHandler::getString() const
{
    return _amountStr;
}

float AmountEntryHandler::getAmount() const
{
    return _amountStr.toFloat();
}

bool AmountEntryHandler::isComplete() const
{
    return _isComplete;
}

bool AmountEntryHandler::processKey(char key)
{
    if (_isComplete)
        return false;

    // 'C' es para Confirmar
    if (key == 'C')
    {
        _isComplete = true;
        return true;
    }

    // 'B' es para Borrar
    if (key == 'B')
    {
        if (_amountStr.length() > 1)
        {
            char lastChar = _amountStr.charAt(_amountStr.length() - 1);
            if (lastChar == '.')
            {
                _hasDecimal = false;
            }
            _amountStr.remove(_amountStr.length() - 1);
        }
        else
        {
            _amountStr = "0";
        }
        return true;
    }

    // '*' es para Punto Decimal
    if (key == '*')
    {
        if (!_hasDecimal)
        {
            _hasDecimal = true;
            _amountStr += '.';
            return true;
        }
        return false; // Ya hay un punto
    }

    // --- FIN DE LÓGICA DE TECLAS ---

    if (isDigit(key))
    {
        if (_amountStr == "0")
        {
            _amountStr = String(key);
            return true;
        }

        if (_hasDecimal)
        {
            int decimalPos = _amountStr.indexOf('.');
            if (_amountStr.length() - decimalPos <= 2)
            {
                _amountStr += key;
                return true;
            }
        }
        else
        {
            _amountStr += key;
            return true;
        }
        return false; // Violó la regla de 2 decimales
    }

    return false; // Tecla no válida (ej. 'A', 'D', '#')
}
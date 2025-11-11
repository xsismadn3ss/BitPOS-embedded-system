#ifndef I2CSCANNER_H
#define I2CSCANNER_H

#include <Wire.h>
#include <Arduino.h> // Para HardwareSerial

class I2CScanner {
public:
    // El constructor recibe la interfaz I2C y el Serial para imprimir
    I2CScanner(TwoWire& wire, HardwareSerial& serial);
    
    // Método principal que hace el trabajo
    void scan();

private:
    TwoWire& _wire;
    HardwareSerial& _serial;
};

#endif
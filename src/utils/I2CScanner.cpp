#include "I2CScanner.h"

I2CScanner::I2CScanner(TwoWire& wire, HardwareSerial& serial) 
    : _wire(wire), _serial(serial) {}

void I2CScanner::scan() {
    _serial.println("\n[DIAG] Iniciando escaneo I2C (Modo OOP)...");
    byte error, address;
    int nDevices = 0;

    for(address = 1; address < 127; address++ ) {
        _wire.beginTransmission(address);
        error = _wire.endTransmission();

        if (error == 0) {
            _serial.print("[DIAG] Dispositivo encontrado en 0x");
            if (address < 16) _serial.print("0");
            _serial.println(address, HEX);
            nDevices++;
        }
    }
    _serial.println("[DIAG] Escaneo finalizado.");
}
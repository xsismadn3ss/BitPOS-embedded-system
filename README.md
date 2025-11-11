# BitPOS

Punto de pago con soporte para transacciones con Bitcoin. Este prototipo busca mejorar la adopción del uso de Bitcoin, mostrando un caso de uso para pagar con Bitcoin de una forma intuitiva y fácil.

Este proyecto ha sido desarrollado sobre la placa de desarrollo **ESP8266** (NodeMCU v2) utilizando **PlatformIO** como entorno de desarrollo.

## 🚀 Sobre PlatformIO

Este proyecto utiliza **PlatformIO**, una plataforma profesional de desarrollo para microcontroladores que ofrece:

- **Escalabilidad**: Gestión eficiente de dependencias y librerías mediante archivos de configuración
- **Seguridad**: Control de versiones de librerías y herramientas, reduciendo vulnerabilidades
- **Portabilidad**: Compatible con múltiples plataformas y frameworks (Arduino, ESP-IDF, etc.)
- **Organización**: Estructura de proyecto clara y profesional
- **Automatización**: Build, upload y monitorización integrados
- **Colaboración**: Configuración compartible mediante `platformio.ini`

### Ventajas sobre el IDE de Arduino tradicional

- Gestión automática de dependencias
- Mejor integración con control de versiones (Git)
- Soporte para múltiples entornos de desarrollo (VS Code, CLion, etc.)
- Compilación más rápida y eficiente
- Mejor manejo de proyectos complejos con múltiples archivos

## 📋 Requisitos

### Hardware
- **ESP8266** (NodeMCU v2)
- **Keypad 4x4 I2C**
- **Display OLED** (SSD1306, 128x64)
- **Módulo RFID RC522**
- Cables de conexión (jumper wires)
- Fuente de alimentación 3.3V/5V

### Software
- **PlatformIO IDE** o **PlatformIO Core** (CLI)
- **Visual Studio Code** (recomendado) con extensión PlatformIO
- Drivers USB para ESP8266 (CH340/CP2102 según tu placa)

## 🔧 Instalación

1. **Instalar PlatformIO**:
   - Opción A: Instalar la extensión PlatformIO en VS Code
   - Opción B: Instalar PlatformIO Core vía pip: `pip install platformio`

2. **Clonar o descargar este proyecto**

3. **Abrir el proyecto en PlatformIO**:
   ```bash
   # Si usas CLI
   pio project init
   ```

4. **Compilar el proyecto**:
   ```bash
   pio run
   ```

5. **Subir al dispositivo**:
   ```bash
   pio run --target upload
   ```

6. **Monitorizar el puerto serial**:
   ```bash
   pio device monitor
   ```

## 🔌 Conexiones

### Keypad 4x4 I2C
| KEYPAD    | ESP8266   |
|:---------:|:---------:|
| GND       | GND       |
| VCC       | 3.3V      |
| SDA       | D2 (GPIO4)|
| SCL       | D1 (GPIO5)|

### Display OLED (SSD1306)
| Display   | ESP8266   |
|:---------:|:---------:|
| GND       | GND       |
| VCC       | 3.3V      |
| SDA       | D2 (GPIO4)|
| SCL       | D1 (GPIO5)|

> **Nota**: Los componentes que utilizan comunicación I2C pueden compartir los mismos pines (SDA y SCL).

### Módulo RFID RC522
| RFID      | ESP8266   |
|:---------:|:---------:|
| GND       | GND       |
| VCC       | 3.3V      |
| MISO      | D6 (GPIO12)|
| MOSI      | D7 (GPIO13)|
| SCK       | D5 (GPIO14)|
| SDA (SS)  | D8 (GPIO15)|

## 📚 Librerías Utilizadas

Este proyecto utiliza las siguientes librerías (gestionadas automáticamente por PlatformIO):

- **Adafruit GFX Library**: Gráficos para el display OLED
- **Adafruit SSD1306**: Controlador para displays OLED SSD1306
- **MFRC522**: Controlador para el módulo RFID RC522
- **I2CKeyPad**: Manejo del keypad I2C

Todas las dependencias están especificadas en el archivo `platformio.ini`.

## 📁 Estructura del Proyecto

```
Bitpos/
├── include/          # Archivos de cabecera
├── lib/              # Librerías locales
├── src/              # Código fuente principal
│   ├── services/     # Servicios del sistema
│   └── utils/        # Utilidades
├── test/             # Tests unitarios
├── platformio.ini    # Configuración de PlatformIO
└── README.md         # Este archivo
```

## 🛠️ Desarrollo

### Compilar
```bash
pio run
```

### Subir al dispositivo
```bash
pio run --target upload
```

### Limpiar archivos de compilación
```bash
pio run --target clean
```

### Monitor serial
```bash
pio device monitor
```

## 📝 Notas

- Asegúrate de que el ESP8266 esté correctamente alimentado (3.3V)
- Verifica las conexiones antes de encender el dispositivo
- El módulo RFID RC522 requiere 3.3V, no usar 5V directamente

## 📄 Licencia

Este proyecto está bajo la Licencia MIT. Ver el archivo [LICENSE](LICENSE) para más detalles.

## 🤝 Contribuciones

Las contribuciones son bienvenidas. Por favor, abre un issue o pull request para cualquier mejora.
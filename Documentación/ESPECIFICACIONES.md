# Especificaciones Técnicas del Proyecto

## Hardware

### Arduino UNO R3
- Microcontrolador: ATmega328P
- Voltaje: 5V
- Corriente de operación: 40-50 mA
- Pines digitales: 14
- Pines analógicos: 6
- Memoria Flash: 32 KB
- RAM: 2 KB
- EEPROM: 1 KB
- Velocidad: 16 MHz

### Wemos D1 Mini (ESP8266)
- Microcontrolador: ESP8266
- Voltaje: 3.3V (entrada 5V tolerante)
- Corriente: 80-160 mA (operación normal)
- WiFi: 802.11 b/g/n (2.4 GHz solo)
- Pines digitales: 11 (D0-D8 + A0)
- I2C: soportado (D1=SCL, D2=SDA)
- Serial: UART estándar
- Memoria Flash: 4 MB

### Sensor DHT22
- Tecnología: Capacitivo
- Rango temperatura: -40 a +80°C
- Precisión temperatura: ±0.5°C
- Rango humedad: 0-100% RH
- Precisión humedad: ±2%
- Tiempo de respuesta: 2 segundos
- Interfaz: Digital (1 pin)
- Voltaje: 3-5.5V
- Consumo: 0.3-0.5 mA

### Sensor BMP280
- Tecnología: Piezorresistivo
- Rango presión: 300-1100 hPa
- Precisión presión: ±1 hPa
- Resolución: 0.01 hPa
- Temperatura compensada
- Interfaz: I2C o SPI (usamos I2C)
- Voltaje: 1.8-3.6V (tolerancia 5V)
- Consumo: 2-5 μA (standby), 3-12 mA (activo)
- Dirección I2C: 0x76 (default) o 0x77

### Fotoresistencia LDR
- Tipo: Sulfuro de Cadmio (CdS)
- Rango resistencia: 5kΩ (luz) a 200kΩ (oscuridad)
- Tiempo de respuesta: 10-30 ms
- Rango espectral: 400-700 nm (visible)
- Voltaje máximo: 150V
- Consumo: negligible

### Componentes Pasivos
- Resistencia pull-up I2C: 10kΩ (opcional, suele venir en módulos)
- Resistencia divisor LDR: 10kΩ
- Capacitores de desacople: 100nF (opcional, para estabilidad)

## Software

### Arduino Sketch
- Lenguaje: C++
- Compilador: avr-gcc
- Velocidad Serial: 9600 baud
- Velocidad I2C: 100 kHz
- Intervalo lectura: 10 segundos (configurable)

### Wemos Sketch
- Lenguaje: C++ (Arduino compatible)
- Compilador: esp-gcc
- Velocidad Serial: 9600 baud (Arduino) / 115200 baud (USB)
- WiFi: 802.11 b/g/n
- Puerto HTTP: 80 (estándar)
- Compilación: con Turbo

### Librerías
\`\`\`
Arduino:
- DHT.h (Adafruit) - lectura DHT22
- Adafruit_BMP280.h - lectura BMP280
- Adafruit_Sensor.h - dependencia BMP280
- ArduinoJson.h v6.x - serialización JSON
- Wire.h - I2C (built-in)

Wemos:
- ESP8266WiFi.h - conectividad WiFi
- ESP8266WebServer.h - servidor HTTP
- ArduinoJson.h - parseo JSON
- todos los anteriores soportados
\`\`\`

## Comunicación

### Serial Arduino → Wemos
- Protocolo: UART asíncrono
- Velocidad: 9600 baud
- Formato datos: JSON línea
- Intervalo: 10 segundos
- Longitud máxima JSON: 256 bytes

### JSON Transmitido
\`\`\`json
{"temp":23.5,"hum":45,"pres":1013,"luz":450}
\`\`\`

Campos:
- `temp` (float): Temperatura °C
- `hum` (int): Humedad relativa %
- `pres` (int): Presión hPa
- `luz` (int): Luminosidad 0-1023

### HTTP Wemos → Cliente
- Método: GET
- Puerto: 80
- Rutas:
  - `/` → Página HTML completa
  - `/datos` → JSON con datos
  - 404 → Error para rutas no encontradas

### Respuesta /datos
\`\`\`json
{
  "temp": 23.5,
  "hum": 45,
  "pres": 1013,
  "luz": 450,
  "timestamp": 1234567890
}
\`\`\`

## Consumo Energético

### Arduino
- Idle: 40-50 mA
- Lectura sensores: 50-80 mA
- Serial TX: +5 mA
- Promedio: 50-70 mA

### Wemos
- WiFi conectado: 80-120 mA
- Servidor HTTP activo: 100-160 mA
- Recepción Serial: +10 mA
- Promedio: 100-160 mA

### Sistema Total
- Consumo combinado: 150-230 mA
- Batería 2000 mAh: ~10 horas operación
- Alimentación recomendada: 2A @ 5V (USB)

## Rango de Operación

### Temperatura ambiente
- Sensor DHT22: -40°C a +80°C
- Sensor BMP280: -40°C a +85°C
- Arduino UNO: 0°C a +70°C
- Wemos D1: 0°C a +40°C

**Rango práctico de operación:** 5°C a +35°C

### Humedad ambiente
- Sensor DHT22: 0-100% RH
- Otros componentes: 0-95% RH (sin condensación)

### Luminosidad
- LDR + resistencia 10kΩ: 0-1023 (rango ADC)
- Oscuridad completa: 0-50
- Luz interior normal: 200-600
- Luz exterior día nublado: 600-850
- Luz solar directa: 900-1023

## Precisión y Exactitud

### Temperatura
- Precisión: ±0.5°C (DHT22)
- Resolución: 0.1°C
- Error de calibración: ±1°C (típico)

### Humedad
- Precisión: ±2% RH (DHT22)
- Resolución: 1% RH
- Histéresis: ±3% RH

### Presión
- Precisión: ±1 hPa (BMP280)
- Resolución: 0.01 hPa
- Error de calibración: ±5 hPa (típico)

### Luminosidad
- Precisión: ±5% (dependencia no lineal)
- Resolución: 1 LSB (10-bit ADC)
- Rango dinámico: 200:1

## Características de Red

### WiFi
- Estándar: 802.11 b/g/n
- Banda: 2.4 GHz solo
- Velocidad: hasta 150 Mbps
- RSSI mínimo: -95 dBm (rango 100m aprox)
- Latencia típica: 10-50 ms

### HTTP
- Método: GET (stateless)
- Keep-Alive: no (conexión por solicitud)
- Timeout: 30 segundos
- Máximas conexiones concurrentes: 4

## Almacenamiento

### Arduino
- Buffer Serial: 256 bytes
- Buffer JSON: 200 bytes
- Datos en RAM: 50 bytes (actuales)
- Espacio disponible: ~1.5 KB

### Wemos
- Flash: 4 MB (código + SPIFFS)
- Código programa: ~200 KB
- Espacio disponible: ~3.8 MB (unused)
- RAM: ~81 KB (50 KB usualmente disponibles)

---

## Notas Importantes

1. **Voltajes:** Arduino es 5V, Wemos es 3.3V. BMP280 requiere 3.3V. DHT22 tolera ambos.

2. **I2C Pull-ups:** Muchos módulos BMP280 ya incluyen resistencias pull-up. Si causa problemas, desactiva en código o remueve.

3. **Interferencia:**
   - Aleja cables de fuentes de energía (EMI)
   - Usa cables cortos cuando sea posible
   - Agrega capacitores 100nF si es necesario

4. **Calibración:**
   - DHT22 viene calibrado de fábrica
   - BMP280 se auto-calibra cada medición
   - LDR no necesita calibración (relativo)

5. **Lifespan:**
   - DHT22: 5+ años en operación normal
   - BMP280: 10+ años
   - LDR: 5+ años
   - Arduino UNO: indefinido (componentes sólidos)
   - Wemos: 3-5 años (célula flash tiene ciclos limitados)

# Estación Meteorológica con Arduino + Wemos D1 Mini

Proyecto de estación meteorológica autónoma. Captura datos ambientales en tiempo real y los muestra en una página web accesible desde una red local.

## Características

- Medición de temperatura, humedad, presión atmosférica y luminosidad
- Servidor web embebido en Wemos D1 Mini
- Página web moderna con actualización en tiempo real
- Comunicación serial Arduino → Wemos
- Funcionamiento autónomo
- Bajo consumo de energía

## Componentes Necesarios

### Electrónica
- Arduino UNO R3
- Wemos D1 Mini (ESP8266)
- Sensor DHT22 (temperatura + humedad)
- Sensor BMP280 (presión atmosférica)
- Fotoresistencia LDR (luminosidad)
- Resistencia 10kΩ (para divisor resistivo del LDR)
- Cables Dupont
- Protoboard

### Software
- Arduino IDE 1.8.x o superior
- Drivers CH340 para Wemos
- Librerías:
  - DHT (by Adafruit)
  - Adafruit BMP280
  - ArduinoJson (by Benoit Blanchon)
  - ESP8266WiFi (incluida)
  - ESP8266WebServer (incluida)

## Estructura del Proyecto

\`\`\`
estacion-meteorologica/
├── Arduino/
│   └── estacion_meteorologica.ino         # Código principal Arduino
├── Wemos/
│   └── estacion_meteorologica.ino         # Código principal Wemos
├── Documentación/
│   ├── CONEXIONES.md                      # Esquema de conexiones
│   ├── INSTALACION_LIBRERÍAS.md           # Guía de instalación
│   ├── CONFIGURACIÓN.md                   # Pasos de configuración
│   ├── TROUBLESHOOTING.md                 # Solución de problemas
│   └── ESPECIFICACIONES.md                # Datos técnicos
├── README.md                              # Este archivo
└── LICENSE                                # Licencia MIT
\`\`\`

## Instalación Rápida

### 1. Preparar Arduino IDE

- Instala Arduino IDE desde arduino.cc
- Abre Arduino IDE → Herramientas → Gestor de placas
- Busca e instala "Arduino AVR Boards"

### 2. Instalar Librerías

En Arduino IDE → Herramientas → Gestionar librerías, busca e instala:
- "DHT sensor library" (Adafruit)
- "Adafruit BMP280 Library"
- "ArduinoJson" (Benoit Blanchon)

### 3. Cargar código en Arduino UNO

- Abre `Arduino/estacion_meteorologica.ino`
- Selecciona Herramientas → Placa → Arduino UNO
- Conecta Arduino por USB
- Carga el código (Ctrl+U)

### 4. Configurar y cargar en Wemos

- Abre Arduino IDE → Archivo → Preferencias
- En "URL adicionales de gestor de placas" agrega:
  \`\`\`
  http://arduino.esp8266.com/stable/package_esp8266com_index.json
  \`\`\`
- Herramientas → Gestor de placas → Busca "ESP8266" → Instala
- Selecciona Herramientas → Placa → "LOLIN(WEMOS) D1 Mini (ESP8266)"
- Selecciona Herramientas → Puerto → (tu puerto COM)
- **EDITA las credenciales WiFi** en el código Wemos
- Carga el código (Ctrl+U)

### 5. Conectar Sensores

Ver archivo `Documentación/CONEXIONES.md` para esquema detallado.

### 6. Acceder a la Página Web

- Abre Monitor Serial (Wemos) a 9600 baud
- Verifica que el Wemos se conecte al WiFi
- Anota la IP asignada (ej: 192.168.1.120)
- Desde cualquier PC del colegio, abre navegador y ve a: `http://192.168.1.120`

## Conexiones de Sensores

### DHT22
- VCC → 5V Arduino
- GND → GND
- DATA → Pin 2 Arduino

### BMP280
- VCC → 3.3V Arduino
- GND → GND
- SCL → A5 (Arduino) / D1 (Wemos)
- SDA → A4 (Arduino) / D2 (Wemos)

### LDR
- 5V → Resistencia 10kΩ → A0 → GND
- LDR en paralelo con la resistencia hacia GND

### Comunicación Arduino-Wemos
- Arduino TX → Wemos RX (GPIO 3, RX)
- Arduino GND → Wemos GND

## Formato de Datos JSON

El Arduino envía datos en este formato cada 10 segundos:

\`\`\`json
{"temp":23.7,"hum":48,"pres":1012,"luz":320}
\`\`\`

- `temp`: Temperatura en °C (decimal)
- `hum`: Humedad relativa en % (entero)
- `pres`: Presión atmosférica en hPa (entero)
- `luz`: Luminosidad 0-1023 (entero, 0=oscuro, 1023=muy claro)

## API del Servidor Web

### GET /
Devuelve la página HTML principal con la interfaz completa.

### GET /datos
Devuelve JSON con los últimos valores medidos:

\`\`\`json
{
  "temp": 23.7,
  "hum": 48,
  "pres": 1012,
  "luz": 320,
  "timestamp": 1234567890
}
\`\`\`

## Solución de Problemas

### El Arduino no carga
- Verifica que seleccionaste placa "Arduino UNO"
- Comprueba el puerto COM correcto
- Instala drivers CH340 si es necesario

### El Wemos no se conecta a WiFi
- Verifica SSID y contraseña correctos (son sensibles a mayúsculas)
- Verifica que el WiFi del colegio acepta 2.4GHz
- Prueba con un hotspot de celular para descartar problemas de red

### Los sensores dan valores erráticos
- Verifica todas las conexiones (cables sueltos)
- Reinicia Arduino
- Calibra el sensor BMP280 si es necesario

### No veo la página web
- Verifica la IP en Monitor Serial del Wemos
- Prueba desactivar firewall temporal
- Usa ping desde CMD para probar conectividad

## Mejoras Futuras

- Gráficos históricos con Chart.js
- Almacenamiento en tarjeta SD
- Envío de datos a la nube (Thingspeak, Blynk)
- Pantalla LCD local
- Alertas por exceso/defecto de temperatura
- Exportación de datos CSV

## Autor

Sammauro Francisco - 2025

---

Para más información, consulta la carpeta `Documentación/`.

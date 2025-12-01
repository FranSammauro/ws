# Esquema de Conexiones - Estación Meteorológica

## Diagrama General del Sistema

\`\`\`
[Sensores] → [Arduino UNO R3] → [Serial] → [Wemos D1 Mini] → [Servidor HTTP] → [Red WiFi Colegio]

DHT22      ↓                                     ↓
BMP280     Arduino                              Wemos D1 Mini
LDR        (Procesa datos)                      (Sirve página web)
           (Envía JSON)                         (Recibe JSON por Serial)
\`\`\`

## Conexión Detallada - Arduino UNO R3

### Sensor DHT22 (Temperatura + Humedad)

\`\`\`
DHT22
├─ Pin VCC (1) → Arduino 5V
├─ Pin GND (4) → Arduino GND
└─ Pin DATA (2) → Arduino Pin Digital 2
   (opcional: capacitor 100nF entre DATA y GND)
\`\`\`

**Notas:**
- El DHT22 es más preciso que DHT11 (±2% vs ±5%)
- Requiere lectura cada 2 segundos mínimo
- Tiempo de respuesta: ~2 segundos

### Sensor BMP280 (Presión Atmosférica)

\`\`\`
BMP280 (I2C)
├─ Pin VCC → Arduino 3.3V
├─ Pin GND → Arduino GND
├─ Pin SCL → Arduino A5 (I2C Clock)
└─ Pin SDA → Arduino A4 (I2C Data)
\`\`\`

**Dirección I2C:** 0x76 (por defecto) o 0x77

**Notas:**
- Comunicación I2C a 100kHz
- Opcional: resistencias pull-up 10kΩ (suelen venir integradas)
- Detecta cambios mínimos de 1 Pa

### Fotoresistencia LDR (Luminosidad)

\`\`\`
Divisor Resistivo:
    Arduino 5V
         |
        [R1] 10kΩ
         |
    Pin A0 ← Leer voltaje
         |
       [LDR]
         |
       GND

Conexión detallada:
- Desde Arduino 5V: conecta resistencia 10kΩ hasta Pin A0
- Desde Pin A0: conecta LDR hasta GND
- Optional: capacitor 100nF entre A0 y GND (reduce ruido)
\`\`\`

**Calibración:**
- Oscuro total: valor ≈ 0-50
- Luz normal interior: valor ≈ 300-600
- Luz solar directa: valor ≈ 900-1023

### Arduino → Wemos (Serial UART)

\`\`\`
Arduino UNO          Wemos D1 Mini
    TX (1) ────────→ RX (GPIO 3)
    GND ───────────→ GND (CRUCIAL)
\`\`\`

**Comunicación:**
- Velocidad: 9600 baud
- Protocolo: Envío cada 10 segundos formato JSON
- GND compartido es OBLIGATORIO

---

## Conexión Detallada - Wemos D1 Mini

### Alimentación

\`\`\`
Wemos D1 Mini
├─ Pin 5V (micro-USB o pin) → Fuente 5V
├─ Pin GND → GND común (con Arduino)
└─ Pin 3.3V → Sensores I2C (si no vienen 5V tolerantes)
\`\`\`

**Nota:** Wemos soporta 5V en entrada micro-USB pero pines I/O son 3.3V

### Sensor BMP280 (I2C en Wemos)

\`\`\`
Wemos D1 Mini I2C:
├─ Pin D1 (GPIO5) → BMP280 SCL
├─ Pin D2 (GPIO4) → BMP280 SDA
└─ GND → BMP280 GND
\`\`\`

**Nota:** El código ya está configurado para estos pines

### Entrada Serial desde Arduino

\`\`\`
Wemos D1 Mini - Puerto Serial
├─ RX (GPIO 3) ← Arduino TX
├─ GND → Arduino GND (COMPARTIDO)
└─ Velocidad: 9600 baud
\`\`\`

---

## Esquema Completo en ASCII

\`\`\`
    ╔════════════════════════╗
    ║   Arduino UNO R3       ║
    ║   (Adquisición datos)  ║
    ╚════════════════════════╝
           ↑   ↑   ↑   ↑
       [DHT22] [BMP280] [LDR] [5V GND]
           ↓   ↓    ↓    ↓
    ╔════════════════════════╗
    ║  Procesa sensores      ║
    ║  Forma JSON            ║
    ║  Envía Serial @9600    ║
    ╚════════════════════════╝
           |
           | UART Serial
           | TX → RX
           | (JSON cada 10s)
           ↓
    ╔════════════════════════╗
    ║   Wemos D1 Mini        ║
    ║   (Servidor Web)       ║
    ╚════════════════════════╝
           |
      Recibe JSON
      Actualiza variables
      Sirve HTTP
           |
           ↓
    ╔════════════════════════╗
    ║  Red WiFi Colegio      ║
    ║  (2.4 GHz)             ║
    ╚════════════════════════╝
           |
           | HTTP GET /
           | IP: 192.168.x.x
           |
           ↓
    ╔════════════════════════╗
    ║  Navegador Web         ║
    ║  (Cualquier PC colegio)║
    ║  Visualiza datos       ║
    ╚════════════════════════╝
\`\`\`

---

## Lista de Componentes - Checklist de Conexiones

- [ ] DHT22 → Pin 2 Arduino + 5V + GND
- [ ] BMP280 → I2C (A4, A5) + 3.3V + GND
- [ ] LDR → A0 con divisor resistivo
- [ ] Resistencia 10kΩ → Divisor LDR
- [ ] Arduino TX → Wemos RX
- [ ] Arduino GND → Wemos GND (VERIFICADO 2 veces)
- [ ] Wemos conectado a USB (alimentación)
- [ ] Todos los cables bien insertados en protoboard

---

## Verificación de Conexiones

### Test de DHT22
\`\`\`cpp
// Agrega al setup()
if (isnan(dht.readTemperature())) {
  Serial.println("ERROR: DHT22 no responde");
}
\`\`\`

### Test de BMP280
\`\`\`cpp
// Agrega al setup()
if (!bmp.begin(0x76)) {
  Serial.println("ERROR: BMP280 no encontrado");
}
\`\`\`

### Test de LDR
\`\`\`cpp
// En el loop()
Serial.print("LDR: ");
Serial.println(analogRead(A0));
\`\`\`

### Test de Comunicación Serial Arduino-Wemos
- Carga código Arduino
- Abre Monitor Serial (9600 baud)
- Verifica que recibas JSON válido cada 10 segundos
- Carga código Wemos
- En Monitor Serial del Wemos, verifica que reciba JSON

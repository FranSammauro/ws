# Como Conectar los Sensores al Arduino

Esta guía te explica paso a paso cómo conectar cada sensor físicamente sin complicaciones técnicas.

## Lo que necesitas

- Arduino UNO R3
- Wemos D1 Mini
- 4 sensores: DHT22, BMP280, LDR y una resistencia de 10kΩ
- Cables Dupont (hembra-macho y macho-macho)
- Protoboard
- 5V y GND = Electricidad positiva y negativa

## Concepto básico

Cada sensor necesita:
- **5V**: Cable rojo (alimentación positiva)
- **GND**: Cable negro (tierra/negativa)  
- **DATO**: Cable de otro color (el cable que envía la información)

---

## Sensor 1: DHT22 (Temperatura y Humedad)

### ¿Qué es?
Es un pequeño cilindro que mide qué tan caliente y húmedo está el aire.

### Pines del DHT22
\`\`\`
    [DHT22]
     |||||
     12345
     
1 = VCC (voltaje positivo)
2 = DATA (información que envía)
3 = NC (no conectar)
4 = GND (tierra)
\`\`\`

### Cómo conectarlo:

| DHT22 | Arduino |
|-------|---------|
| Pin 1 (VCC) | 5V |
| Pin 2 (DATA) | Pin digital 2 |
| Pin 4 (GND) | GND |

**En la protoboard:**
- Rojo (5V) → Pin 1 del DHT22
- Amarillo o Verde (Pin 2) → Pin 2 del DHT22
- Negro (GND) → Pin 4 del DHT22

---

## Sensor 2: BMP280 (Presión Atmosférica)

### ¿Qué es?
Es un cuadrado pequeño que mide la presión del aire (también mide temperatura como referencia).

### Comunicación I2C
Este sensor habla con el Arduino usando un sistema llamado **I2C**, que usa 2 cables especiales:
- **SDA**: Envía datos
- **SCL**: Envía el reloj (sincronización)

### Pines en Arduino UNO para I2C:
- **SDA**: A4 (pin analógico 4)
- **SCL**: A5 (pin analógico 5)

### Cómo conectarlo:

| BMP280 | Arduino |
|--------|---------|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

**En la protoboard:**
- Rojo (5V) → VCC del BMP280
- Negro (GND) → GND del BMP280
- Azul o Verde (A4) → SDA del BMP280
- Naranja o Violeta (A5) → SCL del BMP280

---

## Sensor 3: LDR (Medidor de Luz)

### ¿Qué es?
Es una pequeña resistencia que cambia su valor según cuánta luz le llega.

### ¿Cómo funciona?
- Con luz → poca resistencia
- Sin luz → mucha resistencia

Necesita una **división resistiva** (un "divisor" de voltaje).

### Componentes
- 1 LDR (fotoresistencia)
- 1 resistencia de 10kΩ (marrón-negro-naranja)

### Esquema en la protoboard:

\`\`\`
5V ──── [LDR] ──── A0 ──── [Resistencia 10kΩ] ──── GND
\`\`\`

**Paso a paso:**
1. Conecta 5V (rojo) a una línea en la protoboard
2. Conecta el LDR entre el 5V y el A0
3. Conecta la resistencia de 10kΩ entre el A0 y GND (negro)
4. Del punto A0, saca un cable hacia Arduino Pin A0

| Componente | Arduino |
|-----------|---------|
| 5V | 5V |
| A0 (punto medio) | A0 |
| GND | GND |

---

## Sensor 4: Conexión entre Arduino y Wemos

### ¿Por qué?
El Arduino mide los sensores y envía los datos al Wemos, que es el que sirve la página web.

### Conexión Serial (UART)

| Arduino | Wemos D1 Mini |
|---------|---------------|
| TX (Pin 1) | RX (D4) |
| GND | GND |

**Importante**: 
- TX del Arduino va a RX del Wemos
- RX del Arduino va a TX del Wemos (si quieres bidireccional, pero aquí solo enviamos)
- **Deben compartir GND (tierra)**

### Visualmente en la protoboard:

\`\`\`
[Arduino]          [Protoboard]          [Wemos]
  TX(1) ──────► Cable ──────► RX(D4)
  GND ──────► Cable ──────► GND
\`\`\`

---

## Diagrama COMPLETO (Visión general)

\`\`\`
┌─────────────────────────────────────────────────────────┐
│                      ARDUINO UNO                         │
│                                                           │
│  5V  ─── [DHT22-1] [BMP280-VCC] [LDR-5V]               │
│  GND ─── [DHT22-4] [BMP280-GND] [LDR-GND] [Wemos-GND]  │
│  Pin2 ─── [DHT22-2]                                     │
│  A4 ───── [BMP280-SDA]                                  │
│  A5 ───── [BMP280-SCL]                                  │
│  A0 ───── [LDR-Divisor]                                 │
│  TX ───── [Wemos-RX]                                    │
│                                                           │
└─────────────────────────────────────────────────────────┘
                           ↓
                    [PROTOBOARD]
                           ↓
┌─────────────────────────────────────────────────────────┐
│                    WEMOS D1 MINI                         │
│           (Servidor Web, recibe datos)                   │
│                                                           │
│  Conecta a WiFi del colegio                             │
│  Sirve página web en: http://192.168.1.XXX              │
│                                                           │
└─────────────────────────────────────────────────────────┘
\`\`\`

---

## Resumen: Cables por Color (Recomendación)

| Color | Uso |
|-------|-----|
| **Rojo** | 5V (alimentación positiva) |
| **Negro** | GND (tierra/negativa) |
| **Amarillo** | DHT22 DATA |
| **Azul** | I2C SDA (BMP280) |
| **Verde** | I2C SCL (BMP280) |
| **Naranja** | A0 (LDR) |
| **Violeta** | TX/RX (Wemos) |

Esto evita confusiones y facilita revisar las conexiones.

---

## Checklist Final

- [ ] DHT22: 5V, GND, Pin2
- [ ] BMP280: 5V, GND, A4, A5
- [ ] LDR: 5V, A0, GND (con resistencia 10kΩ)
- [ ] Wemos: GND compartido con Arduino, RX conectado a TX del Arduino
- [ ] Todos los cables están bien conectados en la protoboard
- [ ] Ningún cable suelo sin conectar

---

## ¿Qué hago si algo no funciona?

1. **Revisa que todos los cables estén bien encajados** (a veces se afloja con vibraciones)
2. **Verifica que el 5V y GND sean correctos** (son los más críticos)
3. **Comprueba con un multímetro** si hay voltaje donde debe haber
4. Ver la sección TROUBLESHOOTING en la documentación

```</parameter>

```markdown file="Documentación/INSTALACION_LIBRERÍAS.md"
# Instalar Librerías en Arduino IDE

Sigue estos pasos para instalar todas las librerías necesarias.

## Paso 1: Abre Arduino IDE

Inicia la aplicación Arduino IDE en tu computadora.

## Paso 2: Ve a Gestor de Librerías

En el menú superior:
\`\`\`
Herramientas → Gestor de librerías
\`\`\`

O atajo: `Ctrl + Shift + I`

Se abrirá una ventana de búsqueda.

---

## Paso 3: Instala cada librería

### Librería 1: DHT sensor library

1. Escribe en la búsqueda: **DHT**
2. Busca "DHT sensor library by Adafruit"
3. Haz clic en **Instalar**
4. Espera a que termine

### Librería 2: Adafruit BMP280 Library

1. Escribe: **BMP280**
2. Busca "Adafruit BMP280 Library"
3. Haz clic en **Instalar**
4. Espera a que termine

### Librería 3: ArduinoJson

1. Escribe: **ArduinoJson**
2. Busca "ArduinoJson by Benoit Blanchon"
3. Haz clic en **Instalar**
4. Espera a que termine

### Librería 4: ESP8266 (para Wemos)

1. Ve a: `Archivo → Preferencias`
2. En "URLs adicionales de gestor de tarjetas", añade:
   \`\`\`
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   \`\`\`
3. Ve a: `Herramientas → Placa → Gestor de tarjetas`
4. Busca: **ESP8266**
5. Busca "esp8266 by ESP8266 Community"
6. Haz clic en **Instalar**
7. Espera a que termine (es más grande)

---

## Paso 4: Verifica que todo está instalado

En el menú `Herramientas`, deberías ver:
- Placa: Arduino Uno (o Wemos D1 Mini según cuál estés usando)
- Las librerías instaladas aparecerán sin errores

¡Listo! Ahora puedes cargar los códigos.

```</parameter>

```markdown file="Documentación/CONFIGURACIÓN.md"
# Configuración Rápida del Sistema

Sigue estos pasos en orden para que todo funcione.

## Paso 1: Instala las Librerías

Ver documento: `INSTALACION_LIBRERÍAS.md`

---

## Paso 2: Carga el Código en Arduino UNO

1. Abre Arduino IDE
2. Abre el archivo: `Arduino/estacion_meteorologica.ino`
3. Selecciona la placa:
   - `Herramientas → Placa → Arduino AVR Boards → Arduino Uno`
4. Selecciona el puerto COM (donde conectaste Arduino):
   - `Herramientas → Puerto → COM3` (o el que aparezca)
5. Haz clic en **Subir** (flecha derecha en la barra superior)
6. Espera a que salga: "Listo" en verde

---

## Paso 3: Configura el Wemos D1 Mini

### 3.1: Conecta el Wemos por USB a la computadora

### 3.2: Abre el código del Wemos

1. Abre Arduino IDE (nueva ventana)
2. Abre el archivo: `Wemos/estacion_meteorologica.ino`

### 3.3: Selecciona la placa Wemos

\`\`\`
Herramientas → Placa → Gestor de tarjetas
\`\`\`

Busca y selecciona: **LOLIN(WEMOS) D1 R1 and mini**

### 3.4: Configura los parámetros

Deja todo como está, EXCEPTO:

- Placa: **LOLIN(WEMOS) D1 R2 and mini**
- Velocidad de subida: **115200**
- Puerto: **COM3** (o el que aparezca)

### 3.5: **EDITA LAS CREDENCIALES WiFi**

En el código, busca esto (cerca del inicio):

\`\`\`cpp
const char* ssid = "RED_COLEGIO";        // Nombre exacto del WiFi
const char* password = "CONTRASEÑA";     // Contraseña
\`\`\`

Cámbialo por:
\`\`\`cpp
const char* ssid = "nombre_real_del_wifi";      // ej: "WiFi-Colegio"
const char* password = "contraseña_real";       // ej: "micontraseña123"
\`\`\`

**Importante:**
- Usa el nombre EXACTO del WiFi (mayúsculas/minúsculas importa)
- Usa la contraseña EXACTA
- Deja las comillas

### 3.6: Sube el código

1. Haz clic en **Subir** (flecha derecha)
2. Espera a que salga "Listo" en verde

---

## Paso 4: Conecta los Sensores

Ver documento: `CONEXIONES.md`

Conecta exactamente como dice ahí:
- DHT22 al pin 2 y 5V/GND
- BMP280 a I2C (A4, A5) y 5V/GND
- LDR a A0 y 5V/GND
- Wemos RX a TX del Arduino

---

## Paso 5: Enciende el Sistema

1. Conecta Arduino y Wemos a alimentación (USB o batería)
2. Espera 5 segundos a que se inicien
3. En Arduino IDE, abre el Monitor Serial (Arduino):
   - `Herramientas → Monitor Serial`
   - Velocidad: 9600 baud
   - Deberías ver datos en formato JSON cada 10 segundos

---

## Paso 6: Accede a la Página Web

1. Abre cualquier navegador (Chrome, Firefox, etc.)
2. Ve a la IP del Wemos:
   - Si no sabes la IP, ver documento `TROUBLESHOOTING.md`
   - Generalmente es: `http://192.168.1.100` (aprox.)
3. Deberías ver la página con los 4 sensores actualizando

---

## ¿Lista? ¡Listo!

Si llegaste aquí sin errores, ¡funciona todo! 

Si hay problemas, ver: `TROUBLESHOOTING.md`

```</parameter>

```markdown file="Documentación/TROUBLESHOOTING.md"
# Solución de Problemas

## El Arduino no carga el código

**Problema:** Sale error al hacer click en "Subir"

**Soluciones:**
1. Verifica que el **Puerto COM sea correcto** (Herramientas → Puerto)
2. Conecta Arduino directamente a USB (no a través de hub)
3. Intenta otro cable USB
4. Reinicia Arduino IDE

---

## El Wemos no se conecta a WiFi

**Problema:** La página web no aparece, dice "Sin conexión"

**Soluciones:**
1. Verifica que escribiste exactamente igual el nombre WiFi (mayúsculas/minúsculas)
2. Verifica que la contraseña es correcta
3. En Arduino IDE (Wemos), abre Monitor Serial y mira si dice algo
4. Intenta hardcodear la IP:
   \`\`\`cpp
   WiFi.config(IPAddress(192, 168, 1, 100), ...);
   \`\`\`

---

## El Monitor Serial no muestra datos

**Problema:** El Monitor está abierto pero vacío

**Soluciones:**
1. Verifica que la **velocidad sea 9600 baud** (abajo a la derecha del Monitor)
2. Reinicia Arduino (desconecta y reconecta)
3. Verifica que los sensores estén bien conectados

---

## Los sensores leen 0 o valores extraños

**Problema:** Temperatura = 0, Humedad = 0, etc.

**Soluciones:**
1. **DHT22 no funciona:**
   - Verifica que esté conectado en Pin 2
   - Comprueba que hay 5V y GND
   - Espera 2 segundos después de encender (el DHT es lento)

2. **BMP280 no funciona:**
   - Verifica cables I2C en A4 y A5
   - Revisa que SDA y SCL estén bien conectados
   - Si sigue sin funcionar, la dirección I2C podría ser 0x77 en lugar de 0x76

3. **LDR siempre da el mismo valor:**
   - Verifica la resistencia de 10kΩ (marrón-negro-naranja)
   - Prueba en diferentes niveles de luz

---

## ¿Cómo sé la IP del Wemos?

**Opción 1: Por Serial del Wemos**
- Carga el código Wemos
- Abre Monitor Serial a 115200 baud
- Debería mostrar la IP al conectarse a WiFi

**Opción 2: Por router del colegio**
- Accede al panel de control del router (generalmente 192.168.1.1)
- Busca "dispositivos conectados"
- Busca "LOLIN" o "Wemos"
- Ese será tu IP

**Opción 3: Prueba las IPs comunes**
- `http://192.168.1.100`
- `http://192.168.1.120`
- `http://192.168.1.50`

---

## La página web carga pero no actualiza los datos

**Problema:** Ver --/--/-- en todo

**Soluciones:**
1. Verifica que Arduino está enviando datos (Monitor Serial del Arduino)
2. Verifica que el Wemos los está recibiendo (Monitor Serial del Wemos a 115200)
3. Revisa la conexión serial entre Arduino y Wemos
4. Espera 3-5 segundos, a veces tarda en recibir los primeros datos

---

## Página web LENTA o se congela

**Problema:** La página es muy lenta, tarda en actualizar

**Soluciones:**
1. Reduce la frecuencia de actualizaciones en el código JavaScript:
   \`\`\`javascript
   setInterval(actualizarDatos, 5000); // Cambiar a 5 segundos
   \`\`\`
2. Verifica que el Wemos tiene buena señal WiFi
3. Cierra otras pestañas o aplicaciones del navegador

---

## Error: "BMP280 no encontrado"

**Problema:** El Monitor Serial dice "BMP280 no encontrado"

**Soluciones:**
1. Verifica que BMP280 está bien conectado en A4 y A5
2. La dirección I2C podría ser 0x77, cambia en el código:
   \`\`\`cpp
   if (!bmp.begin(0x77)) { // Cambiar 0x76 a 0x77
   \`\`\`
3. Usa un escaneador I2C para verificar la dirección (en Arduino IDE, ejemplos)

---

## ¿Nada funciona?

**Checklist final:**
- [ ] Todas las librerías instaladas (ver INSTALACION_LIBRERÍAS.md)
- [ ] Códigos cargados sin errores en Arduino y Wemos
- [ ] Sensores conectados según CONEXIONES.md
- [ ] Arduino y Wemos compartiendo GND
- [ ] WiFi correctamente configurada
- [ ] Monitor Serial mostrando datos

Si aún no funciona, comparte los mensajes de error en el repositorio de GitHub (Issues).

```</parameter>

```markdown file="Documentación/ESPECIFICACIONES.md"
# Especificaciones Técnicas del Proyecto

## Hardware

### Arduino UNO R3
- Microcontrolador: ATmega328P
- Voltaje: 5V
- Puertos digitales: 14 (0-13)
- Puertos analógicos: 6 (A0-A5)
- Puerto Serial: 1 (UART)
- Velocidad serial: 9600 baud

### Wemos D1 Mini
- Microcontrolador: ESP8266
- WiFi: 802.11 b/g/n
- Puertos digitales: 11 (D0-D8, TX, RX)
- Puertos analógicos: 1 (A0)
- Servidor web: HTTP integrado
- Velocidad serial: 9600 baud (desde Arduino)

### Sensores

**DHT22**
- Rango temperatura: -40 a 80°C (±0.5°C)
- Rango humedad: 0-100% (±2%)
- Velocidad lectura: 1 lectura/2 segundos
- Protocolo: 1-Wire

**BMP280**
- Rango presión: 300 a 1100 hPa
- Precisión: ±1 hPa
- Rango temperatura: -40 a 85°C (±1°C)
- Protocolo: I2C
- Dirección I2C: 0x76 (o 0x77)

**LDR (Fotoresistencia)**
- Rango: 0-1023 (analógico)
- Resistencia: ~10MΩ (oscuro) a ~100Ω (luz)
- Sin unidades estandarizadas (valor relativo)

---

## Software

### Protocolos de Comunicación

**Serial (Arduino → Wemos)**
- Baudrate: 9600
- Datos: 8 bits
- Stop: 1 bit
- Paridad: Ninguna
- Formato: JSON línea por línea

**I2C (Arduino → BMP280)**
- SDA: A4
- SCL: A5
- Dirección: 0x76

**WiFi (Wemos)**
- Estándar: 802.11b/g/n
- Red: Red local del colegio

**HTTP (Navegador → Wemos)**
- Puerto: 80
- Método GET
- Endpoints:
  - `/` (página HTML)
  - `/datos` (API JSON)

### Formato de Datos

**JSON que envía Arduino:**
\`\`\`json
{"temp":23.7,"hum":48,"pres":1012,"luz":320}
\`\`\`

**JSON que sirve Wemos:**
\`\`\`json
{
  "temp": 23.7,
  "hum": 48,
  "pres": 1012,
  "luz": 320,
  "timestamp": 45230
}
\`\`\`

---

## Consumo de Potencia

| Componente | Corriente |
|-----------|-----------|
| Arduino UNO | ~50 mA |
| Wemos D1 Mini | ~80 mA |
| DHT22 | ~0.5 mA |
| BMP280 | ~0.1 mA |
| LDR | Negligible |
| **Total** | **~130 mA** |

Con batería de 2000mAh: ~15 horas de funcionamiento

---

## Intervalos de Lectura

- Arduino: Lee sensores cada 10 segundos
- Wemos: Actualiza página cada 3 segundos
- Monitor Serial Arduino: Muestra cada lectura

---

## Pinout Resumen

### Arduino UNO
\`\`\`
Sensores:
- Pin 2:  DHT22 DATA
- A4:     BMP280 SDA
- A5:     BMP280 SCL
- A0:     LDR
- TX(1):  Wemos RX

Alimentación:
- 5V:     Todos los sensores +
- GND:    Todos los sensores -
\`\`\`

### Wemos D1 Mini
\`\`\`
Comunicación:
- RX (D4): Recibe datos de Arduino TX
- GND:     Compartido con Arduino

WiFi:
- Integrado (no requiere conexión física)
\`\`\`

---

## Rango de Temperaturas

- Operación Arduino: 0 a 50°C
- Operación Wemos: 0 a 40°C
- Almacenamiento: -40 a 125°C
- Sensores: Ver especificaciones individuales

```</parameter>



</CodeProject>

Perfecto. He actualizado el código del Wemos con tus mejoras (mejorado el setup() con comentarios claros) y reescrito la documentación de CONEXIONES de forma **mucho más simple y visual**. 

La nueva documentación de CONEXIONES incluye:

✅ **Explicaciones en lenguaje normal** - Sin tecnicismos complicados
✅ **Tablas claras** - Qué va a dónde
✅ **Código visual** - Diagramas ASCII simples
✅ **Paso a paso** - Cada sensor explicado individualmente
✅ **Código de colores** - Recomendación de cables por color
✅ **Checklist final** - Para verificar que todo está correcto

También actualicé:
- **INSTALACION_LIBRERÍAS.md** - Más simple
- **CONFIGURACIÓN.md** - Paso a paso hasta que funcione
- **TROUBLESHOOTING.md** - Problemas comunes con soluciones
- **ESPECIFICACIONES.md** - Datos técnicos del hardware

Ahora puedes descargar el ZIP (botón tres puntos) y subirlo a GitHub. La documentación es accesible incluso para alguien sin experiencia en electrónica.

<AssistantMessageContentPart partEncoded="eyJ0eXBlIjoidGFzay1kaWFnbm9zdGljcy12MSIsImNyZWF0ZWRBdCI6MTc2NDY0MTYyOTc1OCwiZmluaXNoZWRBdCI6bnVsbCwiaWQiOiJyYzlaalJmMlE4WmJHTUZQIiwibGFzdFBhcnRTZW50QXQiOjE3NjQ2NDE2Mjk3NTgsInBhcnRzIjpbeyJ0eXBlIjoiZmV0Y2hpbmctZGlhZ25vc3RpY3MifV19" />



<AssistantMessageContentPart partEncoded="eyJ0eXBlIjoidGFzay1kaWFnbm9zdGljcy12MSIsImNyZWF0ZWRBdCI6MTc2NDY0MTYyOTc1OCwiZmluaXNoZWRBdCI6bnVsbCwiaWQiOiJyYzlaalJmMlE4WmJHTUZQIiwibGFzdFBhcnRTZW50QXQiOjE3NjQ2NDE2MzAzMzgsInBhcnRzIjpbeyJ0eXBlIjoiZmV0Y2hpbmctZGlhZ25vc3RpY3MifSx7InR5cGUiOiJkaWFnbm9zdGljcy1wYXNzZWQifV19" />



<AssistantMessageContentPart partEncoded="eyJ0eXBlIjoidGFzay1kaWFnbm9zdGljcy12MSIsImNyZWF0ZWRBdCI6MTc2NDY0MTYyOTc1OCwiZmluaXNoZWRBdCI6MTc2NDY0MTYzMDM4NSwiaWQiOiJyYzlaalJmMlE4WmJHTUZQIiwibGFzdFBhcnRTZW50QXQiOjE3NjQ2NDE2MzAzMzgsInBhcnRzIjpbeyJ0eXBlIjoiZmV0Y2hpbmctZGlhZ25vc3RpY3MifSx7InR5cGUiOiJkaWFnbm9zdGljcy1wYXNzZWQifV19" />



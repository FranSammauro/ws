# Guía de Configuración - Paso a Paso

## Paso 1: Configurar Arduino UNO

### 1.1 Preparar el Código

1. Abre Arduino IDE
2. Abre el archivo `Arduino/estacion_meteorologica.ino`
3. Revisa que incluya:
   \`\`\`cpp
   #include <DHT.h>
   #include <Adafruit_BMP280.h>
   #include <ArduinoJson.h>
   \`\`\`

### 1.2 Verificar Pines

Asegúrate que coincidan con tu conexión física:

\`\`\`cpp
#define DHT_PIN 2           // DHT22 en Pin 2
#define DHT_TYPE DHT22      // Tipo DHT22
#define LDR_PIN A0          // LDR en A0
\`\`\`

Si cambiaste pines, actualiza estos valores.

### 1.3 Cargar Código en Arduino

1. Herramientas → Placa → Arduino AVR Boards → Arduino UNO
2. Herramientas → Puerto → (selecciona tu puerto COM)
3. Carga el código: Ctrl+U (o botón Upload)
4. Espera el mensaje: "Listo para cargar"

### 1.4 Verificar en Monitor Serial

1. Herramientas → Monitor Serial
2. Velocidad: 9600 baud (esquina inferior derecha)
3. Deberías ver JSON cada 10 segundos:
   \`\`\`
   {"temp":23.5,"hum":45,"pres":1013,"luz":450}
   \`\`\`

Si no ves datos, verifica:
- Conexiones de sensores
- Librerías instaladas correctamente
- Puerto COM correcto

---

## Paso 2: Configurar Wemos D1 Mini

### 2.1 Preparar el Código

1. Abre Arduino IDE
2. Abre el archivo `Wemos/estacion_meteorologica.ino`

### 2.2 Configurar WiFi

Busca estas líneas (cerca del inicio):

\`\`\`cpp
const char* ssid = "RED_COLEGIO";        // CAMBIAR
const char* password = "CONTRASEÑA";     // CAMBIAR
\`\`\`

**IMPORTANTE:** Edita con tus credenciales:
- `ssid` = nombre exacto del WiFi del colegio
- `password` = contraseña del WiFi
- Respeta mayúsculas/minúsculas

Ejemplo real:
\`\`\`cpp
const char* ssid = "Red_Estudiantes";
const char* password = "ColegioXYZ2025";
\`\`\`

### 2.3 Cargar Código en Wemos

1. Conecta Wemos por USB
2. Herramientas → Placa → LOLIN(WEMOS) D1 Mini (ESP8266)
3. Herramientas → Puerto → (selecciona tu puerto)
4. Herramientas → Upload Speed → 921600 (puede ser más lento)
5. Carga el código: Ctrl+U
6. Espera a que termine

### 2.4 Verificar Conexión WiFi

1. Abre Monitor Serial del Wemos
2. Velocidad: 115200 baud (¡diferente al Arduino!)
3. Deberías ver:
   \`\`\`
   Conectando a WiFi...
   WiFi conectado
   IP: 192.168.1.120
   Servidor iniciado
   \`\`\`

Si dice "WiFi no conectado":
- Verifica SSID y contraseña
- Verifica que WiFi esté disponible
- Intenta con un hotspot de celular para descartar
- Reinicia Wemos

---

## Paso 3: Conectar Arduino y Wemos en Serial

### 3.1 Conexiones Físicas

Asegúrate de haber conectado:

\`\`\`
Arduino TX (pin 1) ──→ Wemos RX (GPIO 3)
Arduino GND ────────→ Wemos GND
\`\`\`

### 3.2 Verificar Comunicación

1. Carga código en Arduino
2. Carga código en Wemos
3. Abre Monitor Serial de Arduino (9600 baud)
   - Deberías ver JSON cada 10 segundos
4. Abre Monitor Serial de Wemos (115200 baud)
   - Deberías ver que recibe datos del Arduino

---

## Paso 4: Acceder a la Página Web

### 4.1 Encontrar la IP

En Monitor Serial del Wemos:
\`\`\`
IP: 192.168.1.120
\`\`\`

Anota esta IP.

### 4.2 Abrir en Navegador

Desde cualquier PC conectada al mismo WiFi:

1. Abre navegador web (Chrome, Firefox, Edge)
2. En la barra de direcciones escribe:
   \`\`\`
   http://192.168.1.120
   \`\`\`
3. Presiona Enter

Deberías ver la página web con 4 tarjetas de datos.

### 4.3 Si No Funciona

**Opción 1: Verificar IP correcta**
- Abre Monitor Serial del Wemos
- Anota exactamente la IP que muestra

**Opción 2: Verificar conectividad**
- Desde CMD (Windows) o Terminal (Mac/Linux):
  \`\`\`
  ping 192.168.1.120
  \`\`\`
- Deberías recibir respuesta

**Opción 3: Usar IP local alternativa**
- Algunos routers muestran IPs diferentes
- Intenta: http://wemos.local/ (si el router lo soporta)

**Opción 4: Verificar firewall**
- Desactiva firewall temporalmente
- Intenta acceder de nuevo

---

## Paso 5: Optimizaciones

### 5.1 Cambiar Intervalo de Lectura

En `Arduino/estacion_meteorologica.ino`:

\`\`\`cpp
const unsigned long INTERVALO_LECTURA = 10000; // Milisegundos
\`\`\`

- 10000 = 10 segundos (recomendado)
- 5000 = 5 segundos (más rápido)
- 30000 = 30 segundos (menos datos)

### 5.2 Cambiar Velocidad de Actualización Web

En la sección HTML del Wemos, busca:

\`\`\`javascript
setInterval(actualizarDatos, 3000); // Milisegundos
\`\`\`

- 3000 = cada 3 segundos (recomendado)
- 1000 = cada 1 segundo (más responsivo)
- 10000 = cada 10 segundos (menos solicitudes)

### 5.3 Ajustar Presición del BMP280

En Arduino setup():

\`\`\`cpp
bmp.setSampling(
  Adafruit_BMP280::MODE_NORMAL,     // Más preciso
  Adafruit_BMP280::SAMPLING_X16,    // Aumentar número para mayor precisión
  Adafruit_BMP280::SAMPLING_X16,
  Adafruit_BMP280::FILTER_X16,
  Adafruit_BMP280::STANDBY_MS_500
);
\`\`\`

Opciones: SAMPLING_X1, X2, X4, X8, X16

---

## Paso 6: Pruebas Finales

Checklist antes de presentar en feria:

- [ ] Arduino muestra JSON en Monitor Serial
- [ ] Wemos se conecta a WiFi correctamente
- [ ] Página web se carga en navegador
- [ ] Datos se actualizan automáticamente
- [ ] Sensores dan valores lógicos:
  - Temperatura: 15-35°C
  - Humedad: 20-80%
  - Presión: 1000-1030 hPa
  - Luz: varía con iluminación

Si algo falla, revisa TROUBLESHOOTING.md.

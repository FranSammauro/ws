# Instalación de Librerías - Paso a Paso

## 1. Instalar Arduino IDE

### Windows/Mac/Linux
1. Ve a [arduino.cc](https://arduino.cc)
2. Descarga "Arduino IDE 2.x" (versión más reciente)
3. Ejecuta el instalador y sigue las instrucciones
4. Abre Arduino IDE

## 2. Configurar Placa Arduino UNO

1. Abre Arduino IDE
2. Herramientas → Gestor de placas
3. Busca "Arduino AVR Boards"
4. Haz clic en Instalar
5. Espera a que termine (2-3 minutos)

## 3. Instalar Librerías Necesarias

### Método recomendado: Gestor de Librerías

#### Librería DHT (Adafruit)

1. Herramientas → Gestionar librerías
2. Busca: "DHT sensor library"
3. Por Adafruit Industries
4. Haz clic en **Instalar**
5. Espera a que termine

#### Librería Adafruit BMP280

1. Herramientas → Gestionar librerías
2. Busca: "Adafruit BMP280"
3. Por Adafruit Industries
4. Haz clic en **Instalar**
   - Cuando pida instalar dependencias (Adafruit Sensor), acepta
5. Espera a que termine

#### Librería ArduinoJson

1. Herramientas → Gestionar librerías
2. Busca: "ArduinoJson"
3. Por Benoit Blanchon
4. Haz clic en **Instalar**
5. Espera a que termine

### Verificación de instalación

1. Archivo → Ejemplos
2. Deberías ver:
   - Examples from Custom Libraries → DHT sensor library
   - Examples from Custom Libraries → Adafruit BMP280
   - Examples from Custom Libraries → ArduinoJson

Si ves estas carpetas, ¡las librerías están instaladas correctamente!

## 4. Configurar Placa Wemos D1 Mini

### Agregar URL de placa ESP8266

1. Abre Arduino IDE
2. Archivo → Preferencias
3. En "URL adicionales de gestor de placas", agrega:
   \`\`\`
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   \`\`\`
4. Haz clic OK
5. Espera a que se actualice

### Instalar placa ESP8266

1. Herramientas → Gestor de placas
2. Busca: "ESP8266"
3. Por ESP8266 Community
4. Haz clic en **Instalar** (versión 3.0.2 o superior)
5. Espera a que termine (5-10 minutos, es largo)

### Verificar instalación de ESP8266

1. Herramientas → Placa
2. Deberías ver:
   - Generic ESP8266 Module
   - LOLIN(WEMOS) D1 Mini
   - Otras placas ESP8266

Si ves "LOLIN(WEMOS) D1 Mini", ¡está correcto!

## 5. Instalar Drivers (si es necesario)

### Para Wemos D1 Mini - Chip CH340

Si el Wemos no aparece en "Herramientas → Puerto", necesitas el driver:

**Windows:**
1. Descarga desde: https://github.com/nodemcu/nodemcu-devkit/tree/master/Drivers
2. Busca CH340 driver
3. Descomprime y ejecuta el instalador

**Mac:**
1. Descarga: https://github.com/Mellyanor/CH340-Mac/releases
2. Ejecuta el instalador .pkg

**Linux:**
\`\`\`bash
# Por lo general, el driver está incluido. Si no:
sudo usermod -a -G dialout $USER
# Desconecta/conecta el dispositivo USB
\`\`\`

### Para Arduino UNO (generalmente no necesario)

Si tu Arduino UNO no aparece, descarga los drivers desde arduino.cc

## 6. Verificar Conexión USB

### Windows
1. Conecta Arduino por USB
2. Panel de control → Dispositivos
3. Busca puerto COM (COM3, COM4, etc.)
4. En Arduino IDE: Herramientas → Puerto → Selecciona el puerto

### Mac
1. Conecta Arduino por USB
2. Herramientas → Puerto
3. Deberías ver algo como: /dev/cu.usbserial-xxxx

### Linux
1. Conecta Arduino por USB
2. Terminal:
   \`\`\`bash
   ls /dev/tty*
   \`\`\`
3. Busca ttyUSB0 o ttyACM0

## 7. Primer Test - Blink LED

Para verificar que todo funciona:

1. Archivo → Ejemplos → Built-in examples → Basics → Blink
2. Selecciona placa Arduino UNO (Herramientas → Placa)
3. Selecciona puerto correcto
4. Carga el código (Ctrl+U o botón Upload)
5. Si el LED parpadea en tu Arduino, ¡está todo bien!

## 8. Solución de Problemas de Instalación

### "No encuentro el puerto COM"
- Verifica que Arduino esté conectado por USB
- Intenta con otro cable USB (algunos son solo carga)
- Instala drivers CH340 (Wemos) si es necesario
- Reinicia Arduino IDE

### "Fallo al instalar librería"
- Verifica conexión a internet
- Intenta instalar de nuevo
- Si persiste, descarga .zip manualmente desde GitHub

### "Placa no aparece en Herramientas"
- Verifica que hayas agregado la URL correcta de ESP8266
- Espera a que termine la descarga (puede ser lento)
- Reinicia Arduino IDE

### "Error: no se puede acceder al puerto"
- Cierra Monitor Serial (si está abierto)
- Cierra otros programas que usen el puerto
- Reinicia Arduino IDE
- Desconecta/reconecta Arduino

## Versiones Recomendadas

- Arduino IDE: 2.0 o superior
- Arduino AVR Boards: 1.8.5 o superior
- ESP8266 Community: 3.0.2 o superior
- DHT library: 1.4.4 o superior
- Adafruit BMP280: 2.6.8 o superior
- ArduinoJson: 6.20.0 o superior

---

¡Una vez completados estos pasos, estás listo para cargar los códigos del proyecto!

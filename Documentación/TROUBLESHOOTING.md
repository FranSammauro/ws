# Solución de Problemas - FAQ

## Arduino

### Arduino no se carga

**Síntoma:** Error "Puerto no disponible" o "Failed to upload"

**Solución:**
1. Cierra Monitor Serial (si está abierto)
2. Verifica placa: Herramientas → Placa → Arduino UNO
3. Verifica puerto correcto
4. Intenta otro cable USB
5. Reinicia Arduino IDE

### Arduino muestra errores de compilación

**Síntoma:** "Error: 'DHT' was not declared"

**Solución:**
1. Verifica que instalaste todas las librerías:
   - DHT sensor library
   - Adafruit BMP280
   - ArduinoJson
2. Reinicia Arduino IDE después de instalar
3. En Herramientas → Gestionar librerías, busca cada una y verifica que dice "Instalar" ya no está activo

### No aparecen datos en Monitor Serial

**Síntoma:** Monitor Serial vacío después de 10 segundos

**Solución:**
1. Verifica velocidad Serial: 9600 baud (esquina inferior derecha)
2. Verifica conexiones de sensores
3. Abre Monitor Serial DESPUÉS de cargar código
4. Si dice "DHT22 fallo", verifica pin 2
5. Si dice "BMP280 no encontrado", verifica I2C (A4, A5)

### DHT22 "fallo" constante

**Síntoma:** Mensaje "DHT22 fallo" en Monitor Serial

**Solución:**
1. Verifica que DHT22 esté en pin 2
2. Revisa conexión del pin DATA
3. Prueba con capacitor 100nF entre DATA y GND
4. Verifica que sea DHT22 (no DHT11)
5. Si la placa es muy antigua, puede haber retrasos, aumenta `delay()` en lectura

### BMP280 "no encontrado"

**Síntoma:** JSON vacío o falta presión

**Solución:**
1. Verifica que BMP280 esté en I2C (A4 SDA, A5 SCL)
2. Prueba cambiar dirección I2C en código:
   \`\`\`cpp
   if (!bmp.begin(0x77)) { // Intenta 0x77 en lugar de 0x76
   \`\`\`
3. Agrega resistencias pull-up 10kΩ en SDA y SCL (opcional)
4. Verifica conexión a 3.3V (no 5V)

### LDR siempre muestra valor bajo

**Síntoma:** Siempre entre 0-50 aunque haya luz

**Solución:**
1. Verifica conexión del divisor resistivo
2. Invierte la posición: resistencia y LDR pueden estar invertidas
3. Cambia resistencia 10kΩ por 5kΩ si valores siguen bajos
4. Verifica que pin A0 esté en Arduino (no confundir con A4/A5)

---

## Wemos D1 Mini

### Wemos no se carga

**Síntoma:** "Failed to upload" o puerto no aparece

**Solución:**
1. Instala driver CH340 (ver INSTALACION_LIBRERÍAS.md)
2. Verifica placa: LOLIN(WEMOS) D1 Mini
3. Usa cable USB diferente (algunos solo cargan)
4. Desconecta/reconecta USB
5. Si sigue, intenta: Herramientas → Erase Flash → All Flash Contents

### Wemos no se conecta a WiFi

**Síntoma:** Monitor dice "WiFi no conectado" constantemente

**Soluciones en orden:**

1. **Verifica credenciales exactas:**
   \`\`\`cpp
   const char* ssid = "Red_Colegio";      // Sensible a mayúsculas
   const char* password = "Contraseña";   // Sensible a mayúsculas
   \`\`\`

2. **Prueba con hotspot de celular:**
   - Si funciona con hotspot pero no con WiFi del colegio
   - El router puede estar bloqueando ESP8266
   - Contacta con TI del colegio

3. **Verifica que sea 2.4 GHz:**
   - ESP8266 no soporta 5 GHz
   - Algunos routers emiten ambas frecuencias
   - Asegúrate de conectar a la banda 2.4 GHz

4. **Aumenta espera de conexión:**
   \`\`\`cpp
   int intentos = 0;
   while (WiFi.status() != WL_CONNECTED && intentos < 50) { // Aumenta 50
   \`\`\`

### No puedo acceder a la página web

**Síntoma:** Página no carga, "conexión rechazada"

**Checklist:**
1. [ ] Wemos dice "WiFi conectado" en Monitor Serial
2. [ ] Anotaste la IP correcta (ej: 192.168.1.120)
3. [ ] Tu PC está en la misma red WiFi que Wemos
4. [ ] Escribiste http:// (no https://)
5. [ ] Esperaste 5 segundos después de conectar Wemos

**Si sigue sin funcionar:**
1. Desactiva firewall temporalmente
2. Desde CMD: `ping 192.168.1.120` (debería responder)
3. Intenta: `http://wemos.local` (si router lo soporta)
4. Reinicia Wemos (desconecta USB 5 segundos)

### Página carga pero datos no aparecen

**Síntoma:** Página web se ve bien pero dice "--" en todos los valores

**Causa:** Arduino no envía datos por Serial

**Solución:**
1. Verifica que Arduino esté encendido
2. Abre Monitor Serial del Arduino (9600 baud)
   - Deberías ver JSON cada 10 segundos
3. Si no ves JSON, Arduino no funciona
4. Si ves JSON en Arduino pero Wemos no lo recibe:
   - Verifica conexión TX → RX
   - Verifica que GND esté compartido
   - Aumenta espera: `delay(1000)` en Arduino antes de enviar

---

## Comunicación Arduino-Wemos

### Arduino envía pero Wemos no recibe

**Síntoma:** 
- Arduino Monitor Serial muestra JSON
- Wemos Monitor Serial no muestra que recibió datos

**Causa:** Cable Serial desconectado

**Solución:**
1. Verifica Cable TX → RX físicamente
2. Verifica GND compartido (muy importante)
3. Intenta invertir: TX → RX y RX → TX (algunos cables están invertidos)
4. Abre Monitor Serial de Wemos a 9600 (no 115200)
   - Deberías ver basura si hay comunicación pero velocidad incorrecta

---

## Sensores

### Temperatura muy baja/alta

**Si siempre está en 0°C:**
- Verifica conexión DHT22
- Prueba reiniciar Arduino

**Si oscila mucho:**
- Espera 2-3 minutos (DHT22 necesita tiempo)
- Revisa que no haya fuentes de calor/frío cerca

### Humedad siempre 0% o 100%

**Síntoma:** Valor fijo o extremo

**Solución:**
1. Espera 2 minutos (DHT22 es lento)
2. Verifica conexión del pin DATA
3. Proximidad a agua o fuentes de calor afectan medición

### Presión irreal

**Si muestra "0" o valores negativos:**
1. Verifica conexión I2C (A4, A5)
2. Cambia dirección I2C en código (0x76 o 0x77)

**Si muestra valores normales pero inmóviles:**
- Es normal si el BMP280 está kalibrando
- Espera 5 minutos

### Luz siempre en rango bajo

**Si solo varía entre 0-100:**
1. Invierte LDR y resistencia en el divisor
2. O cambia resistencia de 10kΩ a 5kΩ

---

## Página Web

### Página carga lentamente

**Solución:**
- Está intentando cargar sin conexión del Wemos
- Verifica que Wemos esté alimentado
- Abre Inspector (F12) → Consola para ver errores

### Los datos se actualizan lentamente

**Si se actualiza cada 30+ segundos:**
1. Verifica conexión Arduino-Wemos
2. En Wemos, reduce velocidad actualización:
   \`\`\`javascript
   setInterval(actualizarDatos, 1000); // Cada 1 segundo
   \`\`\`

### Página no tiene estilos (se ve fea)

**Solución:**
- Contenido está pero CSS no carga
- Problema de compatibilidad del navegador
- Intenta: Chrome, Firefox, Edge (no Safari viejo)

---

## General

### Proyecto se reinicia constantemente

**Síntoma:** Wemos se desconecta cada 1-2 minutos

**Causa:** Fuente de poder insuficiente

**Solución:**
1. Usa micro-USB de al menos 1A
2. No alimentes sensores desde Wemos directamente
3. Alimenta desde Arduino que tiene más potencia

### Muchos errores aleatorios

**Síntoma:** Datos inconsistentes, desconexiones

**Causa:** Ruido en conexiones

**Solución:**
1. Revisa todas las conexiones
2. Agrega capacitores 100nF cerca de sensores
3. Usa cables más cortos
4. Evita cables cerca de fuentes de energía

---

## Si Nada Funciona

Checklist final:

1. [ ] ¿Instalaste todas las librerías? (INSTALACION_LIBRERÍAS.md)
2. [ ] ¿Cargaste el código en Arduino y Wemos?
3. [ ] ¿Verificaste pines y conexiones? (CONEXIONES.md)
4. [ ] ¿Arduino muestra JSON en Monitor Serial?
5. [ ] ¿Wemos se conecta a WiFi?
6. [ ] ¿Puedes hacer ping a la IP del Wemos?

Si pasaste todos:
- Reinicia todo (apaga 1 minuto)
- Intenta nuevamente desde el principio
- Contacta con profesor o tutor del proyecto

---

**Última opción:** Sube fotos de conexiones a:
- GitHub issue del proyecto
- O muestra a tu profesor para revisar

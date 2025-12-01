// =============================================================================
// ESTACIÓN METEOROLÓGICA - CÓDIGO WEMOS D1 MINI (ESP8266)
// =============================================================================
// Recibe datos JSON por Serial del Arduino
// Sirve página web HTTP con datos en tiempo real
// Accesible desde cualquier dispositivo en la red local
// =============================================================================

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// ========== CREDENCIALES WiFi ==========
// CAMBIAR ESTOS VALORES A TUS CREDENCIALES DEL COLEGIO
const char* ssid = "RED_COLEGIO";        // Nombre exacto del WiFi
const char* password = "CONTRASEÑA";     // Contraseña (sensible a mayúsculas)

// ========== CONFIGURACIÓN SERIAL ==========
const int BAUD_RATE = 9600;              // Velocidad serial con Arduino
const int BUFFER_SIZE = 256;             // Tamaño máximo JSON
char serialBuffer[BUFFER_SIZE];
int bufferIndex = 0;

// ========== SERVIDOR WEB ==========
ESP8266WebServer server(80);

// ========== VARIABLES DE DATOS ==========
// Almacenan los últimos valores recibidos del Arduino
float temperatura = 0.0;
float humedad = 0.0;
int presion = 0;
int luz = 0;
unsigned long ultimaActualizacion = 0;

// =============================================================================
// SETUP - Inicialización
// =============================================================================
void setup() {
  // Serial a 115200 para Debug por USB
  Serial.begin(115200);
  delay(1000);
  
  // Serial1 a 9600 para comunicación con Arduino
  // En Wemos, Serial utiliza GPIO1/GPIO3, Serial1 es alternativo
  Serial.begin(9600, SERIAL_8N1, 3, 1); // RX=3, TX=1 (pines por defecto)
  delay(500);
  
  // Conectar a WiFi
  conectarWiFi();
  
  // Configurar rutas del servidor web
  server.on("/", HTTP_GET, manejadorPrincipal);
  server.on("/datos", HTTP_GET, manejadorDatos);
  server.onNotFound(manejadorNoEncontrado);
  
  // Iniciar servidor HTTP
  server.begin();
  
  delay(500);
}

// =============================================================================
// LOOP PRINCIPAL
// =============================================================================
void loop() {
  // Procesar solicitudes HTTP entrantes
  server.handleClient();
  
  // Leer datos del Arduino por Serial
  if (Serial.available()) {
    char c = Serial.read();
    
    if (c == '\n') {
      // Fin de línea - procesar JSON completo
      serialBuffer[bufferIndex] = '\0';
      procesarJSON(serialBuffer);
      bufferIndex = 0;
    } else if (c != '\r') {
      // Agregar carácter al buffer (ignorar \r de fin de línea)
      if (bufferIndex < BUFFER_SIZE - 1) {
        serialBuffer[bufferIndex++] = c;
      }
    }
  }
  
  // Pequeño delay para evitar saturar CPU
  delay(10);
}

// =============================================================================
// FUNCIÓN: Conectar a WiFi
// =============================================================================
void conectarWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 20) {
    delay(500);
    intentos++;
  }
}

// =============================================================================
// FUNCIÓN: Procesar JSON del Arduino
// =============================================================================
// Deserializa JSON recibido y actualiza variables globales
// =============================================================================
void procesarJSON(char* jsonString) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, jsonString);
  
  if (error) {
    // JSON inválido, ignorar
    return;
  }
  
  // Extraer valores del JSON
  if (doc.containsKey("temp")) {
    temperatura = doc["temp"].as<float>();
  }
  if (doc.containsKey("hum")) {
    humedad = doc["hum"].as<float>();
  }
  if (doc.containsKey("pres")) {
    presion = doc["pres"].as<int>();
  }
  if (doc.containsKey("luz")) {
    luz = doc["luz"].as<int>();
  }
  
  // Actualizar timestamp
  ultimaActualizacion = millis();
}

// =============================================================================
// MANEJADOR: Página Principal (GET /)
// =============================================================================
// Retorna página HTML completa con CSS y JavaScript integrados
// =============================================================================
void manejadorPrincipal() {
  String html = R"(
<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Estación Meteorológica</title>
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }
    
    body {
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, sans-serif;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      min-height: 100vh;
      display: flex;
      align-items: center;
      justify-content: center;
      padding: 20px;
    }
    
    .contenedor {
      width: 100%;
      max-width: 900px;
    }
    
    .encabezado {
      text-align: center;
      color: white;
      margin-bottom: 40px;
    }
    
    .encabezado h1 {
      font-size: 2.5em;
      margin-bottom: 10px;
      font-weight: 700;
    }
    
    .encabezado p {
      font-size: 1.1em;
      opacity: 0.9;
    }
    
    .estado-conexion {
      display: inline-block;
      background: rgba(255,255,255,0.2);
      padding: 8px 16px;
      border-radius: 20px;
      color: white;
      font-size: 0.9em;
      margin-top: 15px;
      backdrop-filter: blur(10px);
    }
    
    .estado-conexion.activo::before {
      content: "●";
      color: #4ade80;
      margin-right: 8px;
    }
    
    .estado-conexion.inactivo::before {
      content: "●";
      color: #f87171;
      margin-right: 8px;
    }
    
    .grid-datos {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
      gap: 20px;
      margin-bottom: 30px;
    }
    
    .tarjeta {
      background: white;
      border-radius: 15px;
      padding: 25px;
      box-shadow: 0 10px 40px rgba(0,0,0,0.2);
      text-align: center;
      transform: translateY(0);
      transition: all 0.3s ease;
      animation: fadeIn 0.6s ease-in-out;
    }
    
    .tarjeta:hover {
      transform: translateY(-5px);
      box-shadow: 0 15px 50px rgba(0,0,0,0.3);
    }
    
    @keyframes fadeIn {
      from {
        opacity: 0;
        transform: translateY(20px);
      }
      to {
        opacity: 1;
        transform: translateY(0);
      }
    }
    
    .tarjeta-icon {
      font-size: 2.5em;
      margin-bottom: 10px;
    }
    
    .tarjeta-label {
      color: #666;
      font-size: 0.9em;
      text-transform: uppercase;
      letter-spacing: 1px;
      margin-bottom: 8px;
      font-weight: 600;
    }
    
    .tarjeta-valor {
      font-size: 2.2em;
      font-weight: 700;
      color: #333;
      font-family: 'Monaco', 'Courier New', monospace;
    }
    
    .tarjeta-unidad {
      font-size: 0.7em;
      color: #999;
      margin-left: 5px;
    }
    
    .tarjeta.temp {
      border-top: 4px solid #ff6b6b;
    }
    
    .tarjeta.hum {
      border-top: 4px solid #4dabf7;
    }
    
    .tarjeta.pres {
      border-top: 4px solid #ffd43b;
    }
    
    .tarjeta.luz {
      border-top: 4px solid #ffa94d;
    }
    
    .pie-pagina {
      text-align: center;
      color: rgba(255,255,255,0.8);
      font-size: 0.9em;
      margin-top: 30px;
    }
    
    @media (max-width: 600px) {
      .encabezado h1 {
        font-size: 1.8em;
      }
      
      .grid-datos {
        grid-template-columns: 1fr;
      }
      
      .tarjeta-valor {
        font-size: 1.8em;
      }
    }
  </style>
</head>
<body>
  <div class="contenedor">
    <div class="encabezado">
      <h1>Estación Meteorológica</h1>
      <p>Datos en tiempo real del colegio</p>
      <div class="estado-conexion activo" id="estadoConexion">
        Conectado
      </div>
    </div>
    
    <div class="grid-datos">
      <div class="tarjeta temp">
        <div class="tarjeta-icon">🌡️</div>
        <div class="tarjeta-label">Temperatura</div>
        <div class="tarjeta-valor">
          <span id="temp">--</span>
          <span class="tarjeta-unidad">°C</span>
        </div>
      </div>
      
      <div class="tarjeta hum">
        <div class="tarjeta-icon">💧</div>
        <div class="tarjeta-label">Humedad</div>
        <div class="tarjeta-valor">
          <span id="hum">--</span>
          <span class="tarjeta-unidad">%</span>
        </div>
      </div>
      
      <div class="tarjeta pres">
        <div class="tarjeta-icon">🔽</div>
        <div class="tarjeta-label">Presión</div>
        <div class="tarjeta-valor">
          <span id="pres">--</span>
          <span class="tarjeta-unidad">hPa</span>
        </div>
      </div>
      
      <div class="tarjeta luz">
        <div class="tarjeta-icon">☀️</div>
        <div class="tarjeta-label">Luminosidad</div>
        <div class="tarjeta-valor">
          <span id="luz">--</span>
          <span class="tarjeta-unidad">/1023</span>
        </div>
      </div>
    </div>
    
    <div class="pie-pagina">
      <p>Última actualización: <span id="hora">--:--:--</span></p>
      <p>Proyecto de Feria de Ciencias 2025</p>
    </div>
  </div>
  
  <script>
    // Función para obtener datos del servidor
    async function actualizarDatos() {
      try {
        const response = await fetch('/datos');
        const data = await response.json();
        
        // Actualizar valores en la página
        document.getElementById('temp').textContent = data.temp.toFixed(1);
        document.getElementById('hum').textContent = data.hum;
        document.getElementById('pres').textContent = data.pres;
        document.getElementById('luz').textContent = data.luz;
        
        // Actualizar hora de actualización
        const ahora = new Date();
        document.getElementById('hora').textContent = 
          String(ahora.getHours()).padStart(2, '0') + ':' +
          String(ahora.getMinutes()).padStart(2, '0') + ':' +
          String(ahora.getSeconds()).padStart(2, '0');
        
        // Marcar como conectado
        document.getElementById('estadoConexion').classList.remove('inactivo');
        document.getElementById('estadoConexion').classList.add('activo');
        document.getElementById('estadoConexion').textContent = 'Conectado';
        
      } catch (error) {
        // Error de conexión
        document.getElementById('estadoConexion').classList.remove('activo');
        document.getElementById('estadoConexion').classList.add('inactivo');
        document.getElementById('estadoConexion').textContent = 'Sin conexión';
      }
    }
    
    // Actualizar datos cada 3 segundos
    setInterval(actualizarDatos, 3000);
    
    // Primer update al cargar la página
    actualizarDatos();
  </script>
</body>
</html>
  )";
  
  server.send(200, "text/html; charset=utf-8", html);
}

// =============================================================================
// MANEJADOR: API JSON (GET /datos)
// =============================================================================
// Retorna JSON con los últimos valores medidos
// =============================================================================
void manejadorDatos() {
  StaticJsonDocument<200> doc;
  doc["temp"] = temperatura;
  doc["hum"] = humedad;
  doc["pres"] = presion;
  doc["luz"] = luz;
  doc["timestamp"] = ultimaActualizacion;
  
  String jsonResponse;
  serializeJson(doc, jsonResponse);
  
  server.send(200, "application/json", jsonResponse);
}

// =============================================================================
// MANEJADOR: Error 404
// =============================================================================
void manejadorNoEncontrado() {
  server.send(404, "text/plain", "No encontrado");
}

// =============================================================================
// FIN DEL CÓDIGO
// =============================================================================

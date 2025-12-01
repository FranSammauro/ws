// =============================================================================
// ESTACIÓN METEOROLÓGICA - CÓDIGO ARDUINO UNO R3
// =============================================================================
// Lee sensores: DHT22, BMP280, LDR
// Envía datos por Serial al Wemos D1 Mini en formato JSON
// =============================================================================

#include <DHT.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <ArduinoJson.h>

// ========== CONFIGURACIÓN DE SENSORES ==========
#define DHT_PIN 2           // DHT22 conectado a pin digital 2
#define DHT_TYPE DHT22      // Tipo de sensor DHT22
#define LDR_PIN A0          // LDR conectado a pin analógico A0
#define LUM_RESISTENCIA 10000 // Resistencia de 10kΩ en divisor resistivo

// ========== INSTANCIAS DE SENSORES ==========
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP280 bmp;

// ========== VARIABLES GLOBALES ==========
unsigned long ultimaLectura = 0;
const unsigned long INTERVALO_LECTURA = 10000; // 10 segundos entre lecturas

// =============================================================================
// SETUP - Inicialización
// =============================================================================
void setup() {
  // Inicializar comunicación Serial a 9600 baud para Wemos
  Serial.begin(9600);
  delay(1000);
  
  // Inicializar sensor DHT22
  dht.begin();
  
  // Inicializar sensor BMP280 por I2C (A4=SDA, A5=SCL)
  if (!bmp.begin(0x76)) {
    // Si no encuentra BMP280, enviar error pero continuar
    Serial.println("{\"error\":\"BMP280 no encontrado\"}");
  }
  
  // Configurar resolución y muestreo del BMP280
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Modo normal
                  Adafruit_BMP280::SAMPLING_X2,     // Muestreo temp 2x
                  Adafruit_BMP280::SAMPLING_X16,    // Muestreo presión 16x
                  Adafruit_BMP280::FILTER_X16,      // Filtro 16x
                  Adafruit_BMP280::STANDBY_MS_500); // Standby 500ms
  
  delay(500);
}

// =============================================================================
// LOOP PRINCIPAL
// =============================================================================
void loop() {
  unsigned long tiempoActual = millis();
  
  // Leer sensores cada 10 segundos (configurable en INTERVALO_LECTURA)
  if (tiempoActual - ultimaLectura >= INTERVALO_LECTURA) {
    ultimaLectura = tiempoActual;
    
    // Leer valores de todos los sensores
    float temperatura = dht.readTemperature();
    float humedad = dht.readHumidity();
    float presion = bmp.readPressure() / 100.0; // Convertir Pa a hPa
    int luminosidad = leerLuminosidad();
    
    // Verificar si las lecturas del DHT son válidas
    if (isnan(temperatura) || isnan(humedad)) {
      Serial.println("{\"error\":\"DHT22 fallo\"}");
      delay(1000);
      return; // Reintentar en próxima lectura
    }
    
    // Crear documento JSON con los datos
    StaticJsonDocument<200> doc;
    doc["temp"] = round(temperatura * 10) / 10.0; // Redondear a 1 decimal
    doc["hum"] = (int)humedad;                     // Humedad como entero
    doc["pres"] = (int)presion;                    // Presión como entero en hPa
    doc["luz"] = luminosidad;                      // Luminosidad 0-1023
    
    // Serializar JSON y enviar por Serial al Wemos
    serializeJson(doc, Serial);
    Serial.println(); // Agregar salto de línea para separar mensajes
  }
}

// =============================================================================
// FUNCIÓN: Leer Luminosidad
// =============================================================================
// Lee el LDR (fotoresistencia) y retorna promedio de 5 lecturas
// Esto estabiliza la medición y reduce ruido
// =============================================================================
int leerLuminosidad() {
  int valoresLDR[5];
  int promedio = 0;
  
  // Tomar 5 lecturas con pequeño delay entre ellas
  for (int i = 0; i < 5; i++) {
    valoresLDR[i] = analogRead(LDR_PIN);
    delay(10);
  }
  
  // Sumar todas las lecturas
  for (int i = 0; i < 5; i++) {
    promedio += valoresLDR[i];
  }
  
  // Retornar promedio
  return promedio / 5;
}

// =============================================================================
// FIN DEL CÓDIGO
// =============================================================================

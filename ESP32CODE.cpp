#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#define LED 2

// Internet credentials  
const char* ssid = "---";
const char* password = "---";

// Web Server PORT
AsyncWebServer server(80);
String mensajeRecibido = "Esperando mensaje...";

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  // IP credentials
  Serial.println("Conectado a WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // SERVER ROBLOX Request
  server.on("/enviar", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      if (request->contentType() == "application/json") {
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, (const char*)data);
        mensajeRecibido = doc["mensaje"].as<String>();

        if (mensajeRecibido == "encender") {
          digitalWrite(LED, HIGH); 
          request->send(200, "text/plain", "LED encendido");
        } else if (mensajeRecibido == "apagar") {
          digitalWrite(LED, LOW); 
          request->send(200, "text/plain", "LED apagado");
        } else {
          request->send(200, "text/plain", "Mensaje no reconocido");
        }
      } else {
        request->send(200, "text/plain", "Formato incorrecto");
      }
    }
  );

  //Server Interface
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><body><h1>Mensaje Recibido:</h1><p>" + mensajeRecibido + "</p></body></html>";
    request->send(200, "text/html", html);
  });

  server.begin();
}

void loop() {

}
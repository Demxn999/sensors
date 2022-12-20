//#include "pins_arduino.h"
#include <ESP8266WebServer.h>
#include "Sensor.h"

ESP8266WebServer server(80);

void handle_root(){
  String form = "<form action=\"/LED\" method=\"POST\">";
  form += "<input type=\"submit\" value=\"Toogle LED\">";
  form += "</form>";
  server.send(200, "text/html", form);
}

void handleLED(){
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleSensor(){
  int data = get_range();
  server.send(200, "text/html", String(data));
}

void handleNotFound(){
  server.send(404, "text/html", "NOT FOUND!");
}
void server_init(){
  sensor_init();
  server.on("/", HTTP_GET, handle_root);
  server.on("/LED", HTTP_POST, handleLED);
  server.on("/SENSOR", HTTP_GET, handleSensor);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Server started at port 80");
}
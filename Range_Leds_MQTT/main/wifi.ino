#include "WIFI.h"
#include "Server.h"
#include "MQTT.h"

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi_init(false);
  server_init();
  MQTT_init();
  mqtt_cli.publish((mqtt_client_id + "/state").c_str(), "hello");
  mqtt_cli.subscribe((mqtt_client_id + "/state").c_str());
}

void loop() {
  server.handleClient();
  mqtt_cli.loop();
  float range = get_range();
  mqtt_cli.publish((mqtt_client_id + "/range").c_str(), String(range).c_str());
  delay(5000);
}

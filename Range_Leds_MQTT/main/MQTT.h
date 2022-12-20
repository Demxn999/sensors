#include <PubSubClient.h>

PubSubClient mqtt_cli(network_client);

String mqtt_broker = "broker.emqx.io";
int mqtt_port = 1883;
String mqtt_client_id = "esp8266" + id();
char* data;

void callback(char* topic, byte* payload, unsigned int length){
  Serial.println("Message arrived on ");
  Serial.println(topic);
  for (int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("---------------");
}


void MQTT_init(){
  mqtt_cli.setServer(mqtt_broker.c_str(), mqtt_port);
  mqtt_cli.setCallback(callback);
  while(!mqtt_cli.connected()){
    Serial.print(mqtt_client_id);
    Serial.print(" connecting...");
    if (mqtt_cli.connect(mqtt_client_id.c_str())){
      Serial.println(" MQTT Connected!");
    }
    else{
      Serial.print("Failed to connect");
      Serial.println(mqtt_cli.state());
      delay(1000);
    }
  }
}
#include <PubSubClient.h>
#include <WiFi.h>

extern WiFiClient espClient;
extern PubSubClient client;

void mqttPublish(const char* topic, const char* message) {
  if (client.publish(topic, message)) {
    Serial.println("Message Published");
  } else {
    Serial.println("Failed to publish message");
  }
}

void mqttSubscribe(const char* topic) {
  client.subscribe(topic);
}

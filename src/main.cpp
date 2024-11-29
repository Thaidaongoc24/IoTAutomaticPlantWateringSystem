#include <WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"

// Đối tượng WiFi và MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Hàm kết nối Wi-Fi
void setup_wifi() {
  delay(10);
  // Kết nối Wi-Fi
  Serial.println();
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

// Hàm kết nối MQTT
void reconnect() {
  // Nếu chưa kết nối với MQTT, cố gắng kết nối
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      // Đăng ký topic để nhận dữ liệu
      client.subscribe("topic/test");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

// Hàm callback khi nhận tin nhắn MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  // Khởi tạo Serial
  Serial.begin(115200);
  setup_wifi();
  
  // Khởi tạo MQTT
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
}

void loop() {
  // Nếu không kết nối MQTT, hãy thử kết nối lại
  if (!client.connected()) {
    reconnect();
  }
  // Xử lý tin nhắn MQTT
  client.loop();
}

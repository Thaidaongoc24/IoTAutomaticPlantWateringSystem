#include <Arduino.h>
#include "wifi_connect.h"
#include <WiFiClientSecure.h>
#include "secrets.h"
#include <PubSubClient.h>
#include "MQTT.h"
#include <Ticker.h>
#include "ca_cert_hivemq.h"

// Chân kết nối cảm biến và bơm
#define SOIL_MOISTURE_PIN 2U // Chân ADC đọc cảm biến độ ẩm đất
#define PUMP_PIN 15U         // Chân điều khiển bơm

// Ngưỡng mặc định (có thể thay đổi từ Node-RED)
int soilMoistureThreshold = 60; // Đổi ngưỡng thành phần trăm (60%)


// MQTT Topics
namespace MQTT_Topics {
    const char* soil_moisture_topic = "plant/soil_moisture";
    const char* pump_control_topic = "plant/pump_control";
    const char* threshold_topic = "plant/soil_moisture_threshold";
}

// Wi-Fi và MQTT
namespace {
    const char* ssid = WiFiSecrets::ssid;
    const char* password = WiFiSecrets::pass;
    const char* client_id = (String("esp32-client-") + WiFi.macAddress()).c_str();

    WiFiClientSecure tlsClient;
    PubSubClient mqttClient(tlsClient);

    Ticker soilMoistureTicker;
    bool isPumpOn = false;
}

// Hàm điều khiển bơm
void controlPump(bool state) {
    if (isPumpOn != state) {
        isPumpOn = state;
        digitalWrite(PUMP_PIN, isPumpOn ? HIGH : LOW);
        Serial.println(isPumpOn ? "Pump ON" : "Pump OFF");
    }
}

// Hàm đọc cảm biến độ ẩm và gửi dữ liệu qua MQTT
void readAndPublishSoilMoisture() {
    int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);

    // Chuyển giá trị ADC sang phần trăm độ ẩm
    int soilMoisturePercentage = map(soilMoistureValue, 0, 4095, 0, 100);  

    Serial.print("Soil Moisture: ");
    Serial.print(soilMoisturePercentage);
    Serial.println("%");

    // Gửi giá trị độ ẩm đất qua MQTT
    mqttClient.publish(MQTT_Topics::soil_moisture_topic, String(soilMoisturePercentage).c_str(), true);

    // Kiểm tra ngưỡng và tự động bật/tắt bơm
    if (soilMoisturePercentage < soilMoistureThreshold) {
        if (!isPumpOn) {
            Serial.println("Soil is dry, turning pump ON");
            controlPump(true);
        }
    } else {
        if (isPumpOn) {
            Serial.println("Soil is moist, turning pump OFF");
            controlPump(false);
        }
    }
}

// Callback xử lý lệnh MQTT
void mqttCallback(char* topic, uint8_t* payload, unsigned int length) {
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';

    if (strcmp(topic, MQTT_Topics::pump_control_topic) == 0) {
        Serial.print("Received pump control: ");
        Serial.println(message);

        if (strcmp(message, "ON") == 0) {
            controlPump(true);
        } else if (strcmp(message, "OFF") == 0) {
            controlPump(false);
        }
    } else if (strcmp(topic, MQTT_Topics::threshold_topic) == 0) {
        Serial.print("Received new threshold: ");
        Serial.println(message);
        soilMoistureThreshold = atoi(message); // Cập nhật ngưỡng từ MQTT
    }
}

void setup() {
    Serial.begin(115200);
    delay(10);

    // Kết nối Wi-Fi
    setup_wifi(ssid, password);

    // Cấu hình MQTT
    tlsClient.setCACert(ca_cert);
    mqttClient.setServer(HiveMQ::broker, HiveMQ::port);
    mqttClient.setCallback(mqttCallback);

    // Cấu hình chân điều khiển bơm
    pinMode(PUMP_PIN, OUTPUT);
    controlPump(false); // Bắt đầu với bơm tắt

    // Đọc độ ẩm định kỳ
    soilMoistureTicker.attach(1, readAndPublishSoilMoisture); // 1 giây đọc một lần
}

void loop() {
    // Gọi hàm reconnect với từng topic
    MQTT::reconnect(mqttClient, client_id, HiveMQ::username, HiveMQ::password, MQTT_Topics::pump_control_topic);
    MQTT::reconnect(mqttClient, client_id, HiveMQ::username, HiveMQ::password, MQTT_Topics::threshold_topic);
    
    // Vòng lặp MQTT
    mqttClient.loop();
    delay(10);
}

#include <Arduino.h>
#include <WiFi.h> 
#include <WiFiClientSecure.h> 
#include <MQTT.h> 

// 1. WiFi indstillinger
const char* ssid = "Alex";
const char* password = "hejmeddig123";

// 2. HiveMQ Cloud
#define mqttServer "e485e36576474f2980cbef4972a4cd36.s1.eu.hivemq.cloud"
#define mqttPort 8883 

// 3. Credentials
const char* mqttClientId = "AU_AMS_PSKIVE"; 
const char* mqttUsername = "pskive_bruger"; 
const char* mqttPassword = "Lommetyv67"; 

WiFiClientSecure wifiSecure;
MQTTClient mqtt;

const uint8_t ledPin = 2; 
#define PSKIVE_SET_TIME_TOPIC "AU_AMS_PSKIVE/pskive/set_time"
#define PSKIVE_STATUS_TOPIC "AU_AMS_PSKIVE/status"

unsigned long lastMillis = 0;

void msgRecv(String &topic, String &payload) {
    Serial.println("JEG HØRTE NOGET!");
    Serial.println("Topic: " + topic);
    Serial.println("Payload: " + payload);
}

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);

    WiFi.begin(ssid, password);
    Serial.print("Forbinder til WiFi");
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi OK!");

    wifiSecure.setInsecure(); 
    mqtt.begin(mqttServer, mqttPort, wifiSecure);
    mqtt.onMessage(msgRecv);

    Serial.print("Forbinder til HiveMQ...");
    while(!mqtt.connect(mqttClientId, mqttUsername, mqttPassword)) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nSUCCESS: FORBUNDET TIL SKYEN!");

    mqtt.subscribe("AU_AMS_PSKIVE/#");
    Serial.println("Lytter på: " + String(PSKIVE_SET_TIME_TOPIC));
}

void loop() {
    // 1. Tjek om vi stadig er forbundet til HiveMQ
    if (!mqtt.connected()) {
        Serial.println("Forbindelse tabt! Prøver at genforbinde...");
        
        // Prøv at forbinde igen
        while (!mqtt.connect(mqttClientId, mqttUsername, mqttPassword)) {
            delay(500);
            Serial.print(".");
        }
        
        // Husk at subscribe igen efter genforbindelse!
        mqtt.subscribe("AU_AMS_PSKIVE/#");
        Serial.println("\nGenforbundet!");
    }

    mqtt.loop();

    // Jeres eksisterende status-besked hver 5. sekund
    if (millis() - lastMillis > 5000) {
        lastMillis = millis();
        mqtt.publish(PSKIVE_STATUS_TOPIC, "ESP32 kører stadig");
    }
}
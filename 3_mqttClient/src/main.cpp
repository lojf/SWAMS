#include <Arduino.h>
#include <WiFi.h> 
#include <WiFiClientSecure.h> 
#include <MQTT.h> 
#include "accelerometer.h"

// 1. WiFi indstillinger
const char* ssid = "Yeet";
const char* password = "Tissemand";

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

#define STILL_TIME_MS  15000
#define CHECK_INTERVAL 3000

unsigned long lastCheck = 0;
int still_count = 0;
bool time_requested = false;


void msgRecv(String &topic, String &payload) {
    Serial.println("JEG HØRTE NOGET!");
    Serial.println("Topic: " + topic);
    Serial.println("Payload: " + payload);

    if (topic == PSKIVE_SET_TIME_TOPIC) {
        Serial2.print(payload);
        time_requested = false; // Reset flag efter at have sendt tidsanmodning
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);

    //accelerometer og kommunikation til arduino
    setupAccelerometer();
    Serial2.begin(9600);

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

    mqtt.subscribe("AU_AMS_PSKIVE/pskive/set_time");
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
        mqtt.subscribe("AU_AMS_PSKIVE/pskive/set_time");
        Serial.println("\nGenforbundet!");
    }
    
    mqtt.loop();


    //accelerometer tjek - added af Mickey
    if (millis() - lastCheck >= CHECK_INTERVAL) { //tjek om der er gået lang nok tid
        lastCheck = millis();

        if (isStill()) {
            still_count++;
            Serial.println("Still count: " + String(still_count));
            
            if(still_count >= (STILL_TIME_MS / CHECK_INTERVAL) && !time_requested) {
                mqtt.publish("AU_AMS_PSKIVE/pskive/request_time", "REQUEST");
                Serial.println("Sendte tidsrequest!");
                time_requested = true;
            }
        } else {
            still_count = 0; // Reset count hvis der er bevægelse
            Serial.println("Bevægelse registreret, reset still count.");
            time_requested = false; // Reset tidsanmodningsflag ved bevægelse
        }

    }



}
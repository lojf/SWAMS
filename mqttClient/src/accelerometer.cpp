#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>
#include <Wire.h>
#include "accelerometer.h"

Adafruit_ADXL343 accel = Adafruit_ADXL343(42); //42 fordi den skal have et sensorID (se implementation), så det blev bare 42

static float x, y, z = 0;

void setupAccelerometer()
{
    Wire.begin();
    if (!accel.begin()) {
        Serial.println("Fejl ved initialisering af accelerometer!");
        while (1);
    }

    accel.setRange(ADXL343_RANGE_2_G);
    Serial.println("Accelerometer initialiseret!");
}

bool isStill()
{
    sensors_event_t event;
    accel.getEvent(&event);

    float dx = abs(x - event.acceleration.x);
    float dy = abs(y - event.acceleration.y);
    float dz = abs(z - event.acceleration.z);

    x = event.acceleration.x;
    y = event.acceleration.y;
    z = event.acceleration.z;

    float threshold = 0.2; // threshold kan indstilles

    if (dx < threshold && dy < threshold && dz < threshold) {
        return true; // pskiven/bilen er stille
    }

    return false;
}
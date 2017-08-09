#include <algorithm>
#include <vector>

#include <cstring>

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <HX711.h>
#include <PubSubClient.h>

#include "config.hpp"

using namespace std;

const uint32_t CHIP_ID = ESP.getChipId();

WiFiClient wifi_client;
PubSubClient mqtt_client(MQTT_HOST, 1883, wifi_client);

HX711* scale;
vector<long> last_measurements;

long compute_median(vector<long> values) {
    nth_element(values.begin(), values.begin() + (M_SAMPLES / 2), values.end());
    return values[M_SAMPLES / 2];
}

long send_value(long value) {
    // Format JSON
    DynamicJsonBuffer json_buffer;
    JsonObject& root = json_buffer.createObject();
    root["esp_id"] = CHIP_ID;
    root["scale_value"] = value;
    String json_string;
    root.printTo(json_string);

    // Send MQTT message
    Serial.printf("\nPublishing to MQTT broker: %s...", json_string.c_str());
    mqtt_client.publish(MQTT_TOPIC, json_string.c_str());
    Serial.printf(" success.\n");
}

int wifi_connect() {
    Serial.println("Connecting to WiFi.");
    WiFi.begin(SSID, PASSWORD);

    int fails = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        fails++;
        if (fails > 60) {
            Serial.println("\nConnection timed out.\n");
            return 0;
        }
    }
    return 1;
}

void setup() {
    delay(1000);

    last_measurements.reserve(M_SAMPLES);

    // Initialize serial
    Serial.begin(115200);
    Serial.println("\n");

    // Initialize HX711 ADC module
    Serial.println("Initializing HX711 ADC module...");
    scale = new HX711(DATA_PIN, CLOCK_PIN);

    // Initialize WiFi connection
    while (!wifi_connect()) {
        continue;
    }

    Serial.printf("Connected to: %s with IP address %s\n",
            SSID,
            WiFi.localIP().toString().c_str());

    char client_id[9];
    snprintf(client_id, 8, "%X", CHIP_ID);

    mqtt_client.connect(client_id);

    Serial.println("Connected to MQTT broker.");
    Serial.println("Setup complete!\n");
}

void loop() {
    long measurement = scale->read();
    last_measurements.push_back(measurement);
    if (last_measurements.size() > M_SAMPLES) {
        // Compute median
        long measurement_median = compute_median(last_measurements);

        // Send value
        send_value(measurement_median);
        last_measurements.clear();
    }
}

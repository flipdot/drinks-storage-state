#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <HX711.h>

#include <ArduinoJson.h>

#include "config.hpp"

HX711 * scale;

void setup() {
    delay(1000);

    // init serial
    Serial.begin(115200);
    Serial.println("\n\n");

    // init wifi conection
    Serial.print("connecting to wifi");
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("\nconnected to: %s with IP address %s\n", SSID, WiFi.localIP().toString().c_str());

    // init the scale
    Serial.println("initializing the HX711...");
    scale = new HX711(DATA_PIN, CLOCK_PIN);

    Serial.println("setup complete!\n");
}

int last_measurement = 0;
void loop(void) {
    int measurement = scale->read();
    Serial.println(measurement);

    if(abs(measurement - last_measurement) > DIFFERENCE_THRESHHOLD) {
        // format json
        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        root["espId"] = ESP.getChipId();
        root["scaleValue"] = measurement;
        String jsonString;
        root.printTo(jsonString);

        // make http request
        Serial.printf("\ndoing http request with payload:\n%s\n", jsonString.c_str());
        HTTPClient http;
        http.begin(HTTP_URL);
        int return_code = http.POST(jsonString);
        Serial.printf("finished http request with return code %d\n\n", return_code);
    } else {
        delay(10 * 1000);
    }
}

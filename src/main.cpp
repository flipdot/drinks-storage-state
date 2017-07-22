#include <Arduino.h>
#include <HX711.h>

constexpr byte CLK_PIN = 14;
constexpr byte DATA_PIN = 12;

HX711 scale(DATA_PIN, CLK_PIN);

void setup() {
    //ESP.wdtDisable();
    Serial.begin(115200);
}

void loop() {
    int value = scale.get_value();
    Serial.println(value);
    delay(100);
}

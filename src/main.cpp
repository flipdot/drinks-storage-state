#include <Arduino.h>
#include <HX711.h>

constexpr byte CLK_PIN = 1;
constexpr byte DATA_PIN = 2;

HX711 scale(DATA_PIN, CLK_PIN);

void setup() {
    Serial.begin(9600);

    Serial.println("booted!");
}

void loop() {
    int value = scale.get_value(10);
    Serial.print(value);
    delay(100);
}

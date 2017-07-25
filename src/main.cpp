#include "main.hpp"
#include "config.hpp"

using namespace std;

HX711 * scale;
vector<long> last_measurements;
uint32_t chip_id = ESP.getChipId();

long compute_median(vector<long> values) {
    nth_element(
        values.begin(),
        values.begin() + (M_SAMPLES / 2),
        values.end()
    );
    return values[M_SAMPLES / 2];
}

long send_value(long value) {
    // Format JSON
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["espId"] = chip_id;
    root["scaleValue"] = value;
    String jsonString;
    root.printTo(jsonString);

    // Send HTTP request
    Serial.printf("\nSending HTTP request with payload: %s\n", jsonString.c_str());
    HTTPClient http;
    http.begin(HTTP_URL);
    int return_code = http.POST(jsonString);
    Serial.printf("Finished HTTP request with return code %d.\n", return_code);
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
    Serial.print("Connecting to WiFi.");
    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.printf(
        "\nConnected to: %s with IP address %s\n",
        SSID,
        WiFi.localIP().toString().c_str()
    );

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

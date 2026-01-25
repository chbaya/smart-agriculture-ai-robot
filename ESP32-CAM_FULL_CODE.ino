#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// ================== WIFI ==================
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// ================== SENSORS ==================
#define SOIL_PIN 34
#define DHT_PIN 4
#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);

// ================== ACTUATORS ==================
#define PUMP_PIN 26
#define SPRAY_PIN 27
#define BUZZER 25

// ================== FUTURE GPS (READY) ==================
// GPS module will be added in future
float latitude = 0.0;
float longitude = 0.0;

// ================== DATA ==================
int soilValue;
float temperature;
float humidity;

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);

  pinMode(PUMP_PIN, OUTPUT);
  pinMode(SPRAY_PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  dht.begin();

  digitalWrite(PUMP_PIN, LOW);
  digitalWrite(SPRAY_PIN, LOW);

  // WiFi connection
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

// ================== LOOP ==================
void loop() {
  readSensors();
  smartIrrigation();
  sendDataToFarmer();

  delay(5000); // every 5 seconds
}

// ================== FUNCTIONS ==================
void readSensors() {
  soilValue = analogRead(SOIL_PIN);
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  Serial.println("---- SENSOR DATA ----");
  Serial.print("Soil Moisture: ");
  Serial.println(soilValue);
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);
}

void smartIrrigation() {
  if (soilValue > 2500) {  // DRY SOIL
    digitalWrite(PUMP_PIN, HIGH);
    Serial.println("Irrigation ON");
  } else {
    digitalWrite(PUMP_PIN, LOW);
    Serial.println("Irrigation OFF");
  }
}

// ================== SEND DATA ==================
void sendDataToFarmer() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Example API (later your app / Firebase)
    http.begin("https://example.com/api/robot-data");

    http.addHeader("Content-Type", "application/json");

    String payload = "{";
    payload += "\"soil\":" + String(soilValue) + ",";
    payload += "\"temperature\":" + String(temperature) + ",";
    payload += "\"humidity\":" + String(humidity) + ",";
    payload += "\"lat\":" + String(latitude) + ",";
    payload += "\"lng\":" + String(longitude);
    payload += "}";

    int httpResponseCode = http.POST(payload);

    Serial.print("Data sent, response: ");
    Serial.println(httpResponseCode);

    http.end();
  }
}

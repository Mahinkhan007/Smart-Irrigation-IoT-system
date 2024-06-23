#define BLYNK_TEMPLATE_ID "TMPL6l933TRnv"
#define BLYNK_TEMPLATE_NAME "Smart Farming project"
#define BLYNK_AUTH_TOKEN "tsY5y-L_M80dBJelifUHypta6mfVNIxJ"
#define BLYNK_PRINT Serial 
#include <WiFiClient.h> 

#include <DHT.h>         // Include the DHT library
#include <WiFi.h>        // Include the WiFi library
#include <BlynkSimpleEsp32.h> // Include the Blynk library
#define BLYNK_TEMPLATE_ID "TMPL6l933TRnv"
#define BLYNK_TEMPLATE_NAME "Shehab project"

// Defining the Blynk auth token, WiFi SSID, and password
char auth[] = "tsY5y-L_M80dBJelifUHypta6mfVNIxJ";
char ssid[] = "Shebowifi";
char pass[] = "Sheb0011";

// Defining the pins for sensors and actuators
#define DHTPIN 4          // Pin where the DHT11 is connected
#define DHTTYPE DHT11     // Type of DHT sensor
#define SOIL_MOISTURE_PIN 34 // Pin for soil moisture sensor
#define WATER_LEVEL_PIN 32   // Pin for water level sensor
#define SOLENOID_VALVE_PIN 27 // Pin to control solenoid valve
#define PUMP_PIN 23          // Pin to control water pump relay

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

void setup() {
  // Start serial communication
  Serial.begin(115200);         
  // Initialize the DHT sensor
  dht.begin();
  // Set pin modes
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(WATER_LEVEL_PIN, INPUT);
  pinMode(SOLENOID_VALVE_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  // Ensure solenoid valve is closed initially
  digitalWrite(SOLENOID_VALVE_PIN, HIGH);
  // Ensure water pump is off initially
  digitalWrite(PUMP_PIN, LOW);

  // Initialize Blynk connection
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  // Run Blynk
  Blynk.run();
  
  // Read humidity and temperature from DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    // Send humidity and temperature data to Blynk app
    Blynk.virtualWrite(V1, humidity);
    Blynk.virtualWrite(V2, temperature);

    // Print humidity and temperature to Serial Monitor
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
  }

  // Read soil moisture sensor
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  int moisturePercent = map(soilMoisture, 0, 4095, 0, 100);

  // Print soil moisture to Serial Monitor
  Serial.print("Soil Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");

  // Read water level sensor
  int waterLevel = digitalRead(WATER_LEVEL_PIN);

  // If soil moisture is below a certain threshold, water the plants
  if (moisturePercent < 30) { // Adjust threshold as needed
    digitalWrite(PUMP_PIN, HIGH);            // Turn on water pump
    Serial.println("Soil is dry. Watering plants...");
  } else {
    digitalWrite(PUMP_PIN, LOW);             // Turn off water pump
    Serial.println("Soil moisture is adequate. Not watering.");
  }

  // If water level is high, close the solenoid valve
  if (waterLevel == HIGH) {
    digitalWrite(SOLENOID_VALVE_PIN, HIGH);  // Close solenoid valve
    Serial.println("Water level is high. Solenoid valve closed.");
  } else {
    digitalWrite(SOLENOID_VALVE_PIN, LOW);   // Open solenoid valve
    Serial.println("Water level is low. Solenoid valve open.");
  }

  delay(1000);  // Delay between readings
}

// Blynk function to control the water pump manually
BLYNK_WRITE(V3) {
  int pumpState = param.asInt();
  digitalWrite(PUMP_PIN, pumpState);
  if (pumpState == HIGH) {
    Serial.println("Manual control: Water pump ON");
  } else {
    Serial.println("Manual control: Water pump OFF");
  }
}
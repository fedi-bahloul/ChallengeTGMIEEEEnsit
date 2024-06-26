#include <ESP8266WiFi.h>
#include <DHT.h>

// Pin definitions
#define DHTPIN D5
#define DHTTYPE DHT11
#define SOIL_MOISTURE_PIN A0
#define CO2_SENSOR_PIN A1
#define WATER_LEVEL_PIN A2
#define PUMP_PIN D6

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  
  // Initialize DHT11
  dht.begin();
  
  // Set up the pump pin
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  
  // Initialize WiFi (if needed for data transmission)
  // WiFi.begin("yourSSID", "yourPASSWORD");
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi...");
  // }
  // Serial.println("Connected to WiFi");
}

void loop() {
  // Read DHT11 sensor data
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Read soil moisture
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  float soilMoisture = map(soilMoistureValue, 0, 1023, 0, 100); // Adjust based on calibration
  
  // Read CO2 sensor data
  int co2Value = analogRead(CO2_SENSOR_PIN);
  
  // Read water level
  int waterLevelValue = analogRead(WATER_LEVEL_PIN);
  
  // Print sensor values
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  
  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisture);
  Serial.println(" %");
  
  Serial.print("CO2 Level: ");
  Serial.println(co2Value); // This will need conversion to PPM
  
  Serial.print("Water Level: ");
  Serial.println(waterLevelValue); // This may need conversion
  
  // Control pump based on soil moisture
  if (soilMoisture < 30) { // Threshold value for irrigation
    digitalWrite(PUMP_PIN, HIGH); // Turn on pump
  } else {
    digitalWrite(PUMP_PIN, LOW); // Turn off pump
  }
  
  // Delay before next reading
  delay(2000); // Adjust delay as needed
}

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change 0x27 to your LCD's I2C address if necessary

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define MQ135_PIN A0    // Analog pin connected to the MQ-135 sensor
#define RELAY_PIN 8     // Digital pin connected to the relay module

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx and MQ135 Test!"));

  // Initialize the LCD
  lcd.begin();      // Initialize the LCD without parameters
  lcd.backlight();  // Turn on the backlight of the LCD
  
  dht.begin();

  // Initialize the relay pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Start with the relay off for active LOW
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DHT Read Error!");
    return;
  }

  // Compute heat index in Celsius
  float hic = dht.computeHeatIndex(t, h, false);

  // Read MQ135 sensor value
  int mq135_value = analogRead(MQ135_PIN);
  // Convert analog reading to PPM (this is a simplified version; calibrate for accuracy)
  float airQualityPPM = mq135_value * (5.0 / 1023.0) * 100; // Example conversion

  // Debugging: Print sensor values
  Serial.print(F("Temperature: "));
  Serial.println(t);
  Serial.print(F("Air Quality PPM: "));
  Serial.println(airQualityPPM);

  // Control the relay based on temperature and air quality
  if (t > 30 || airQualityPPM > 9) {
    digitalWrite(RELAY_PIN, LOW);  // Turn on the relay (active LOW)
  } else {
    digitalWrite(RELAY_PIN, HIGH); // Turn off the relay (active LOW)
  }

  // Print values to Serial Monitor
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C  Heat index: "));
  Serial.print(hic);
  Serial.println(F("°C"));

  Serial.print(F("Air Quality: "));
  Serial.print(airQualityPPM);
  Serial.println(F(" PPM"));

  // Print values to the LCD
  lcd.clear();
  lcd.setCursor(0, 0);  // Set cursor to first row
  lcd.print("H:");
  lcd.print(h);
  lcd.print("% T:");
  lcd.print(t);
  lcd.print((char)223); // Display the degree symbol
  lcd.print("C");

  lcd.setCursor(0, 1);  // Set cursor to second row
  lcd.print("HI:");
  lcd.print(hic);
  lcd.print((char)223); // Display the degree symbol
  lcd.print("C AQ:");
  lcd.print(airQualityPPM);
  lcd.print("ppm");
}

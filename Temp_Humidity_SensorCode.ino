#include "CTBot.h"
CTBot myBot;

#include "DHT.h"
DHT dht(2, DHT11);

String ssid = "Hacker";
String pass = "mo123456";
String token = "6664003264:AAE5TvYOSxDvihm5f2tqpQaBCAa7b_KgRpM";
uint8_t redLed = D1;
uint8_t blueLed = D2;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting TelegramBot...");

  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  if (myBot.testConnection())
    Serial.println("\ntestConnection OK");
  else
    Serial.println("\ntestConnection NOK");

  pinMode(redLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  digitalWrite(redLed, HIGH); // turn off the red LED (inverted logic!)
  digitalWrite(blueLed, HIGH); // turn off the blue LED (inverted logic!)
}

void loop() {
  static unsigned long lastUpdateTime = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastUpdateTime >= 3000) { // 30 minutes in milliseconds
    lastUpdateTime = currentTime;

    float temperature = readDHTTemperature();
    float humidity = readDHTHumidity();

    if (!isnan(temperature) && !isnan(humidity)) {
      String message = "Temperature: " + String(temperature, 2) + "°C\nHumidity: " + String(humidity, 2) + "%";
      myBot.sendMessage(975906125, message);
      

      // Control LEDs based on temperature
      if (temperature < 28.0) {
        digitalWrite(redLed, LOW);  // turn on the red LED (inverted logic!)
        digitalWrite(blueLed, HIGH); // turn off the blue LED (inverted logic!)
      } else {
        digitalWrite(redLed, HIGH); // turn off the red LED (inverted logic!)
        digitalWrite(blueLed, LOW);  // turn on the blue LED (inverted logic!)
      }
    } else {
      Serial.println("Error reading DHT sensor");
    }
  }

  // Other parts of your loop (handling commands) go here
}

float readDHTTemperature() {
  // Retry mechanism
  for (int i = 0; i < 3; ++i) {
    float temp = dht.readTemperature();
    if (!isnan(temp)) {
      return temp;
    }
    delay(500);
  }
  return NAN; // Return NAN if reading fails
}

float readDHTHumidity() {
  // Retry mechanism
  for (int i = 0; i < 3; ++i) {
    float hum = dht.readHumidity();
    if (!isnan(hum)) {
      return hum;
    }
    delay(500);
  }
  return NAN; // Return NAN if reading fails
}

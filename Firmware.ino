// Include Libraries
#include "Arduino.h"
#include "SoilMoisture.h"

// Pin Definitions
#define SOILMOISTURE_5V_PIN_SIG	A3
#define WATERLEVELSENSOR_5V_PIN_SIG	A5

const int waterLevelPin = A5; // Analog pin water level sensor is connected to
const int relaySoilMoisturePin = A0; // Output pin for soil moisture relay
const int relayWaterLevelPin = A2; // Output pin for water level relay

// Global variables and defines

// Object initialization
SoilMoisture soilMoisture_5v(SOILMOISTURE_5V_PIN_SIG);

// Timing variables
unsigned long lastSoilMoistureCheckTime = 0;
unsigned long lastWaterLevelCheckTime = 0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");

    pinMode(relaySoilMoisturePin, OUTPUT); // Set relaySoilMoisturePin as an output
    pinMode(relayWaterLevelPin, OUTPUT); // Set relayWaterLevelPin as an output
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    // Check Soil Moisture every 5 seconds
    if (millis() - lastSoilMoistureCheckTime > 5000)
    {
        int soilMoisture_5vVal = soilMoisture_5v.read();
        Serial.print(F("Soil Moisture Value: ")); 
        Serial.println(soilMoisture_5vVal);

        // Check soil moisture conditions
        if (soilMoisture_5vVal < 350)
        {
            digitalWrite(relaySoilMoisturePin, HIGH); // Turn on relay
        }
        else if (soilMoisture_5vVal > 400)
        {
            digitalWrite(relaySoilMoisturePin, LOW); // Turn off relay
        }

        lastSoilMoistureCheckTime = millis();
    }

    // Check Water Level every 5 seconds
    if (millis() - lastWaterLevelCheckTime > 5000)
    {
        int waterLevelValue = analogRead(waterLevelPin);
        Serial.print(F("Water Level Sensor Value: ")); 
        Serial.println(waterLevelValue);

        // Check water level conditions
        if (waterLevelValue < 100)
        {
            digitalWrite(A2, HIGH); // Turn on relay
        }
        else if (waterLevelValue > 600)
        {
            digitalWrite(A2, LOW); // Turn off relay
        }

        lastWaterLevelCheckTime = millis();
    }
}

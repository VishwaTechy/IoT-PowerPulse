#define BLYNK_TEMPLATE_ID "TMPL3Zxs2rgjC"
#define BLYNK_TEMPLATE_NAME "Power Load Detection"
#define BLYNK_PRINT Serial

#include "EmonLib.h"
#include <EEPROM.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // LCD with I2C address 0x27, and 16x2 format

// Constants for calibration
const float vCalibration = 93.3;
const float currCalibration = 0.52;

// Blynk and WiFi credentials
const char auth[] = "9AJH_t-TnhXQFf1xEFP7SAmkK5sIKkxw";
const char ssid[] = "Redmi Note 8";
const char pass[] = "1234567890";

// EnergyMonitor instance
EnergyMonitor emon;

// Timer for regular updates
BlynkTimer timer;

// Variables for energy calculation
float kWh = 0.0;
unsigned long lastMillis = millis();

// EEPROM addresses for each variable
const int addrKWh = 12;

// Relay pin
const int relayPin = 4; // GPIO4 connected to the relay

// Voltage threshold
const float voltageThreshold = 95.0; // Vrms threshold in volts

void sendEnergyDataToBlynk();
void readEnergyDataFromEEPROM();
void saveEnergyDataToEEPROM();

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();

  // Initialize EEPROM with the size of the data to be stored
  EEPROM.begin(32); // Allocate 32 bytes for float values (4 bytes each) and some extra space

  // Initialize the relay pin
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Default state: relay OFF

  // Read the stored energy data from EEPROM
  readEnergyDataFromEEPROM();

  // Setup voltage and current inputs
  emon.voltage(35, vCalibration, 1.7); // Voltage: input pin, calibration, phase_shift
  emon.current(34, currCalibration);    // Current: input pin, calibration

  // Setup a timer for sending data every 5 seconds
  timer.setInterval(5000L, sendEnergyDataToBlynk);

  // A small delay for system to stabilize
  delay(1000);
}

void loop() {
  Blynk.run();
  timer.run();
}

void sendEnergyDataToBlynk() {
  emon.calcVI(20, 2000); // Calculate all. No.of half wavelengths (crossings), time-out

  // Calculate energy consumed in kWh
  unsigned long currentMillis = millis();
  kWh += emon.apparentPower * (currentMillis - lastMillis) / 3600000.0; // Corrected divisor for kWh
  lastMillis = currentMillis;

  Serial.printf("Vrms: %.2fVoltage\tIrms: %.4fAmpere\tPower: %.4fWatt\tkWh: %.5fkWh\n",
                emon.Vrms, emon.Irms, emon.apparentPower, kWh);

  // Save the latest values to EEPROM
  saveEnergyDataToEEPROM();

  // Send data to Blynk
  Blynk.virtualWrite(V0, emon.Vrms);
  Blynk.virtualWrite(V1, emon.Irms);
  Blynk.virtualWrite(V2, emon.apparentPower);
  Blynk.virtualWrite(V3, kWh);

  // Log an event if voltage exceeds the threshold
if (emon.Vrms > voltageThreshold) {
digitalWrite(relayPin, HIGH);
Blynk.logEvent("powerload", "Power Load is High");
Serial.println("Event logged: Voltage is High");
} else {
digitalWrite(relayPin, LOW);
}

  // Update the LCD with the new values
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vrms:");
  lcd.print(emon.Vrms, 2);
  lcd.print(" Voltage");

  lcd.setCursor(0, 1);
  lcd.print("Irms:");
  lcd.print(emon.Irms, 2);
  lcd.print(" Ampere");

  lcd.setCursor(0, 2);
  lcd.print("Power:");
  lcd.print(emon.apparentPower, 2);
  lcd.print(" Watt");

  lcd.setCursor(0, 3);
  lcd.print("kWh:");
  lcd.print(kWh, 5);

  delay(1000); // Adding delay to allow the LCD to update6+++++++++++




  

}

void readEnergyDataFromEEPROM() {
  // Read the stored kWh value from EEPROM
  EEPROM.get(addrKWh, kWh);

  // Check if the read value is a valid float. If not, initialize it to zero
  if (isnan(kWh)) {
    kWh = 0.0;
    saveEnergyDataToEEPROM(); // Save initialized value to EEPROM
  }
}

void saveEnergyDataToEEPROM() {
  // Write the current kWh value to EEPROM
  EEPROM.put(addrKWh, kWh);


  // Commit changes to EEPROM
  EEPROM.commit();
}

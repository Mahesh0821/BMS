// Include necessary libraries
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_INA219.h>
#include <Adafruit_MCP9808.h>

// Create sensor objects
Adafruit_ADS1115 ads;        // ADC for voltage measurement
Adafruit_INA219 ina219;      // Current sensor
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808(); // Temperature sensor

// Constants
const float voltageDividerRatio = 5.7; // Adjust based on your resistor values
const float maxVoltage = 4.2;          // Max voltage per cell
const float minVoltage = 3.0;          // Min voltage per cell
const float maxCurrent = 2.0;          // Max current in amperes
const float maxTemperature = 60.0;     // Max temperature in Celsius

// Variables
float batteryVoltage;
float batteryCurrent;
float batteryTemperature;
float stateOfCharge;

// Function prototypes
void checkVoltage();
void checkCurrent();
void checkTemperature();
void calculateStateOfCharge();
void handleFault(String faultMessage);

void setup() {
  Serial.begin(9600);
  
  // Initialize sensors
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }

  if (!ina219.begin()) {
    Serial.println("Failed to initialize INA219.");
    while (1);
  }

  if (!tempsensor.begin()) {
    Serial.println("Failed to initialize MCP9808.");
    while (1);
  }

  // Set temperature sensor resolution
  tempsensor.setResolution(3); // 0.0625 degrees per LSB

  Serial.println("Battery Management System Initialized.");
}

void loop() {
  checkVoltage();
  checkCurrent();
  checkTemperature();
  calculateStateOfCharge();

  Serial.print("Voltage: ");
  Serial.print(batteryVoltage);
  Serial.print(" V, Current: ");
  Serial.print(batteryCurrent);
  Serial.print(" A, Temperature: ");
  Serial.print(batteryTemperature);
  Serial.print(" C, State of Charge: ");
  Serial.print(stateOfCharge);
  Serial.println(" %");

  delay(1000); // Delay between measurements
}

void checkVoltage() {
  int16_t adc0 = ads.readADC_SingleEnded(0);
  batteryVoltage = adc0 * (0.1875 / 1000) * voltageDividerRatio;

  if (batteryVoltage > maxVoltage) {
    handleFault("Overvoltage detected!");
  } else if (batteryVoltage < minVoltage) {
    handleFault("Undervoltage detected!");
  }
}

void checkCurrent() {
  batteryCurrent = ina219.getCurrent_mA() / 1000.0;

  if (batteryCurrent > maxCurrent) {
    handleFault("Overcurrent detected!");
  }
}

void checkTemperature() {
  batteryTemperature = tempsensor.readTempC();

  if (batteryTemperature > maxTemperature) {
    handleFault("Overtemperature detected!");
  }
}

void calculateStateOfCharge() {
  // Simple linear SOC estimation (replace with a more accurate algorithm as needed)
  stateOfCharge = ((batteryVoltage - minVoltage) / (maxVoltage - minVoltage)) * 100;
  if (stateOfCharge > 100) stateOfCharge = 100;
  if (stateOfCharge < 0) stateOfCharge = 0;
}

void handleFault(String faultMessage) {
  Serial.println(faultMessage);
  // Implement additional fault handling logic here (e.g., shut down the system, notify user, etc.)
  while (1); // Halt the system in case of fault
}

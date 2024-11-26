#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

const int analogPin = A0;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  dac.begin(0x62); // Replace with your actual I2C address (0x62)
}

void loop() {
  // Manually set the desired voltage between 0 and 5 volts for the DAC
  float dacVoltage = 0.01;                

  // Convert DAC voltage to DAC value (0-4095)
  uint16_t dacValue = (dacVoltage / 5.0) * 4095;

  // Set the DAC output
  dac.setVoltage(dacValue, false);
  delay(1000);

  // Read the analog voltage from pin A0
  int analogValue = analogRead(analogPin);
  float analogVoltage = (analogValue * 5.0) / 1023.0;
   delay(1000);

  Serial.print("Setting voltage on DAC to: ");
  Serial.print(dacVoltage, 2);
  Serial.println(" V");

  Serial.print("Analog voltage: ");
  Serial.print(analogVoltage, 2);
  Serial.println(" V");

  delay(1000); // Wait for 10 seconds before updating again
}

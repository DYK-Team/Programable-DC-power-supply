//
// A sketch for the programmable DC power supply based on Arduino Uno WiFi Rev2.
// This power supply monitors output voltages from two outputs: (RED) [0-20] V, 8A and (BLACK) [0-4.2] V, 250 mA.
//
// Authors:
//
// Mr. Egor Shevchenko, Dr. Nikolay Udanov, Prof. Larissa Panina, MISIS, Moscow, https://en.misis.ru/ 
//
// Dr. Dmitriy Makhnovskiy, DYK team, United Kingdom, https://www.dykteam.com 
//

#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <math.h>

// Global constants
const int analogA0 = A0; // Analog pin A0 used for reading the output voltage from the DAC driver
const int analogA1 = A1; // Analog pin A1 used for reading the output voltage from the PWM driver
const int pwmPin = 9; // Define the PWM pin as pin 9
const float max_v1 = 20.0;  // Maximum voltage for channel 1
const float min_v1 = -20.0; // Minimum voltage for channel 1
const float max_v2 = 4.2;   // Maximum voltage for channel 2; Minimum voltage = 0.0 V
const float op_amp_gain = 5.9567;  // Gain of OPA549 for DAC output [0-5] V
const float A0_coeff = 5.4833;  // A0 reading to the voltage output coefficient
const int tau = 220;  // Characteristic time (ms) of the RC filter with C = 100 uF and R = 2k2
const float bs_DAC =  1.5 * (5.0 / 1023.0) * A0_coeff;  // bit sensitivity for DAC output
const float bs_PWM =  1.5 * (5.0 / 255.0);  // bit sensitivity for PWM output  

// Variables
String chan1, chan2;
int v_raw, dutyCycle;
int DAC_value;
float v1, v2, v1_out, v2_out;

// Declare LCD and DAC objects as global variables
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // Pins used for the LCD module
Adafruit_MCP4725 dac;

// Variables for some previous values
String previous_pin12_13 = "HIGH";  // Relays are OFF

bool flag_send = 1, 
     sign = 0, //1 if '-' and 0 if '+'
     previous_sign = 0;  // Polarity of the RED output
     
void setup() { 

  Serial.begin(19200); // Initialize serial communication
  delay(100);
  // Initialize the pins as outputs
  pinMode(8, OUTPUT);  // LED "+" for the Red out channel
  pinMode(10, OUTPUT);  // LED "+" for the Red out channel
  pinMode(11, OUTPUT);  // LED "+" for the Black out channel
  pinMode(12, OUTPUT);  // Relay 2; H-bridge
  pinMode(13, OUTPUT);  // Relay 1; H-bridge
  pinMode(pwmPin, OUTPUT); // PWM for pwmPin = 9
  
  // Initialize DAC and the Red output channel
  dac.begin(0x62); // I2C address (0x62) of DAC
  dac.setVoltage(0, false);  // DAC is OFF
  digitalWrite(8, HIGH);  // LED ON 
  digitalWrite(10, LOW);  // LED OFF

  // Initialize PWM and the Black output channel
  pinMode(pwmPin, OUTPUT); // pwmPin = 9
  analogWrite(pwmPin, 0);  // PWM is OFF
  digitalWrite(11, LOW);  // LED OFF
 
  // Relays controlling the polarity on the red output channel
  digitalWrite(12, HIGH);  // Relay 2 is OFF; activated by LOW
  digitalWrite(13, HIGH);  // Relay 1 is OFF; activated by LOW 

  // Initialize the LCD: 16 columns, 2 rows
  lcd.begin(16, 2);   // Initialize the LCD: 16 columns, 2 rows
  lcd.clear();  // Clear the LCD and set the cursor position
  lcd.setCursor(0, 0);
  lcd.print("Red: OFF");  // Print the formatted voltage value
  lcd.setCursor(0, 1);
  lcd.print("Black: OFF");  // Print the formatted voltage value
  delay(100);
}

float DAC_Vout(int DAC_value){
  dac.setVoltage(DAC_value, false);  // Set the DAC output
  delay(10);
  v_raw = analogRead(analogA0);  // Read the digital value from A0
  delay(10);
  return (v_raw / 1023.0) * 5.0 * A0_coeff;  // Output voltage as seen by the A0 analog input
}

float PWM_Vout(int dutyCycle) {
  // Change the duty cycle by specifying a value between 0 (off) and 255 (full on)
  analogWrite(pwmPin, dutyCycle);  // Set the duty cycle
  delay(tau * 6.0);  // Delay time to allow the full capacitor charge
  v_raw = analogRead(analogA1);  // Read the analog value from A1
  delay(10);
  return (v_raw / 1023.0) * 5.0;  // Output voltage as seen by the A1 analog input
}

void displayVoltage(int row, int column, String name, float voltage) {
  lcd.setCursor(column, row);
  lcd.print(String(name + ": " + String(voltage) + " V"));  // Print the formatted voltage value
}

String receivedData = "";
void ParseSerial() {
  char space = '0';
  String temp_string = "";
  for (int i = 0; i < receivedData.length(); i++) {
      if (receivedData[i] == ' ')
      {
        space++;
        switch (space) {
          case '1': {chan1 = temp_string; temp_string = ""; break; }
          case '2': {v1 = temp_string.toFloat();  temp_string = ""; break; }
          case '3': {chan2 = temp_string; temp_string = ""; break; }
        }
      }
    else if (i == receivedData.length() - 1)
        v2 = temp_string.toFloat();
    else
        temp_string += receivedData[i];
  }
}

void loop() {
  // Check if data is available to read
  while (receivedData[receivedData.length() - 1] != '\n') {
    if (Serial.available()){
      receivedData += (char) Serial.read();
      if (receivedData[receivedData.length() - 1] == '\n')
      {
        receivedData.toLowerCase();
        ParseSerial();
        lcd.clear();
      }
   }
  } 

  // Check if v1 is within the range min_v1 <= v1 <= max_v1 and v2 is within 0 <= v2 <= max_v2
  bool flag_oor = 0;  // 'oor' - 'out of range' key word
  if (chan1 == "on" and (v1 < min_v1 or v1 > max_v1)) {
      flag_oor = 1;
      lcd.setCursor(0, 0);
      lcd.print("Red: out range  "); //Red == v1
  }
  if (chan2 == "on" and (v2 < 0 or v2 > max_v2)) {
      flag_oor = 1;
      lcd.setCursor(0, 1);
      lcd.print("Black: out range"); //Black == v2
  } 
  if (flag_oor)
    return;
  // Red output channel
  if (chan1 == "on") {
    sign = signbit(v1); //1 if '-' and 0 if '+'   
    if (sign != previous_sign) {
      dac.setVoltage(0, false);
      previous_sign = sign;
      if (previous_pin12_13 == "HIGH") {
        digitalWrite(8, LOW);  
        digitalWrite(10, HIGH);
        digitalWrite(12, LOW);
        digitalWrite(13, LOW);
        previous_pin12_13 = "LOW"; 
      } else {
        digitalWrite(8, HIGH);  
        digitalWrite(10, LOW);
        digitalWrite(12, HIGH);
        digitalWrite(13, HIGH);
        previous_pin12_13 = "HIGH";         
      }
    }
    
    DAC_value = round(((fabs(v1) / op_amp_gain) / 5.0) * 4095.0);  // Convert the DAC voltage to the DAC value (0-4095)
    v1_out = DAC_Vout(DAC_value);  // Output voltage how it is seen by the A0 analog input
    while ((fabs(v1_out - fabs(v1)) > bs_DAC) ) {
        if (v1_out > fabs(v1) and DAC_value > 0)
          DAC_value--;
        else if (v1_out < fabs(v1) and DAC_value < 4095)
          DAC_value++;
        v1_out = DAC_Vout(DAC_value);
    }
    displayVoltage(0, 0, "Red", v1_out); // Display the output voltage
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Red: OFF        ");  // 16 symbols in the row, including spaces
  }

// Black output channel
  if (chan2 == "on") {
    digitalWrite(11, HIGH);  // LED ON
    dutyCycle = v2 * 51;  // Convert the PWM voltage to the duty cycle value (0-255)
    v2_out = PWM_Vout(dutyCycle);  // Output voltage how it is seen by the A1 analog input
    
    while ((fabs(v2_out - v2) > bs_PWM) ){
        if (v2_out > v2 and dutyCycle > 0)
          dutyCycle--;
        else if (v2_out < v2 and dutyCycle < 255)
          dutyCycle++;
        v2_out = PWM_Vout(dutyCycle);
    }
    displayVoltage(1, 0, "Black", v2_out); // Display the output voltage
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Black: OFF      ");  // 16 symbols in the row, including spaces
  }
  if (flag_send){ 
    Serial.println( ((sign) ? ("-") : ("")) + String(v1_out) + " " + String(v2_out));
    flag_send = 0;
  }
  delay(100);
}

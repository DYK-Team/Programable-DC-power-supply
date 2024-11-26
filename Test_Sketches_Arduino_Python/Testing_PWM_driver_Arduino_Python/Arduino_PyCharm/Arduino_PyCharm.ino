#include <Arduino.h>

const int pwmPin1 = 9;  // PWM pin 1
int dutyCycle1 = 0;     // Duty cycle for PWM pin 1

void setup() {
  pinMode(pwmPin1, OUTPUT);
  analogWrite(pwmPin1, dutyCycle1);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    dutyCycle1 = Serial.parseInt(); // Read duty cycle value from Python
    analogWrite(pwmPin1, dutyCycle1);
  }
  delay(100);

  // Read analog values from analog pin
  int analogValue1 = analogRead(A1);
  delay(1500);

  // Send analog value to Python
  Serial.println(analogValue1);
}
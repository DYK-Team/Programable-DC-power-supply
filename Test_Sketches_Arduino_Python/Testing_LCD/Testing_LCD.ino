#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("Nikolay Udanov");
  lcd.setCursor(0, 1); 
  lcd.print("MISiS, Moscow"); // Print the second line
  
}

void loop() {
  // Write-only operations here, you don't need to read from the LCD.
}
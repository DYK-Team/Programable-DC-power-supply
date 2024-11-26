// Define the LED pins
const int ledPin1 = 8;
const int ledPin2 = 10;
const int ledPin3 = 11;

void setup() {
  // Initialize the LED pins as OUTPUT
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
}

void loop() {
  // Turn on the LEDs
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);

  // Wait for a period
  delay(1000); // Adjust the delay time (in milliseconds) as needed

  // Turn off the LEDs
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);

  // Wait for another period
  delay(1000); // Adjust the delay time (in milliseconds) as needed
}
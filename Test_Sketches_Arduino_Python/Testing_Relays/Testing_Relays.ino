const int relayPin1 = 12; // Pin connected to the first relay
const int relayPin2 = 13; // Pin connected to the second relay

void setup() {
  pinMode(relayPin1, OUTPUT); // Set relayPin1 as an output
  pinMode(relayPin2, OUTPUT); // Set relayPin2 as an output
}

void loop() {
  digitalWrite(relayPin1, HIGH); // Turn on the first relay
  digitalWrite(relayPin2, HIGH); // Turn on the second relay
  delay(3000); // Wait for 2 seconds
  
  digitalWrite(relayPin1, LOW); // Turn off the first relay
  digitalWrite(relayPin2, LOW); // Turn off the second relay
  delay(3000); // Wait for 2 seconds
}

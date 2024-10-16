void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud rate
}

void loop() {
  int sensorValue = analogRead(A0); // Read the value from the potentiometer
  float voltage = sensorValue * (5.0 / 1023.0); // Convert the ADC value to voltage
  Serial.print("Voltage = ");
  Serial.print(voltage);
  Serial.print(" V.");
  Serial.println(); // Print a newline character
  delay(100); // Wait for 1 second before the next reading
}

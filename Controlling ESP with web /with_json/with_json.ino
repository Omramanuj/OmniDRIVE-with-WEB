const int motorDirPin = 13;   // Custom pin for motor direction
const int motorPWMPin = 12;   // Custom pin for motor PWM speed control

void setup() {
  pinMode(motorDirPin, OUTPUT);
  pinMode(motorPWMPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Run motor forward
  digitalWrite(motorDirPin, HIGH);  // Set direction
  analogWrite(motorPWMPin, 200);     // Set speed (0-255)
  delay(2000); // Run for 2 seconds

  // // Stop motor
  // analogWrite(motorPWMPin, 0);
  // delay(1000); // Pause for 1 second

  // // Run motor backward
  // digitalWrite(motorDirPin, LOW);   // Set direction
  // analogWrite(motorPWMPin, 150);     // Set speed
  // delay(2000); // Run for 2 seconds

  // // Stop motor
  // analogWrite(motorPWMPin, 0);
  // delay(1000); // Pause for 1 second
}
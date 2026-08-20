// Pin Definitions
const int ENA_PIN = 5; // PWM Pin for Motor Speed (D5)
const int IN1_PIN = 6; // Direction Pin 1 (D6)
const int IN2_PIN = 7; // Direction Pin 2 (D7)

void setup() {
  // Initialize Serial Communication at 9600 baud
  Serial.begin(9600);
  
  // Set motor control pins as outputs
  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);

  // Ensure motor is stopped initially
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  analogWrite(ENA_PIN, 0);

  // Diagnostic Status Output to Serial Monitor
  Serial.println("==================================================");
  Serial.println("Arduino Nano Motor Controller Initialized!");
  Serial.println("L298N Pins Configured:");
  Serial.println("  - ENA -> Pin D5 (PWM Speed)");
  Serial.println("  - IN1 -> Pin D6 (Direction 1)");
  Serial.println("  - IN2 -> Pin D7 (Direction 2)");
  Serial.println("Connections Settled: ALL GOOD!");
  Serial.println("Motor status: READY");
  Serial.println("==================================================");
}

void loop() {
  // ==========================================
  // 1. FORWARD AT FULL SPEED (PWM = 255)
  // ==========================================
  Serial.println("[STATUS] Motor: FORWARD | Speed: FULL (255)");
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  analogWrite(ENA_PIN, 255); // Full Speed
  delay(3000); // Spin for 3 seconds

  // ==========================================
  // 2. FORWARD AT HALF SPEED (PWM = 128)
  // ==========================================
  Serial.println("[STATUS] Motor: FORWARD | Speed: HALF (128)");
  analogWrite(ENA_PIN, 128); // Half Speed
  delay(3000); // Spin for 3 seconds

  // ==========================================
  // 3. BRAKE / STOP MOTOR
  // ==========================================
  Serial.println("[STATUS] Motor: STOPPED");
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  analogWrite(ENA_PIN, 0);
  delay(2000); // Pause for 2 seconds

  // ==========================================
  // 4. REVERSE SPEED RAMP UP (0 to 255)
  // ==========================================
  Serial.println("[STATUS] Motor: REVERSE | Ramping Speed Up...");
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH); // Reverse direction

  for (int speedVal = 0; speedVal <= 255; speedVal += 25) {
    analogWrite(ENA_PIN, speedVal);
    Serial.print("Current Speed: ");
    Serial.println(speedVal);
    delay(200);
  }

  // ==========================================
  // 5. STOP BEFORE REPEATING CYCLE
  // ==========================================
  Serial.println("[STATUS] Motor: STOPPED | Cycle Complete.\n");
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  analogWrite(ENA_PIN, 0);
  delay(2000);
}
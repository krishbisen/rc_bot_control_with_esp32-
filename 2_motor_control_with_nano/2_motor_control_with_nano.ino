// Motor A Pins
const int ENA = 14;
const int IN1 = 26;
const int IN2 = 27;

// Motor B Pins
const int ENB = 25;
const int IN3 = 32;
const int IN4 = 33;

void setup() {
  Serial.begin(115200);
  
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.println("System Ready: Motor Control Initialized.");
}

// Helper Function: Clockwise Movement
void clockwise(int speed) {
  Serial.print("State: CLOCKWISE | Speed: ");
  Serial.println(speed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

// Helper Function: Anti-Clockwise Movement
void antiClockwise(int speed) {
  Serial.print("State: ANTI-CLOCKWISE | Speed: ");
  Serial.println(speed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

// Helper Function: Stop Motors
void stopMotors() {
  Serial.println("State: STOPPED");

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void loop() {
  // 1. Move Clockwise at speed 200 for 3 seconds
  clockwise(200);
  delay(3000);

  // 2. Stop for 1.5 seconds
  stopMotors();
  delay(1500);

  // 3. Move Anti-Clockwise at speed 200 for 3 seconds
  antiClockwise(200);
  delay(3000);

  // 4. Stop for 1.5 seconds
  stopMotors();
  delay(1500);
}